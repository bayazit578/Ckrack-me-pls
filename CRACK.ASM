.286
model tiny

.code
org 100h

locals @@

mDISPLAY_TEXT   MACRO text
        mov ah, 09h
        lea dx, text
        int 21h
ENDM

mTERMINATE      MACRO
        mov ax, 4c00h
        int 21h
ENDM

mPASSWORD_CHECK MACRO
        mov ax, ds
        mov es, ax

        lea di, password
        lea si, input
        add si, 2
        mov cx, 3
        rep cmpsb
ENDM

CRLF            equ 0ah, 0dh
CR              equ 0ah

Main:       @@start:
                mDISPLAY_TEXT start_text

                mov  ah, 0ah
                lea  dx, input
                int  21h

                ; mPASSWORD_CHECK
                xor  ax, ax
                call hash_func
                cmp  ax, word ptr password_hash
                jne  @@failure
                jmp  @@success

            @@failure:
                mDISPLAY_TEXT failure_text
                jmp  @@start

            @@success:
                mDISPLAY_TEXT success_text

                mTERMINATE

;-----------------------------------------------------------
; Descr: hash_func calculates the hash for the string 
;
; Entry: DS:SI -> string with the entered password
;
; Assum: 
;
; Exit:  AX - password hash
;
; Destr: CX, SI 
;-----------------------------------------------------------

hash_func       proc
                lea  bx, input
                add  bx, 2
                mov  cx, 8

                mov  si, 0
            @@loop_hash:
                xor  al, [bx + si]
                shl  ax, 1
                inc  si
                loop @@loop_hash
                ret
hash_func       endp

start_text      db 'Enter the password:$'
failure_text    db CRLF, 'You are gay, try again', CRLF, '$'
success_text    db CRLF, 'Nice$'
input           db 18, 10 dup (0)
password_hash   dw 5728h
; penis           db 'torvalds'
checking_addr   dw 0
address         dw 0

end     Main