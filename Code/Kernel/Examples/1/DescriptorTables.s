[GLOBAL Processor_SetGDT]

Processor_SetGDT:
	mov eax, [esp+4]
	lgdt [eax]			; Load the GDT
	
	mov ax, 0x10		; Give the CPU the new descriptor offsets
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x8:.codeFlush	; This sets the code descriptor offset to 0x8
.codeFlush:
	ret
