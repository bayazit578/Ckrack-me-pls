.286
model tiny

.code
org 100h

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
            cmp  ah, 03fh
            jne  @@old

            sub  dx, 6
        @@old:
            jmp  cs:[OldHandler21h]
handler21h  endp

EOP:

OldHandler21h   dd 0
end     Main