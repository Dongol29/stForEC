### --------------------------------------------------------------------
### mydc.s
###
### Desk Calculator (dc)
### --------------------------------------------------------------------

	.equ   ARRAYSIZE, 20
	.equ   EOF, -1
	.equ   RAND_MAX, 1024
	## Formal parameter offsets:
	.equ   IBASE, 8
	.equ   IEXP, 12
	## Local variable offsets:
	.equ   IPINDEX, -8

	
.section ".rodata"

scanfFormat:
	.asciz "%s"
sEmpty:
	.asciz "dc: stack empty\n"
sEmpty1:
	.asciz "dc: stack empty1\n"
Pprint:
	.asciz "%d\n"
### --------------------------------------------------------------------

        .section ".data"

### --------------------------------------------------------------------

        .section ".bss"
buffer:
        .skip  ARRAYSIZE
iIndex:
		.skip  4
### --------------------------------------------------------------------

	.section ".text"

	## -------------------------------------------------------------
	## int main(void)
	## Runs desk calculator program.  Returns 0.
	## -------------------------------------------------------------

	.globl  main
	.type   main,@function

main:

	pushl   %ebp
	movl    %esp, %ebp
input:

	## dc number stack initialized. %esp = %ebp
	
	## scanf("%s", buffer)
	pushl	$buffer
	pushl	$scanfFormat
	call    scanf
	addl    $8, %esp

	## check if user input EOF
	cmp  $EOF, %eax
	je	quit
	## if(isdigit(buffer[0])!=0) goto else_digit
	movl	$buffer, %eax
	movl 	(%eax), %eax
	movb 	%al, %eax
	pushl	%eax
	call	isdigit
	addl	$4, %esp
	cmpl	$0, %eax
	jne  	else_digit
	## if(isdigit(buffer[0])!='_') goto elseif_p
	movl	$buffer, %eax
	cmpb	$'_', (%eax)
	jne 	elseif_p
	## int iIndex=0
	movl	$0, iIndex
loop:
	## if(buffer[iIndex+1]==0) goto endloop
	movl	iIndex, %eax
	addl	$1, %eax
	addl	$buffer, %eax
	movl 	(%eax), %eax
	cmpl 	$0, %eax
	je 		endloop
	## buffer[iIndex]=buffer[iIndex+1]
	movl	iIndex, %eax
	addl	$buffer, %eax
	movl 	1(%eax), %ebx
	movl 	%ebx, (%eax)	
	## iIndex++
	incl	iIndex
	jmp 	loop
endloop:
	pushl	$buffer
	call	atoi
	## stack.push(no)
	addl	$4, %esp
	movl	%eax, %edx
	subl 	%edx, %eax
	subl  	%edx, %eax
	pushl 	%eax
	jmp input 
elseif_p:
	## if(buffer[0]!='p') goto elseif_q
	movl	$buffer, %eax
	cmpb	$'p', (%eax)
	jne 	elseif_q
	## if(stack.peek()!=NULL) goto else_p
	movl	(%esp), %eax
	cmpl	$0, %eax   
	jne 	else_p
	## printf("dc: stack empty\n")
	pushl	$sEmpty1
	call	printf
	addl 	$4, %esp
	jmp 	input
else_p:
	##printf("%d\n", (int)stack.top())
	pushl	(%esp)
	## movb 	$0, %al
	## pushl	%eax
	pushl	$Pprint
	call 	printf
	addl	$8, %esp
	jmp		input
elseif_q:
	## if(buffer[0]!='q') goto elseif_plus
	movl	$buffer, %eax
	cmpb	$'q', (%eax)
	jne		elseif_plus
	jmp 	quit
elseif_plus:
	## if(buffer[0]!='+') goto elseif_minus
	movl	$buffer, %eax
	cmpb	$'+', (%eax)
	jne		elseif_minus
	## int a,b
	##subl 	$8, %esp  
	## if(stack.peek()!=NULL) goto endif1
	movl	(%esp), %eax
	cmpl	$0, %eax     
	jne 	endif1
	## printf("dc: stack empty\n")
	pushl	$sEmpty
	call	printf
	addl 	$4, %esp
	jmp 	input
endif1:
	## a=(int)stack.pop()  
	popl	%eax
	## if(stack.peek()!=NULL) goto endif2
	movl	(%esp), %edx
	cmpl	$0, %edx  
	jne 	endif2
	## printf("dc: stack empty\n")
	pushl	$sEmpty
	call	printf
	addl 	$4, %esp
	## stack.push(a)
	pushl	%eax
	jmp		input
endif2:
	## b=(int)stack.pop()
	popl	%ebx
	## res=a+b
	movl	%ebx, %edx
	addl 	%eax, %edx
	## stack.push(res)
	pushl 	%edx
	jmp 	input
