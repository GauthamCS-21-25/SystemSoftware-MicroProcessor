.model small
.data
.code
mov ax,@data
mov ds,ax

mov cx,07h
mov bl,00h
mov bh,01h
loop1:
mov dl,bl
push dx
cmp dl,0ah
jge alpha
add dl,30h
jmp display
alpha:
add dl,37h
display:
mov ah, 02h
int 21h
pop dx
add bl,bh
mov bh,dl
loop loop1

mov ah, 4ch
int 21h
end
