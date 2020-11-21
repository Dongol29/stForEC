### --------------------------------------------------------------------
### mydc.s
###
### Desk Calculator (dc)
### --------------------------------------------------------------------

	.equ   ARRAYSIZE, 20
	.equ   EOF, -1
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
	cmp	$EOF, %eax
	je	quit
	## if(isdigit(buffer[0])==0) goto else33
	movl	$buffer, %eax
	pushl	(%eax)    	
	call	isdigit
	addl	$4, %esp
	cmpl	$0, %eax
	je  	else33
	## if(isdigit(buffer[0])!='_') goto elseif5
	movl	$buffer, %eax
	cmpl	$'_', (%eax)
	jne 	elseif5
	## int iIndex=0
	movl	$0, iIndex
loop:
	## if(buffer[iIndex+1]==0) goto endloop
	movl	iIndex, %eax
	sall 	$2, %eax
	addl	$4, %eax
	addl	$buffer, %eax
	movl 	(%eax), %eax
	cmpl 	$0, %eax
	je 		endloop
	## buffer[iIndex]=buffer[iIndex+1]
	movl	iIndex, %eax
	sall 	$2, %eax
	addl	$buffer, %eax
	movl 	4(%eax), %ebx
	movl 	%ebx, (%eax)	
	## iIndex++
	incl	iIndex
	jmp 	loop
endloop:
	pushl	$buffer
	call	atoi
	## stack.push(no)
	pushl 	%eax
	jmp input 
elseif5:
	## if(buffer[0]!='p') goto elseif11
	movl	$buffer, %eax
	cmpl	$'p', (%eax)
	jne 	elseif11
	## if(stack.peek()!=NULL) goto else8
	movl	(%esp), %eax
	cmpl	$0, %eax   
	jne 	else8
	## printf("dc: stack empty\n")
	pushl	$sEmpty
	call	printf
	addl 	$4, %esp
	jmp 	input
else8:
	##printf("%d\n", (int)stack.top())
	movl	(%esp), %eax
	pushl	%eax
	pushl	Pprint
	call 	printf
	addl	$8, %esp
	jmp		input
elseif11:
	## if(buffer[0]!='q') goto elseif13
	movl	$buffer, %eax
	cmpl	$'q', (%eax)
	jne		elseif13
	jmp 	quit
elseif13:
	## if(buffer[0]!='+') goto elseif28
	movl	$buffer, %eax
	cmpl	$'+', (%eax)
	jne		elseif28
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
	movl	(%esp), %eax
	cmpl	$0, %eax  
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
	movl 	$0, %edx
	addl	%eax, %edx
	addl 	%ebx, %edx
	## stack.push(res)
	pushl 	%edx
	jmp 	input
elseif28:
	## if(buffer[0]!='-') goto elseif30
	movl	$buffer, %eax
	cmpl	$'-', (%eax)
	jne		elseif30
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
	movl	(%esp), %eax
	cmpl	$0, %eax 	
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
	movl 	$0, %edx
	addl	%eax, %edx
	subl 	%ebx, %edx
	## stack.push(res)
	pushl 	%edx
	jmp 	input
elseif30:
	## if(buffer[0]!='^') goto elseif32
	movl	$buffer, %eax
	cmpl	$'^', (%eax)
	jne		elseif32
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
	movl	(%esp), %eax
	cmpl	$0, %eax 	
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
elseif32:
	## if(buffer[0]!='*') goto elseif34
	movl	$buffer, %eax
	cmpl	$'*', (%eax)
	jne		elseif34
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
	movl	(%esp), %eax
	cmpl	$0, %eax 	
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
elseif34:
	## if(buffer[0]!='/') goto elseif36
	movl	$buffer, %eax
	cmpl	$'/', (%eax)
	jne		elseif34
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
	movl	(%esp), %eax
	cmpl	$0, %eax 	 
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
else36:
	//if(buffer[0]!='%')
	## int a,b
	//subl 	$8, %esp  이미 저장되있는 값이라서 필요없을것같은데
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
	movl	(%esp), %eax
	cmpl	$0, %eax 		
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
else33:
	## int no=atoi(buffer)
	pushl	$buffer
	call	atoi
	## stack.push(no)
	pushl 	%eax
	jmp input 
quit:
	movl	$0, %eax
	movl 	%ebp, %esp
	popl 	%ebp
	ret
power:
	pushl	%ebp
	movl 	%esp, %ebp
	## int iPower=1, int ipIndex=1;
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