elseif_minus:
	## if(buffer[0]!='-') goto elseif_power
	movl	$buffer, %eax
	cmpb	$'-', (%eax)
	jne		elseif_power
	## int a,b
	##subl 	$8, %esp  
	## if(stack.peek()!=NULL) goto endif3
	movl	(%esp), %eax
	cmpl	$0, %eax 	
	jne 	endif3
	## printf("dc: stack empty\n")
	pushl	$sEmpty
	call	printf
	addl 	$4, %esp
	jmp 	input
endif3:
	## a=(int)stack.pop()   
	popl	%eax
	## if(stack.peek()!=NULL) goto endif4
	movl	(%esp), %edx
	cmpl	$0, %edx 	
	jne 	endif4
	## printf("dc: stack empty\n")
	pushl	$sEmpty
	call	printf
	addl 	$4, %esp
	## stack.push(a)
	pushl	%eax
	jmp		input
endif4:
	## b=(int)stack.pop()
	popl	%ebx
	## res=a-b
	movl	%ebx, %edx
	subl 	%eax, %edx
	## stack.push(res)
	pushl 	%edx
	jmp 	input
elseif_power:
	## if(buffer[0]!='^') goto elseif_mul
	movl	$buffer, %eax
	cmpb	$'^', (%eax)
	jne		elseif_mul
	## int a,b
	##subl 	$8, %esp  
	## if(stack.peek()!=NULL) goto endif5
	movl	(%esp), %eax
	cmpl	$0, %eax 	
	jne 	endif5
	## printf("dc: stack empty\n")
	pushl	$sEmpty
	call	printf
	addl 	$4, %esp
	jmp 	input
endif5:
	## a=(int)stack.pop()  	
	popl	%eax
	## if(stack.peek()!=NULL) goto endif6
	movl	(%esp), %edx
	cmpl	$0, %edx 	
	jne 	endif6
	## printf("dc: stack empty\n")
	pushl	$sEmpty
	call	printf
	addl 	$4, %esp
	## stack.push(a)
	pushl	%eax
	jmp		input
endif6:
	## b=(int)stack.pop()
	popl	%ebx
	## res=power(a,b)
	pushl	%eax
	pushl	%ebx
	call 	power
	addl	$8, %esp
	## stack.push(res)
	movl	%eax, %edx
	pushl 	%edx
	jmp 	input
elseif_mul:
	## if(buffer[0]!='*') goto elseif_quo
	movl	$buffer, %eax
	cmpb	$'*', (%eax)
	jne		elseif_quo
	## int a,b
	## subl 	$8, %esp  
	## if(stack.peek()!=NULL) goto endif7
	movl	(%esp), %eax
	cmpl	$0, %eax 		
	jne 	endif7
	## printf("dc: stack empty\n")
	pushl	$sEmpty
	call	printf
	addl 	$4, %esp
	jmp 	input
endif7:
	## a=(int)stack.pop()  	
	popl	%eax
	## if(stack.peek()!=NULL) goto endif8
	movl	(%esp), %edx
	cmpl	$0, %edx 	
	jne 	endif8
	## printf("dc: stack empty\n")
	pushl	$sEmpty
	call	printf
	addl 	$4, %esp
	## stack.push(a)
	pushl	%eax
	jmp		input
endif8:
	## b=(int)stack.pop()
	popl	%ebx
	## res=a*b
	imull	%ebx
	## stack.push(res)
	movl	%eax, %edx
	pushl 	%edx
	jmp 	input
elseif_quo:
	## if(buffer[0]!='/') goto elseif_rem
	movl	$buffer, %eax
	cmpb	$'/', (%eax)
	jne		elseif_rem
	## int a,b
	## subl 	$8, %esp  
	## if(stack.peek()!=NULL) goto endif9
	movl	(%esp), %eax
	cmpl	$0, %eax 	
	jne 	endif9
	## printf("dc: stack empty\n")
	pushl	$sEmpty
	call	printf
	addl 	$4, %esp
	jmp 	input
endif9:
	## a=(int)stack.pop()  
	popl	%eax
	## if(stack.peek()!=NULL) goto endif10
	movl	(%esp), %edx
	cmpl	$0, %edx 	 
	jne 	endif10
	## printf("dc: stack empty\n")
	pushl	$sEmpty
	call	printf
	addl 	$4, %esp
	## stack.push(a)
	pushl	%eax
	jmp		input
endif10:
	## b=(int)stack.pop()
	popl	%ebx
	## res=a/b
	idivl	%ebx
	## stack.push(res)
	movl	%eax, %edx
	pushl 	%edx
	jmp 	input
elseif_rem:
	## if(buffer[0]!='%') goto elseif_f
	movl	$buffer, %eax
	cmpb	$'%', (%eax)
	jne		elseif_f
	## int a,b
	## subl 	$8, %esp  
	## if(stack.peek()!=NULL) goto endif11
	movl	(%esp), %eax
	cmpl	$0, %eax 	
	jne 	endif11
	## printf("dc: stack empty\n")
	pushl	$sEmpty
	call	printf
	addl 	$4, %esp
	jmp 	input
