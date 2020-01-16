section .data
	mensaje		db "hola mundo", 0xA
	longitud	equ $ - mensaje

sectio .text
	globa _inicio
_inicio:
	mov edx, longitud
	mov ecx, mensaje
	mov ebx, 1
	mov eax, 4
	int 0x80

	mov ebx, 0
	mov eax, 1
	int 0x80
