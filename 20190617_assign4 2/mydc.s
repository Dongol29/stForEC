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
	.equ   IPOWER, -4
	.equ   IPINDEX, -8
	.equ   N, -4
	.equ   I, -8

	
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

## While loop
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
	movl	$buffer, %edx
	movl 	(%edx), %edx
	movl 	%edx, %eax
	andl 	$0xFF, %eax  
	pushl	%eax
	call	isdigit
	addl	$4, %esp
	cmpl	$0, %eax
	jne  	else_digit

##  Checking if it is negative number by checking existence of the 
## pre-appending '-' symbol to the number. If it is, pushes negative 
## value to the stack.
if_minus:
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

##  Doing the operation corresponding to operator
##'p':  Prints the value that sits on the top of the stack.
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

##  Doing the operation corresponding to operator
##'q': quit the program.
elseif_q:
	## if(buffer[0]!='q') goto elseif_plus
	movl	$buffer, %eax
	cmpb	$'q', (%eax)
	jne		elseif_plus
	jmp 	quit

##  Doing the operation corresponding to operator
##'+': pops upmost two values from the stack, performs addition
## and pushes the result to the stack.
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

##  Doing the operation corresponding to operator
##'-': pops upmost two values from the stack, performs subtraction
## and pushes the result to the stack.
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

##  Doing the operation corresponding to operator
##'^': pops upmost two values from the stack, performs exponentiation
## and pushes the result to the stack.
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

##  Doing the operation corresponding to operator
##'*': pops upmost two values from the stack, performs multiflication
## and pushes the result to the stack.
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

##  Doing the operation corresponding to operator
##'/': pops upmost two values from the stack, performs division and 
## pushes the quotient to the stack.
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
	## b=(int)stack.pop()  
	popl	%ebx
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
	## a=(int)stack.pop()
	popl	%eax
	## res=a/b
	movl 	$0, %edx
	idivl	%ebx
	## stack.push(res)
	pushl	%eax
	jmp 	input

##  Doing the operation corresponding to operator
##'%': pops upmost two values from the stack, performs division and 
## pushes the remainder to the stack.
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
	## b=(int)stack.pop()  	
	popl	%ebx
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
	## a=(int)stack.pop()
	popl	%eax
	## res=a%b
	movl	$0, %edx
	idivl	%ebx
	## stack.push(res)
	pushl 	%edx
	jmp 	input

##  Doing the operation corresponding to operator
##'f': Prints the contents of the stack in LIFO order.
elseif_f:
	## if(buffer[0]!='f') goto elseif_c
	movl	$buffer, %eax
	cmpb	$'f', (%eax)
	jne		elseif_c
	## int i=%esp-%ebp
	movl 	%esp, %ebx
	subl 	%ebp, %ebx
loop_f:
	## if(i>=0) goto endloop_f
	cmpl 	$0, %ebx
	jge 	endloop_f
	## printf("%d\n",(%ebp,%ebx))
	movl 	$0, %edx
	addl 	%ebp, %edx
	addl 	%ebx, %edx
	pushl	(%edx)
	pushl	$Pprint
	call	printf
	addl 	$8, %esp
	addl 	$4, %ebx
	jmp 	loop_f
endloop_f:
	jmp 	input

##  Doing the operation corresponding to operator
##'c': Clears the contents of the stack.
elseif_c:
	## if(buffer[0]!='c') goto elseif_d
	movl	$buffer, %eax
	cmpb	$'c', (%eax)
	jne		elseif_d
	movl 	%ebp, %esp
	jmp 	input

##  Doing the operation corresponding to operator
##'d': Duplicates the top-most entry of the stack 
## and pushes it in the stack.
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

## Doing the operation corresponding to operator
##'r': Reverses the order of (swaps) the top two values on the stack.
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
	## stack.push(a)
	pushl	%eax
	## printf("dc: stack empty\n")
	pushl	$sEmpty
	call	printf
	addl 	$4, %esp
	jmp		input
endif14:
	## b=(int)stack.pop()
	popl 	%ebx
	## stack.push(b)
	pushl 	%eax
	## stack.push(a)
	pushl 	%ebx
	jmp 	input

## Doing the operation corresponding to operator 'x'
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

## Doing the operation corresponding to operator 'y'
elseif_y:
	## if(buffer[0]!='y') goto input
	movl	$buffer, %eax
	cmpb	$'y', (%eax)
	jne		input
	## a=stack.copy() 
	movl	(%esp), %eax
	## if(a<=1) goto input
	cmpl 	$1, %eax
	jle 	input
	## res=prime(a)
	pushl	%eax
	call 	prime
	addl 	$4, %esp
	## stack.push(res);
	pushl	%eax
	jmp 	input

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

##  This power function performs exponentiation and stores the result
## in eax.
##  The formal parameter offsets IBASE and IEXP are used.
##  The local variable offsets IPINDEX, IPOWER are used.
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
	movl 	IPOWER(%ebp), %eax
	imull 	IBASE(%ebp), %eax
	movl 	%eax, IPOWER(%ebp)
	## ipIndex++
	incl 	IPINDEX(%ebp)
	jmp 	ploop
ploopend:
	## return value(=iPower) to eax
	movl 	IPOWER(%ebp), %eax
	movl 	%ebp, %esp
	popl 	%ebp
	ret

##  This prime function finds the biggest one of the prime numbers that
## are lesser or equal to the input, stores it in the eax if it exist
##  The local variable offsets I and N are used.
prime:
	pushl	%ebp
	movl 	%esp, %ebp
	## int n=a
	movl 	8(%ebp), %eax
	pushl 	%eax
	pushl 	$2
loop1:
	## if(n<2) goto endloop1
	movl 	N(%ebp), %eax
	cmpl 	$2, %eax
	jl 		endloop1
	## i=2
	movl 	$2, I(%ebp)
loop2:
	## if(i>=n) goto endloop2
	movl 	I(%ebp), %eax
	cmpl 	N(%ebp), %eax
	jge 	endloop2
	## if(n%i==0) goto endloop2
	movl 	N(%ebp), %eax
	movl 	I(%ebp), %ebx
	movl 	$0, %edx
	idiv 	%ebx
	cmpl 	$0, %edx
	je 		endloop2
	## if(i==n-1) goto endloop1
	movl 	N(%ebp), %eax
	decl 	%eax
	cmpl 	I(%ebp), %eax
	je 		endloop1
	## i++
	incl 	I(%ebp)
	jmp 	loop2
endloop2:
	## n--
	decl 	N(%ebp)
	jmp 	loop1
endloop1:
	## return n to eax
	movl 	N(%ebp), %eax
	movl 	%ebp, %esp
	popl	%ebp
	ret

	

