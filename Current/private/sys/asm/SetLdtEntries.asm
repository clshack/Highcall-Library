; Hc/NtSetLdtEntries
; This file was automatically generated by Highcall's syscall generator.

IFDEF RAX
; 64bit

EXTERNDEF sciSetLdtEntries:DWORD

.DATA
.CODE

HcSetLdtEntries PROC
	mov r10, rcx
	mov eax, sciSetLdtEntries
	syscall
	ret
HcSetLdtEntries ENDP

ELSE
; 32bit

EXTERNDEF C sciSetLdtEntries:DWORD

.586			  
.MODEL FLAT, C   
.STACK
.DATA
.CODE

ASSUME FS:NOTHING

HcSetLdtEntries PROC
	mov eax, sciSetLdtEntries
	mov ecx, fs:[0c0h]
	test ecx, ecx
	jne _wow64
	lea edx, [esp + 4]
	INT 02eh
	ret
	_wow64:
	xor ecx, ecx
	lea edx, [esp + 4h]
	call dword ptr fs:[0c0h]
	ret
HcSetLdtEntries ENDP

ENDIF

END