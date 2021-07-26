	.file	"test.c"
	.text
	.globl	toggle_green_off
	.type	toggle_green_off, @function
toggle_green_off:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movb	$0, green_on(%rip)
	movl	$0, %eax
	call	led_update
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	toggle_green_off, .-toggle_green_off
	.ident	"GCC: (SUSE Linux) 7.4.1 20190424 [gcc-7-branch revision 270538]"
	.section	.note.GNU-stack,"",@progbits
