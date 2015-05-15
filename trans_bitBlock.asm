extern width_window
extern height_window
extern x_window
extern y_window
extern pt_memBuffer
extern pt_buffer
bytes_w equ 5504 
global trans_bitBlock
[section .text]
trans_bitBlock:
;get esi,edi(esi for memBuffer,edi for framebuffer)
	;get offset-bytes first
	mov eax,bytes_w
	mul dword [y_window];whole valid bit stored whithin eax
	mov edi,[x_window] 
	shl edi,2
	add edi,eax
	;edi store offset-bytes now
	add edi,[pt_buffer]
	;now, edi point the right [pt_buffer+offset]  of pt_buffer,just write pixel.
	;esi is always pt_memBuffer,to imitate window-style
	mov esi,[pt_memBuffer]
;esi,edi done

	;get incre-bytes
	mov eax,[width_window]  
	shl eax,2
	mov ebx,bytes_w
	sub ebx,eax
	;done
	mov ecx,[height_window]
	cld
	trans_line:
		push ecx
		mov ecx,[width_window]
		rep movsd
		add esi,ebx
		add edi,ebx
		pop ecx 
	loop trans_line
ret

	