endif11:
	## a=(int)stack.pop()  	
	popl	%eax
	## if(stack.peek()!=NULL) goto endif12
	movl	(%esp), %edx
	cmpl	$0, %edx 		
	jne 	endif12
	## printf("dc: stack empty\n")
	pushl	$sEmpty
	call	printf
	addl 	$4, %esp
	## stack.push(a)
	pushl	%eax
	jmp		input
endif12:
	## b=(int)stack.pop()
	popl	%ebx
	## res=a%b
	idivl	%ebx
	## stack.push(res)
	pushl 	%edx
	jmp 	input
elseif_f:
	## if(buffer[0]!='f') goto elseif_c
	movl	$buffer, %eax
	cmpb	$'f', (%eax)
	jne		elseif_c
	## int i=%esp-%ebp
	movl 	%esp, %eax
	subl 	%ebp, %eax
loop_f:
	## if(i<=0) goto endloop_f
	cmpl 	$0, %eax
	jle 	endloop_f
	## printf("%d\n",(%ebp,%eax))
	movl 	$0, %edx
	addl 	%ebp, %edx
	addl 	%eax, %edx
	pushl	(%edx)
	call	printf
	addl 	$4, %esp
	jmp 	loop_f
endloop_f:
	jmp 	input
elseif_c:
	## if(buffer[0]!='c') goto elseif_d
	movl	$buffer, %eax
	cmpb	$'c', (%eax)
	jne		elseif_d
	movl 	%ebp, %esp
	jmp 	input
elseif_d:
	## if(buffer[0]!='d') goto elseif_r
	movl	$buffer, %eax
	cmpb	$'d', (%eax)
	jne		elseif_r
	## if(stack.peek()!=NULL) goto else_d
	movl	(%esp), %eax
	cmpl	$0, %eax 		
	jne 	else_d
	## printf("dc: stack empty\n")
	pushl	$sEmpty
	call	printf
	addl 	$4, %esp
	jmp		input
else_d:
	movl 	(%esp), %eax
	pushl 	%eax
	jmp 	input
elseif_r:
	## if(buffer[0]!='r') goto elseif_x
	movl	$buffer, %eax
	cmpb	$'r', (%eax)
	jne		elseif_x
	## if(stack.peek()!=NULL) goto endif13
	movl	(%esp), %eax
	cmpl	$0, %eax 		
	jne 	endif13
	## printf("dc: stack empty\n")
	pushl	$sEmpty
	call	printf
	addl 	$4, %esp
	jmp		input
endif13:
	## a=(int)stack.pop()
	popl	%eax
	## if(stack.peek()!=NULL) goto endif14
	movl	(%esp), %edx
	cmpl	$0, %edx  
	jne 	endif14
	## printf("dc: stack empty\n")
	pushl	$sEmpty
	call	printf
	addl 	$4, %esp
	## stack.push(a)
	pushl	%eax
	jmp		input
endif14:
	## b=(int)stack.pop()
	popl 	%ebx
	## stack.push(b)
	pushl 	%eax
	## stack.push(a)
	pushl 	%ebx
	jmp 	input
elseif_x:
	## if(buffer[0]!='x') goto elseif_y
	movl	$buffer, %eax
	cmpb	$'x', (%eax)
	jne		elseif_y
	## srand(time(NULL))
	pushl 	$0
	call	time
	addl	$4, %esp
	pushl 	%eax
	call 	srand
	addl 	$4, %esp
	call 	rand
	movl 	$RAND_MAX, %ebx
	idivl 	%ebx
	pushl 	%edx
	jmp 	input
elseif_y:
	## if(buffer[0]!='y') goto input
	movl	$buffer, %eax
	cmpb	$'y', (%eax)
	jne		input

else_digit:
	## int no=atoi(buffer)
	pushl	$buffer
	call	atoi
	addl 	$4, %esp
	## stack.push(no)
	pushl 	%eax
	jmp 	input
quit:
	movl	$0, %eax
	movl 	%ebp, %esp
	popl 	%ebp
	ret
power:
	pushl	%ebp
	movl 	%esp, %ebp
	## int iPower=1, int ipIndex=1
	pushl	$1
	pushl 	$1
ploop:
	## if (ipIndex>iExp) goto ploopend
	movl	IPINDEX(%ebp), %eax
	cmpl	IEXP(%ebp), %eax
	jg		ploopend
	## iPower*=IBASE
	movl 	-4(%ebp), %eax
	imull 	IBASE(%ebp), %eax
	movl 	%eax, -4(%ebp)
	## ipIndex++
	incl 	IPINDEX(%ebp)
	jmp 	ploop
ploopend:
	## return value(=iPower) to eax
	movl 	-4(%ebp), %eax
	movl 	%ebp, %esp
	popl 	%ebp
	ret

