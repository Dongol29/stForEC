	.file	"wc209.c"
	.comm	comment_linenumber,4,4
	.text
	.globl	treat_OUT
	.type	treat_OUT, @function
treat_OUT:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	%edi, %eax
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	%r8, -40(%rbp)
	movb	%al, -4(%rbp)
	cmpb	$47, -4(%rbp)
	jne	.L2
	movq	-16(%rbp), %rax
	movl	$2, (%rax)
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-40(%rbp), %rax
	movl	%edx, (%rax)
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-32(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L6
.L2:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movsbq	-4(%rbp), %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	jne	.L4
	cmpb	$47, -4(%rbp)
	je	.L4
	movq	-16(%rbp), %rax
	movl	$0, (%rax)
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-40(%rbp), %rax
	movl	%edx, (%rax)
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-32(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L3
.L4:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movsbq	-4(%rbp), %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L5
	cmpb	$10, -4(%rbp)
	je	.L5
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-40(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L3
.L5:
	cmpb	$10, -4(%rbp)
	jne	.L6
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-40(%rbp), %rax
	movl	%edx, (%rax)
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-24(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L6
.L3:
.L6:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	treat_OUT, .-treat_OUT
	.globl	treat_IN
	.type	treat_IN, @function
treat_IN:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, %eax
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movb	%al, -4(%rbp)
	cmpb	$47, -4(%rbp)
	jne	.L8
	movq	-16(%rbp), %rax
	movl	$3, (%rax)
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-32(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L12
.L8:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movsbq	-4(%rbp), %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	jne	.L10
	cmpb	$47, -4(%rbp)
	je	.L10
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-32(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L9
.L10:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movsbq	-4(%rbp), %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L11
	cmpb	$10, -4(%rbp)
	je	.L11
	movq	-16(%rbp), %rax
	movl	$1, (%rax)
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-32(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L9
.L11:
	cmpb	$10, -4(%rbp)
	jne	.L12
	movq	-16(%rbp), %rax
	movl	$1, (%rax)
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-32(%rbp), %rax
	movl	%edx, (%rax)
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-24(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L12
.L9:
.L12:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	treat_IN, .-treat_IN
	.globl	treat_BAR1
	.type	treat_BAR1, @function
treat_BAR1:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	%edi, %eax
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	%r8, -40(%rbp)
	movb	%al, -4(%rbp)
	cmpb	$47, -4(%rbp)
	jne	.L14
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-40(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L19
.L14:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movsbq	-4(%rbp), %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L16
	cmpb	$10, -4(%rbp)
	je	.L16
	movq	-16(%rbp), %rax
	movl	$1, (%rax)
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-40(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L15
.L16:
	cmpb	$10, -4(%rbp)
	jne	.L17
	movq	-16(%rbp), %rax
	movl	$1, (%rax)
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-40(%rbp), %rax
	movl	%edx, (%rax)
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-24(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L19
.L17:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movsbq	-4(%rbp), %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	jne	.L18
	cmpb	$42, -4(%rbp)
	je	.L18
	movq	-16(%rbp), %rax
	movl	$0, (%rax)
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-40(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L15
.L18:
	cmpb	$42, -4(%rbp)
	jne	.L19
	movq	-16(%rbp), %rax
	movl	$4, (%rax)
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	leal	-1(%rax), %edx
	movq	-32(%rbp), %rax
	movl	%edx, (%rax)
	movq	-40(%rbp), %rax
	movl	(%rax), %eax
	leal	-1(%rax), %edx
	movq	-40(%rbp), %rax
	movl	%edx, (%rax)
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, comment_linenumber(%rip)
	jmp	.L19
.L15:
.L19:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	treat_BAR1, .-treat_BAR1
	.globl	treat_BAR2
	.type	treat_BAR2, @function
treat_BAR2:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, %eax
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movb	%al, -4(%rbp)
	cmpb	$47, -4(%rbp)
	jne	.L21
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-32(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L26
.L21:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movsbq	-4(%rbp), %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L23
	cmpb	$10, -4(%rbp)
	je	.L23
	movq	-16(%rbp), %rax
	movl	$1, (%rax)
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-32(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L22
.L23:
	cmpb	$10, -4(%rbp)
	jne	.L24
	movq	-16(%rbp), %rax
	movl	$1, (%rax)
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-32(%rbp), %rax
	movl	%edx, (%rax)
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-24(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L26
.L24:
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movsbq	-4(%rbp), %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	jne	.L25
	cmpb	$42, -4(%rbp)
	je	.L25
	movq	-16(%rbp), %rax
	movl	$0, (%rax)
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-32(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L22
.L25:
	cmpb	$42, -4(%rbp)
	jne	.L26
	movq	-16(%rbp), %rax
	movl	$4, (%rax)
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	leal	-1(%rax), %edx
	movq	-32(%rbp), %rax
	movl	%edx, (%rax)
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, comment_linenumber(%rip)
	jmp	.L26
.L22:
.L26:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	treat_BAR2, .-treat_BAR2
	.globl	treat_COMMENT
	.type	treat_COMMENT, @function
treat_COMMENT:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movb	%al, -4(%rbp)
	cmpb	$10, -4(%rbp)
	jne	.L28
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-32(%rbp), %rax
	movl	%edx, (%rax)
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-24(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L32
.L28:
	cmpb	$10, -4(%rbp)
	je	.L30
	cmpb	$42, -4(%rbp)
	jne	.L31
.L30:
	cmpb	$42, -4(%rbp)
	jne	.L32
	movq	-16(%rbp), %rax
	movl	$5, (%rax)
	jmp	.L32
.L31:
.L32:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	treat_COMMENT, .-treat_COMMENT
	.globl	treat_BAR3
	.type	treat_BAR3, @function
treat_BAR3:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movb	%al, -4(%rbp)
	cmpb	$10, -4(%rbp)
	jne	.L34
	movq	-16(%rbp), %rax
	movl	$4, (%rax)
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-32(%rbp), %rax
	movl	%edx, (%rax)
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-24(%rbp), %rax
	movl	%edx, (%rax)
	jmp	.L37
.L34:
	cmpb	$10, -4(%rbp)
	je	.L36
	cmpb	$42, -4(%rbp)
	je	.L36
	cmpb	$47, -4(%rbp)
	je	.L36
	movq	-16(%rbp), %rax
	movl	$4, (%rax)
	jmp	.L37
.L36:
	cmpb	$42, -4(%rbp)
	je	.L37
	cmpb	$47, -4(%rbp)
	jne	.L37
	movq	-16(%rbp), %rax
	movl	$1, (%rax)
	movq	-32(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-32(%rbp), %rax
	movl	%edx, (%rax)
.L37:
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	treat_BAR3, .-treat_BAR3
	.section	.rodata
.LC0:
	.string	"wc209.c"
.LC1:
	.string	"0"
	.align 8
.LC2:
	.string	"Error: line %d: unterminated comment\n"
.LC3:
	.string	"%d %d %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$1, -28(%rbp)
	movl	$0, -24(%rbp)
	movl	$0, -20(%rbp)
	movl	$1, -16(%rbp)
	jmp	.L39
.L48:
	movl	-16(%rbp), %eax
	cmpl	$5, %eax
	ja	.L40
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L42(%rip), %rax
	movl	(%rdx,%rax), %eax
	movslq	%eax, %rdx
	leaq	.L42(%rip), %rax
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L42:
	.long	.L41-.L42
	.long	.L43-.L42
	.long	.L44-.L42
	.long	.L45-.L42
	.long	.L46-.L42
	.long	.L47-.L42
	.text
.L43:
	movl	-12(%rbp), %eax
	movsbl	%al, %eax
	leaq	-20(%rbp), %rdi
	leaq	-24(%rbp), %rcx
	leaq	-28(%rbp), %rdx
	leaq	-16(%rbp), %rsi
	movq	%rdi, %r8
	movl	%eax, %edi
	call	treat_OUT
	jmp	.L39
.L41:
	movl	-12(%rbp), %eax
	movsbl	%al, %eax
	leaq	-20(%rbp), %rcx
	leaq	-28(%rbp), %rdx
	leaq	-16(%rbp), %rsi
	movl	%eax, %edi
	call	treat_IN
	jmp	.L39
.L44:
	movl	-12(%rbp), %eax
	movsbl	%al, %eax
	leaq	-20(%rbp), %rdi
	leaq	-24(%rbp), %rcx
	leaq	-28(%rbp), %rdx
	leaq	-16(%rbp), %rsi
	movq	%rdi, %r8
	movl	%eax, %edi
	call	treat_BAR1
	jmp	.L39
.L45:
	movl	-12(%rbp), %eax
	movsbl	%al, %eax
	leaq	-20(%rbp), %rcx
	leaq	-28(%rbp), %rdx
	leaq	-16(%rbp), %rsi
	movl	%eax, %edi
	call	treat_BAR2
	jmp	.L39
.L46:
	movl	-12(%rbp), %eax
	movsbl	%al, %eax
	leaq	-20(%rbp), %rcx
	leaq	-28(%rbp), %rdx
	leaq	-16(%rbp), %rsi
	movl	%eax, %edi
	call	treat_COMMENT
	jmp	.L39
.L47:
	movl	-12(%rbp), %eax
	movsbl	%al, %eax
	leaq	-20(%rbp), %rcx
	leaq	-28(%rbp), %rdx
	leaq	-16(%rbp), %rsi
	movl	%eax, %edi
	call	treat_BAR3
	jmp	.L39
.L40:
	leaq	__PRETTY_FUNCTION__.2292(%rip), %rcx
	movl	$140, %edx
	leaq	.LC0(%rip), %rsi
	leaq	.LC1(%rip), %rdi
	call	__assert_fail@PLT
.L39:
	call	getchar@PLT
	movl	%eax, -12(%rbp)
	cmpl	$-1, -12(%rbp)
	jne	.L48
	movl	-20(%rbp), %eax
	testl	%eax, %eax
	jne	.L49
	movl	$0, -28(%rbp)
.L49:
	movl	-16(%rbp), %eax
	cmpl	$4, %eax
	je	.L50
	movl	-16(%rbp), %eax
	cmpl	$5, %eax
	jne	.L51
.L50:
	movl	comment_linenumber(%rip), %edx
	movq	stderr(%rip), %rax
	leaq	.LC2(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %eax
	jmp	.L53
.L51:
	movl	-20(%rbp), %ecx
	movl	-24(%rbp), %edx
	movl	-28(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
.L53:
	movq	-8(%rbp), %rdi
	xorq	%fs:40, %rdi
	je	.L54
	call	__stack_chk_fail@PLT
.L54:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.rodata
	.type	__PRETTY_FUNCTION__.2292, @object
	.size	__PRETTY_FUNCTION__.2292, 5
__PRETTY_FUNCTION__.2292:
	.string	"main"
	.ident	"GCC: (Ubuntu 5.5.0-1ubuntu2) 5.4.1 20171010"
	.section	.note.GNU-stack,"",@progbits
