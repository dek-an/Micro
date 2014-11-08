;-----------------------------------------------
; HEX TO ASCII
;-----------------------------------------------
;I think, this was the smallest (only 10 words). 

;input: R16 = 8 bit value 0 ... 255 
;output: R18, R17, R16 = digits 
;bytes: 20 
                 
Hex2Ascii:	LDI r18,-1+'0' 
_bcd1:		INC r18
			SUBI r16,100 
			BRCC _bcd1 
			LDI r17,10+'0' 
_bcd2:		DEC r17 
			SUBI r16,-10 
			BRCS _bcd2 
			SBCI r16,-'0' 
			RET
