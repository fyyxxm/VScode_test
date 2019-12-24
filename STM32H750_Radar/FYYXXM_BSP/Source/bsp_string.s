    AREA MY_STING, CODE, READONLY, ALIGN=2
	PRESERVE8
	THUMB

;R3可以自动出入栈，不用手动控制R3出入栈。

my_strcpy PROC	
	EXPORT my_strcpy		
loop_strcpy
	LDRB R3, [R1], #1	;R1地址值赋给R3
	CMP R3, #0			;判断R3是否为0
	BEQ over_strcpy		;为0，跳出循环
	STRB R3, [R0], #1	;R3里的值（R1的地址值）,赋给到R0的地址
	B loop_strcpy
	ENDP
over_strcpy
	B over
	
my_strncpy PROC
	EXPORT my_strncpy
loop_strncpy
	CMP R2, #0			;判断R2是否为0	
	BEQ over_strncpy	;为0就跳出
	LDRB R3, [R1], #1	;R1的地址值给R3,然后+1
	STRB R3, [R0], #1	;R3里的的地址值给R0,然后+1
	SUB R2, #1			;R2--
	B loop_strncpy
	ENDP
over_strncpy
	B over

;bug:连续使用时，可能会卡死,原因待查
my_memset PROC
	EXPORT my_memset	
	push {R4}		
	CMP R0, #0			;R0为0直接跳出
	BEQ over_memset		;为0就跳出	
		
	MOV R4, R2			;R2的值给R4
	AND R4, #0x80000000	;R4只取最高位
	CMP R4, #0x80000000 ;和0x80000000比较，判断是否为负值
	BEQ over_memset 	;是负值跳出	
	MOV R3, R1			;R1的值给R3
loop_memset
	CMP R2, #0			;判断R2是否为0	
	BEQ over_memset		;为0就跳出
	STRB R3, [R0], #1	;R3里的的地址值给R0,然后+1
	SUB R2, #1			;R2--
	B loop_memset
	ENDP					
over_memset
	pop{R4}
	B over
	
over
	END





;my_strcpy PROC	
;	EXPORT my_strcpy
;	push {R4}		
;loop_strcpy
;	LDRB R4, [R1], #1	;R1地址值赋给R3
;	CMP R4, #0			;判断R3是否为0
;	BEQ over			;为0，跳出循环
;	STRB R4, [R0], #1	;R3里的值（R1的地址值）,赋给到R0的地址
;	B loop_strcpy
;	ENDP
;		
;my_strncpy PROC
;	EXPORT my_strncpy
;	push {R4}
;loop_strncpy
;	CMP R2, #0			;判断R2是否为0	
;	BEQ over			;为0就跳出
;	LDRB R4, [R1], #1	;R1的地址值给R3,然后+1
;	STRB R4, [R0], #1	;R3里的的地址值给R0,然后+1
;	SUB R2, #1			;R2--
;	B loop_strncpy
;	ENDP
;		
;over
;	pop{R4}
;	END
