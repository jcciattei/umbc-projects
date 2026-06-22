; julia_ciattei_caesar.asm
; Julia Ciattei, PB47617, jcia1@umbc.edu, 7/10/2023
;
; Requests a shift value from the user
; Shift must be between -25 and 25, inclusive
; Re-prompts until a valid shift is entered
; Requests a message from the user
; Message must be greater than 8 characters
; Re-prompts until a valid message is entered
; Shifts all letters by the shift value
; Displays both original and shifted message
; 
; Assemble: nasm -f elf64 julia_ciattei_caesar.asm
; Link: gcc -no-pie -m64 -o caesar julia_ciattei_caesar.o
; Run: ./caesar

; initialized data, includes prompts printed to console
	section	.data
	
; prompt to enter shift value 
shift_prompt:	db	"Enter a shift value between -25 and 25 (included)", 10 
len_shift:	equ	$ - shift_prompt

; prompt to enter message
message_prompt:	db      "Enter a string greater than 8 characters", 10
len_message:	equ     $ - message_prompt

; diplaying original message
display_og:	db	"Current message: "
len_og:		equ	$ - display_og
	
; displaying shifted message
display_edit:	db	"Edited message: "
len_edit:	equ	$ - display_edit
new_line	db	10
	
; uninitialized data, includes unknown shift value and message
	section .bss
shift_buff	resb	4	; shift value can be up to 3 characters + enter
message_buff	resb	128	; message can be up to 127 characters + enter
edit_msg_buff	resb	128	; edited message can be up to 127 characters + enter
	
; instructions, including syscalls to read/write to console
	section .text
	global main

main:

; user validation loop, get shift value between -25 and 25, inclusive
while_get_shift:
        ; write shift_prompt to console
        mov     rax, 1			; write syscall stored in rax
        mov     rdi, 1			; rdi set to 1 = writing to stdout
        mov     rsi, shift_prompt       ; rsi set to address of first char
        mov     rdx, len_shift          ; write all chars in string
        syscall

        ; read user input to shift_buff (these are characters)
        xor     rax, rax		; write syscall 0 stored in rax
        xor     rdi, rdi		; rdi set to 0 = reading stdin
        mov     rsi, shift_buff		; rsi set to address of reserved space
        mov     rdx, 4			; read in up to 4 characters
        syscall

; convert shift_buff to an integer
convert_to_int:	
	mov     cl, [rsi]		; set 8-bits of value stored in rsi to CL	
        cmp     cl, '-'			; check for - char
        jne     is_number		; if no - char, proceed to is_number
        inc     rsi			; if it is a negative sign, move to next 8 bits
        jmp     convert_to_int		; loop back to convert_to_int

is_number:
        xor     rcx, rcx

read_in_chars:
        mov     cl, [rsi]		; set 8 bits of value stored in rsi to CL
        cmp     cl, 10			; check for new line character
        je      done			; if at new line, proceed to done
        imul    rdi, 10			; multiply output integer by 10 
        sub     cl, '0'			; subtract the ASCII for 0 to get the int value
        add     rdi, rcx		; add the integer result
        inc     rsi			; increment to the next 8 bits
        jmp     read_in_chars		; until new line, read in chars

done:
        cmp     byte [shift_buff], '-'  ; check for negative sign  
        jne     compare_range		; if not negative, procced to compare_range
        neg     rdi			; if negative, get two's complement

compare_range:
	mov	rbx, rdi		; move shift value to rbx for compare
	cmp	rbx, -25		; compare with -25
	js	while_get_shift		; if result is positive, loop back
	cmp 	rbx, 26			; compare with 26
	jns	while_get_shift		; if result not positive, loop back
	
; integer value for shift is stored in rbx
	
; user validation loop, get a message from user with minimum 8 characters
while_get_message:
	
	; write message_prompt to console
        mov     rax, 1
        mov     rdi, 1
        mov     rsi, message_prompt     
        mov     rdx, len_message        
        syscall

        ; read user input to message_buff
        xor     rax, rax
        xor     rdi, rdi
        mov     rsi, message_buff
        mov     rdx, 128
        syscall

compare_length:	
	cmp	rax, 10			; rax holds num chars input
	js	while_get_message	; if num chars less than 9 + new line, loop back
	

; message_buff holds address to first char of message that user entered
; rsi holds message_buff
	
; loop over message and caesar shift
	mov	rdi, edit_msg_buff
	
check_chars:
	xor 	rcx, rcx
	mov	cl, [rsi]		; move char from message_buff to CL
	cmp	cl, 10			; check for new line
	je	display			; when new line is reached, display
	cmp	cl, 65			; compare with 65
	js	store_character		; if < 65, store a non-letter
	cmp	cl, 123			; compare with 123
	jns	store_character		; if > 122, store a non-letter
	cmp	cl, 91			; compare with 91
	js	caesar_shift_up		; if < 91, caesar shift uppercase
	cmp	cl, 97			; compare with 96
	jns	caesar_shift_low	; if > 96, caesar shift lowercase

store_character:
	mov	[rdi], cl
	inc	rsi
	inc	rdi
	jmp	check_chars

caesar_shift_up:	
	add	rcx, rbx	       	; add shift stored in rbx to char
	cmp	rcx, 91			; compare with 91
	jns	subtract_26		; if > 90, subtract 26 from char
	cmp	rcx, 65			; compare with 65
	js	add_26			; if < 65, add 26 to char
	jmp	store_character		; otherwise, store the character
	
caesar_shift_low:
	add     rcx, rbx                ; add shift stored in rbx to char
        cmp     rcx, 123                ; compare with 123
        jns     subtract_26             ; if > 122, subtract 26 from char
        cmp     rcx, 97                 ; compare with 65
        js      add_26                  ; if < 97, add 26 to char
        jmp     store_character         ; otherwise, store the character

subtract_26:
	add	rcx, -26
	jmp	store_character

add_26:
	add	rcx, 26
	jmp	store_character
	
; display results to user and then exit
display:	
	; write original message to console
        mov     rax, 1
        mov     rdi, 1
        mov     rsi, display_og		; go to address of first char
        mov     rdx, len_og		; write all chars in string
        syscall				

	mov	rax, 1
	mov	rdi, 1
	mov	rsi, message_buff 	; go to address of first char
	mov	rdx, 128	  	; write all chars in string
	syscall
	
	; write edited message to console
	mov 	rax, 1
	mov 	rdi, 1
	mov 	rsi, display_edit	; go to address of first char
	mov 	rdx, len_edit		; write all chars in string
	syscall

	mov     rax, 1
        mov     rdi, 1
        mov     rsi, edit_msg_buff        ; go to address of first char
        mov     rdx, 128                  ; write all chars in string
        syscall

	mov	rax, 1
	mov	rdi, 1
	mov	rsi, new_line
	mov 	rdx, 1
	syscall
	
	; exit program normally, no seg faults	
	mov	rax, 60
	xor	rdi, rdi
	syscall
	

	
