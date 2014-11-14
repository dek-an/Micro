;===================================================================
;"bin16BCD5"- преобразование 16-битного двоичного
;значения в упакованный BCD формат
;===================================================================
;Количество слов кода            :25 + возврат
;Количество циклов               :25/176 (Мин/Макс) + возврат
;Использованные младшие регистры :нет
;Использованные старшие регистры :4(fbinL,fbinH/tBCD0,tBCD1,tBCD2)
;Использованные указатели        :нет


;Вход
.def    fbinL   =r16            ;двоичное значение, младший байт
.def    fbinH   =r17            ;двоичное значение, старший байт

;Выход
.def    tBCD0   =r17            ;BCD значение, цифры 1 и 0
.def    tBCD1   =r18            ;BCD значение, цифры 3 и 2
.def    tBCD2   =r19            ;BCD значение, цифра 4
;Примечание: Переменные fbinH и tBCD0 должны размещаться в одном
;регистре.


bin16BCD5:	ldi     tBCD2,-1
b16BCD5_l1: inc     tBCD2

        	subi    fbinL,low(10000)
        	sbci    fbinH,high(10000)

        	brsh    b16BCD5_l1

        	subi    fbinL,low(-10000)
        	sbci    fbinH,high(-10000)

        	ldi     tBCD1,-0x11

b16BCD5_l2:	subi    tBCD1,-0x10
        	subi    fbinL,low(1000)
	       	sbci    fbinH,high(1000)
    
	    	brsh 	b16BCD5_l2
    
	    	subi    fbinL,low(-1000)
        	sbci    fbinH,high(-1000)

b16BCD5_l3:	inc     tBCD1
        	subi    fbinL,low(100)
        	sbci    fbinH,high(100)
    
	    	brsh 	b16BCD5_l3
    
	    	subi    fbinL,-100
        	ldi     tBCD0,-0x10

b16BCD5_l4:	subi    tBCD0,-0x10
        	subi    fbinL,10
    
	    	brsh 	b16BCD5_l4
	    	subi    fbinL,-10
        	add     tBCD0,fbinL
        	ret 
