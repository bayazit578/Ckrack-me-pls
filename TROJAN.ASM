.286
model tiny

.code
org 100h

ETALON_HASH equ 10111001b

Main:   
            mov  ax, 0
            mov  es, ax
            mov  bx, 21h * 4

            cli
            mov  ax, es:[bx]
            mov  word ptr OldHandler21h, ax
            mov  ax, es:[bx + 2]
            mov  word ptr OldHandler21h + 2, ax

            lea  ax, handler21h
            mov  es:[bx], ax
            mov  ax, cs
            mov  es:[bx + 2], ax
            sti

            mov  ax, 3100h
            lea  dx, EOP
            shr  dx, 4
            inc  dx
            int  21h

;-----------------------------------------------------------
; 21h interrupt handler
;-----------------------------------------------------------

handler21h  proc
            cmp  ah, 0ah
            je   @@hash_check

        @@old:
            jmp  cs:[OldHandler21h]

        @@hash_check: 
            push bp ax es si di
            mov  bp, sp
            mov  ax, ss:[bp + 12]
            mov  es, ax

            xor  di, di
            xor  si, si 
            xor  ax, ax
            call hash_func
            cmp  di, ETALON_HASH
            pop  di si es ax bp
            jne  @@old

            push bp
            mov  bp, sp
            mov  word ptr ss:[bp + 2], 122h
            pop  bp
            jmp  @@old
handler21h  endp

;-----------------------------------------------------------
; Descr: hash_func calculates hash for the code block that
;        starts with the end of PSP
;
; Entry: ES:BX -> start of the code block
;
; Assum: ES is set to the right code segment
;
; Exit:  DI - calculated hash
;
; Destr: AL, BX, CX, SI
;-----------------------------------------------------------

hash_func   proc
            mov  bx, 100h
            mov  cx, 64h

            mov  al, es:[bx]
            inc  bx

        @@hash_loop:
            xor  di, ax
            mov  al, es:[bx]
            inc  bx
            loop @@hash_loop
            ret
hash_func   endp

EOP:

OldHandler21h   dd 0
end     Main