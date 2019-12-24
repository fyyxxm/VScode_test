    AREA MY_STING, CODE, READONLY, ALIGN=2
	PRESERVE8
	THUMB

;R3�����Զ�����ջ�������ֶ�����R3����ջ��

my_strcpy PROC	
	EXPORT my_strcpy		
loop_strcpy
	LDRB R3, [R1], #1	;R1��ֵַ����R3
	CMP R3, #0			;�ж�R3�Ƿ�Ϊ0
	BEQ over_strcpy		;Ϊ0������ѭ��
	STRB R3, [R0], #1	;R3���ֵ��R1�ĵ�ֵַ��,������R0�ĵ�ַ
	B loop_strcpy
	ENDP
over_strcpy
	B over
	
my_strncpy PROC
	EXPORT my_strncpy
loop_strncpy
	CMP R2, #0			;�ж�R2�Ƿ�Ϊ0	
	BEQ over_strncpy	;Ϊ0������
	LDRB R3, [R1], #1	;R1�ĵ�ֵַ��R3,Ȼ��+1
	STRB R3, [R0], #1	;R3��ĵĵ�ֵַ��R0,Ȼ��+1
	SUB R2, #1			;R2--
	B loop_strncpy
	ENDP
over_strncpy
	B over

;bug:����ʹ��ʱ�����ܻῨ��,ԭ�����
my_memset PROC
	EXPORT my_memset	
	push {R4}		
	CMP R0, #0			;R0Ϊ0ֱ������
	BEQ over_memset		;Ϊ0������	
		
	MOV R4, R2			;R2��ֵ��R4
	AND R4, #0x80000000	;R4ֻȡ���λ
	CMP R4, #0x80000000 ;��0x80000000�Ƚϣ��ж��Ƿ�Ϊ��ֵ
	BEQ over_memset 	;�Ǹ�ֵ����	
	MOV R3, R1			;R1��ֵ��R3
loop_memset
	CMP R2, #0			;�ж�R2�Ƿ�Ϊ0	
	BEQ over_memset		;Ϊ0������
	STRB R3, [R0], #1	;R3��ĵĵ�ֵַ��R0,Ȼ��+1
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
;	LDRB R4, [R1], #1	;R1��ֵַ����R3
;	CMP R4, #0			;�ж�R3�Ƿ�Ϊ0
;	BEQ over			;Ϊ0������ѭ��
;	STRB R4, [R0], #1	;R3���ֵ��R1�ĵ�ֵַ��,������R0�ĵ�ַ
;	B loop_strcpy
;	ENDP
;		
;my_strncpy PROC
;	EXPORT my_strncpy
;	push {R4}
;loop_strncpy
;	CMP R2, #0			;�ж�R2�Ƿ�Ϊ0	
;	BEQ over			;Ϊ0������
;	LDRB R4, [R1], #1	;R1�ĵ�ֵַ��R3,Ȼ��+1
;	STRB R4, [R0], #1	;R3��ĵĵ�ֵַ��R0,Ȼ��+1
;	SUB R2, #1			;R2--
;	B loop_strncpy
;	ENDP
;		
;over
;	pop{R4}
;	END
