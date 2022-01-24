;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
NUM .equ 3499 ;Indicar número a ser convertido
;
main: 		mov #NUM,R5 ;R5 = número a ser convertido
 			mov #RESP,R6 ;R6 = ponteiro para escrever a resposta
 			call #ALG_ROM ;chamar subrotina
 			jmp $ ;travar execução
 			nop ;exigido pelo montador
;
ALG_ROM:

div1000:	cmp		#1000,r5
			jn		teste900
			sub		#1000,r5
			mov.b	#0x4d,0(r6)
			inc.w	r6
			jz		ret
			jmp		div1000

teste900:	cmp		#900,r5
			jn		div500
			sub		#900,r5
			mov.b	#0x43,0(r6)
			inc.w	r6
			mov.b	#0x4d,0(r6)
			inc.w	r6
			jz		ret

div500:		cmp		#500,r5
			jn		teste400
			sub		#500,r5
			mov.b	#0x44,0(r6)
			inc.w	r6
			jz		ret

teste400:	cmp		#400,r5
			jn		div100
			sub		#400,r5
			mov.b	#0x43,0(r6)
			inc.w	r6
			mov.b	#0x44,0(r6)
			inc.w	r6
			jz		ret

div100:		cmp		#100,r5
			jn		teste90
			sub		#100,r5
			mov.b	#0x43,0(r6)
			inc.w	r6
			jz		ret
			jmp		div100

teste90:	cmp		#90,r5
			jn		div50
			sub		#90,r5
			mov.b	#0x58,0(r6)
			inc.w	r6
			mov.b	#0x43,0(r6)
			inc.w	r6
			jz		ret

div50:		cmp		#50,r5
			jn		teste40
			sub		#50,r5
			mov.b	#0x4c,0(r6)
			inc.w	r6
			jz		ret

teste40:	cmp		#40,r5
			jn		div10
			sub		#40,r5
			mov.b	#0x58,0(r6)
			inc.w	r6
			mov.b	#0x4c,0(r6)
			inc.w	r6
			jz		ret

div10:		cmp		#10,r5
			jn		teste9
			sub		#10,r5
			mov.b	#0x58,0(r6)
			inc.w	r6
			jz		ret
			jmp		div10

teste9:		cmp		#9,r5
			jn		div5
			sub		#9,r5
			mov.b	#0x49,0(r6)
			inc.w	r6
			mov.b	#0x58,0(r6)
			inc.w	r6
			jz		ret

div5:		cmp		#5,r5
			jn		teste4
			sub		#5,r5
			mov.b	#0x56,0(r6)
			inc.w	r6
			jz		ret

teste4:		cmp		#4,r5
			jn		div1
			sub		#4,r5
			mov.b	#0x49,0(r6)
			inc.w	r6
			mov.b	#0x56,0(r6)
			inc.w	r6
			jz		ret

div1:		cmp		#1,r5
			jn		ret
			sub		#1,r5
			mov		#0x49,0(r6)
			inc.w	r6
			jz		ret
			jmp		div1


ret:		mov.b	#0,0(r6)
			ret

 .data
; Local para armazenar a resposta (RESP = 0x2400)
RESP: .byte "RRRRRRRRRRRRRRRRRR",0
                                            

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
