.386
.model	flat, stdcall
option	casemap: none

include	windows.inc
include	kernel32.inc
include	user32.inc

includelib		kernel32.lib
includelib		user32.lib

include		winmm.inc
includelib		winmm.lib

DlgProc	PROTO	:DWORD, :DWORD, :DWORD, :DWORD

.const
	IDD_Dlg		equ	101
	IDS_Music	equ	2001

.data?
	hInstance		dd	?
.code
begin:
	invoke GetModuleHandle, NULL
	mov hInstance, eax
	invoke DialogBoxParam, hInstance, IDD_Dlg, NULL, addr DlgProc, NULL
	invoke ExitProcess, eax

DlgProc	proc	hWnd: DWORD, uMsg: DWORD, wParam: DWORD, lParam: DWORD
	
	.if uMsg == WM_INITDIALOG
		invoke PlaySound, IDS_Music, hInstance, SND_LOOP\
										or SND_ASYNC\
										or SND_RESOURCE
	.elseif uMsg == WM_CLOSE
		invoke PlaySound, NULL, hInstance, NULL
		invoke EndDialog, hWnd, NULL
	.endif
	
	xor eax, eax
	ret
DlgProc	endp

end	begin