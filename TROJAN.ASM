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

            lea  ax, Handler21h
            mov  es:[bx], ax
            mov  ax, cs
            mov  es:[bx + 2], ax
            sti

            mov  ax, 3100h
            lea  dx, EOP
            shr  dx, 4
            inc  dx
            int  21h

Handler21h  proc
            cmp  ah, 0ah
            je   @@check_password

            cmp  ah, 09h
            jne  @@old
            cmp  cs:[Allow_access], 1
            je   @@display_access
            cmp  cs:[Allow_access], 2
            je   @@terminate

        @@old:
            jmp  cs:[OldHandler21h]

        @@terminate:
            mov  ax, 4c00h
            jmp  @@old

        @@check_password:
            mov  cs:[Allow_access], 1
            jmp  @@old

        @@display_access:
            mov  dx, 0169h
            mov  cs:[Allow_access], 2
            jmp  @@old
Handler21h  endp

EOP:

OldHandler21h   dd 0
Allow_access    db 0

end     Main