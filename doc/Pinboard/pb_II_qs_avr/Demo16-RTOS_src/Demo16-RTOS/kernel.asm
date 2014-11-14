EEPROMRead:
ERL01:	sbic	EECR,EEWE	;if EEWE not clear
		rjmp	ERL01		; wait more 

		out	EEARH, ZH	;output address 
		out	EEARL, ZL	;output address 

		sbi	EECR, EERE	;set EEPROM Read strobe
		in	OSRG, EEDR	;get data
		ret
	
EEPROMWrite:
EWL01:	sbic	EECR,EEWE	;if EEWE not clear
		rjmp	EWL01		;    wait more
	
		out	EEARH, ZH	
		out	EEARL, ZL	

		out	EEDR,OSRG	;output data
		sbi	EECR,EEMWE	;set EEPROM Master Write Enable
		sbi	EECR,EEWE	;set EEPROM Write strobe
		ret

ClearTaskQueue:
		push ZL
		push ZH

		ldi ZL, low(TaskQueue)
		ldi ZH, high(TaskQueue)

		ldi OSRG, $FF		
		ldi Counter, TaskQueueSize

CEQL01: st Z+, OSRG		;
		dec Counter		;
		brne CEQL01		; Loop

		pop ZH
		pop ZL
		ret
	
ClearTimers:
		push ZL
		push ZH

		ldi ZL, low(TimersPool)
		ldi ZH, high(TimersPool)

		ldi Counter, TimersPoolSize
		ldi OSRG, $FF		; Empty 
		ldi Tmp2, $00

CTL01:	st Z+, OSRG		; Event
		st Z+, Tmp2		; Counter Lo
		st Z+, Tmp2		; Counter Hi

		dec Counter		;
		brne CTL01		; Loop
	
		pop ZH
		pop ZL
		ret	
;------------------------------------------------------------------------------




ProcessTaskQueue:
		ldi ZL, low(TaskQueue)
		ldi ZH, high(TaskQueue)

		ld OSRG, Z		; For Event
		cpi OSRG, $FF	; No Event or Addr out of Range
		breq PTQL02		; No Action
	
		clr ZH
		lsl OSRG
		mov ZL, OSRG

		subi ZL, low(-TaskProcs*2)
		sbci ZH, high(-TaskProcs*2) ; Add
	
		lpm					; mov r0 <- CODE[Z]
		mov OSRG, r0
		ld r0, Z+			; inc Z
		lpm	
		mov ZL, OSRG		; Get Addr
		mov ZH, r0
	
		push ZL
		push ZH

; Advance Queue
		ldi Counter, TaskQueueSize-1
		ldi ZL, low(TaskQueue)
		ldi ZH, high(TaskQueue)
	
		cli
	
PTQL01:	ldd OSRG, Z+1 		;	Shift Queues
		st Z+, OSRG		;
;		cpi OSRG, $FF		;
;		breq PTQL02		; For Long Queues
		dec Counter		;
		brne PTQL01		; Loop
		ldi OSRG, $FF		;
		st Z+, OSRG		;
	
		sei

		pop ZH
		pop ZL

		ijmp 			; Minimize Stack Usage
	
PTQL02:	ret	


;-------------------------------------------------------------------------
; OSRG - Event
SendTask:
		push ZL
		push ZH
		push Tmp2
		push Counter

		ldi ZL, low(TaskQueue)
		ldi ZH, high(TaskQueue)

		ldi Counter, TaskQueueSize

SEQL01: ld Tmp2, Z+

		cpi Tmp2, $FF
		breq SEQL02

		dec Counter		;
		breq SEQL03		; Loop
		rjmp SEQL01

SEQL02: st -Z, OSRG		; Store EVENT



SEQL03:					; EXIT
		pop Counter
		pop Tmp2
		pop ZH
		pop ZL
		ret	
;------------------------------------------------------------------------	
; OSRG - Timer Event
; X - Counter
SetTimer:
		push ZL
		push ZH
		push Tmp2
		push Counter

		ldi ZL, low(TimersPool)
		ldi ZH, high(TimersPool)

		ldi Counter, TimersPoolSize
	
STL01: ld Tmp2, Z		; Value / Counter
		cp Tmp2, OSRG		; Search for Event
		breq STL02
	
		subi ZL, Low(-3)	; Skip Counter
		sbci ZH, High(-3)	; Z+=2

		dec Counter		;
		breq STL03		; Loop
		rjmp STL01
	
STL02:	;cli
		std Z+1, XL		; Critical Section
		std Z+2, XH		; Update Counter
		;sei			; leave Critical Section
		rjmp	STL06		; Exit
STL03:

		ldi ZL, low(TimersPool)
		ldi ZH, high(TimersPool)

		ldi Counter, TimersPoolSize
	
STL04:	ld Tmp2, Z		; Value / Counter
		cpi Tmp2, $FF		; Search for Empty Timer
		breq STL05
	
		subi ZL, Low(-3)	; Skip Counter
		sbci ZH, High(-3)	; Z+=2

		dec Counter		;
		breq STL06		; No Empty Timer
		rjmp STL04
	
STL05:	cli
		st Z, OSRG		; Set Event 
		std Z+1, XL
		std Z+2, XH
		sei

STL06:
		pop Counter
		pop Tmp2
		pop ZH
		pop ZL
		ret	


RAND:	
;		ldi OSRG, 17		; MUL RND,17
;		clr Tmp2
;MULLoop:add Tmp2, RND
;		dec OSRG
;		brne MULLoop		; 4

		mov	Tmp2, RND	; x1
		lsl	Tmp2		; x2
		lsl	Tmp2		; x4
		lsl	Tmp2		; x8
		lsl	Tmp2		; x16
		add	Tmp2, RND	; x(16+1) = 0b00010001

	
		subi Tmp2, -53	; -(-53) = +53
		mov RND,Tmp2	; RND = (RNDi * 17 + 53) {MOD 256}
		ret

;===========================================================================
;RAM 	Lib
;===========================================================================

SaveRAM:	PUSH	ZH
			PUSH	ZL
			
			MOV		ZH,OSRG
			MOV		ZL,OSRG

			POP		ZL


