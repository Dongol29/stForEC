##1 while (1) {
	##2	if (scanf("%s", buffer) == EOF)
	##3		return 0;
	##4 	if (!isdigit(buffer[0])) {
			if(buffer[0]=='_'){
				int iIndex;
				for(i=0;buffer[iIndex+1]!=0;iIndex++){
					buffer[iIndex]=buffer[iIndex+1];
				}
				int no = atoi(buffer);
				stack.push(no);
				goto input;
			}
	##5		if (buffer[0] == 'p') {
	##6			if (stack.peek() == NULL) { /* is %esp == %ebp? */
	##7				printf("dc: stack empty\n");
	##8			} else {
	##9				printf("%d\n", (int)stack.top()); /* value is already pushed in the stack */
	##10			}
	##11		} else if (buffer[0] == 'q') {
	##12			goto quit;
	##13		} else if (buffer[0] == '+') {
	##14			int a, b;
	##15			if (stack.peek() == NULL) {
	##16				printf("dc: stack empty\n");
	##17				continue;
	##18			}
	##19			a = (int)stack.pop();
	##20			if (stack.peek() == NULL) {
	##21				printf("dc: stack empty\n");
	##22				stack.push(a); /* pushl some register value */
	##23				continue;
	##24			}
	##25			b = (int)stack.pop(); /* popl to some register */
	##26			res = a + b;
	##27 			stack.push(res);
	##28		} else if (buffer[0] == '-') {
	##29			/* ... */
	##30		} else if (buffer[0] == '^') {
	##31			/* ... powerfunc() ... */
	##32		} else if (buffer[0] == 'f'){
					int i=%esp-%ebp
					for(;i<0;i+=4){
						printf("%d\n",(%ebp,%eax)))
					}
					continue
				} else if(buffer[0] == 'c'){

				}
				

	##33 	} else { /* the first no. is a digit */
	##34		int no = atoi(buffer);
	##35		stack.push(no);	/* pushl some register value */
	##36	}
	##37 }

a=stack.pop();



input:
	if(scanf("%s",buffer)==EOF) goto quit;
	if(isdigit(buffer[0])==0) goto else_digit;
	if(isdigit(buffer[0])!='_') goto elseif_p;
	int i=0;
loop:
	if(buffer[i+1]==0) goto endloop;
	buffer[i]=buffer[i+1];
	i++;
	goto loop;
endloop:
	int no=atoi(buffer);
	stack.push(no);
	goto input;
elseif_p
	if(buffer[0]!='p') goto elseif_q;
	if(stack.peek()!=NULL) goto else_p;
	printf("dc: stack empty\n");
	goto input;
else_p:
	printf("%d\n", (int)stack.top());
	goto input;
elseif_q:
	if(buffer[0]!='q') goto elseif_plus;
	goto quit;

## +
elseif_plus:
	if(buffer[0]!='+') goto elseif_minus;
	int a,b;
	if(stack.peek()!=NULL) goto endif1;
	printf("dc: stack empty\n");
	goto input;
endif1:
	a=(int)stack.pop();
	if(stack.peek()!=NULL) goto endif2;
	printf("dc: stack empty\n");
	stack.push(a);
	goto input;
endif2:
	b=(int)stack.pop();
	res=a+b;
	stack.push(res);
	goto input;
## -
elseif_minus:
	if(buffer[0]!='-') goto elseif_power;
	int a,b;
	if(stack.peek()!=NULL) goto endif3;
	printf("dc: stack empty\n");
	goto input;
endif3:
	a=(int)stack.pop();
	if(stack.peek()!=NULL) goto endif4;
	printf("dc: stack empty\n");
	stack.push(a);
	goto while;
endif4:
	b=(int)stack.pop();
	res=a-b;
	stack.push(res);
	goto input;
## ^
elseif_power:
	if(buffer[0]!='^') goto elseif_mul;
	int a,b;
	if(stack.peek()!=NULL) goto endif5;
	printf("dc: stack empty\n");
	goto while;
endif5:
	a=(int)stack.pop();
	if(stack.peek()!=NULL) goto endif6;
	printf("dc: stack empty\n");
	stack.push(a);
	goto while;
endif6:
	b=(int)stack.pop();
	res=power(a,b);
	stack.push(res);
	goto while;
## *
elseif_mul:
	if(buffer[0]!='*') goto elseif_quo;
	int a,b;
	if(stack.peek()!=NULL) goto endif7;
	printf("dc: stack empty\n");
	goto while;
endif7:
	a=(int)stack.pop();
	if(stack.peek()!=NULL) goto endif8;
	printf("dc: stack empty\n");
	stack.push(a);
	goto while;
endif8:
	b=(int)stack.pop();
	res=a*b; //mul
	stack.push(res);
	goto while;
## /
elseif_quo:
	if(buffer[0]!='/') goto elseif_rem;
	int a,b;
	if(stack.peek()!=NULL) goto endif9;
	printf("dc: stack empty\n");
	goto while;
endif9:
	a=(int)stack.pop();
	if(stack.peek()!=NULL) goto endif10;
	printf("dc: stack empty\n");
	stack.push(a);
	goto while;
endif10:
	b=(int)stack.pop();
	res=a/b;   //div
	stack.push(res);
	goto while;
## %
elseif_rem:
	if(buffer[0]!='%') goto elseif_f;
	int a,b;
	if(stack.peek()!=NULL) goto endif11;
	printf("dc: stack empty\n");
	goto while;
endif11:
	a=(int)stack.pop();
	if(stack.peek()!=NULL) goto endif12;
	printf("dc: stack empty\n");
	stack.push(a);  
	goto while;
endif12:
	b=(int)stack.pop();
	res=a%b; //div를 먼저 해서 저장 후 그 만큼 loop돌려서 뺌  --걍 얘를 하나의 함수로 만들까?
	stack.push(res);
	goto while;

elseif_f:
	if(buffer[0]!='f') goto elseif_c;
	int i=%esp-%ebp;
loop_f:
	if(i<=0) goto endloop_f;
	printf("%d\n",(%ebp,%eax));
	goto loop_f;
endloop_f:
	goto input;
elseif_c:
	if(buffer[0]!='c') goto elseif_d;
	/* movl		%ebp, %esp */
	goto input;
elseif_d:	
	if(buffer[0]!='d') goto elseif_r;
	if(stack.peek()!=NULL) goto else_d;
	printf("dc: stack empty\n");
	goto input;
else_d:
	/* movl 	(%esp), %eax
	   pushl 	%eax 	*/
	goto input;
elseif_r:
	if(buffer[0]!='r') goto elseif_x;
	int a,b;
	if(stack.peek()!=NULL) goto endif13;
	printf("dc: stack empty\n");
	goto input;
endif13:
	a=(int)stack.pop();
	if(stack.peek()!=NULL) goto endif14;
	printf("dc: stack empty\n");
	stack.push(a); //
	goto input;
endif14:
	b=(int)stack.pop();
	stack.push(b);
	stack.push(a);
	goto input;
elseif_x:
	if(buffer[0]!='x') goto elseif_y;
	srand(time(NULL));
	int a=rand()%1024 //=RAND_MAX
	stack.push(a);
	goto input;
elseif_y:
	if(buffer[0]!='y') goto input; //무시
	a=stack.pop();
	if(a<=1) goto input;
	res=prime(a);
	stack.push(res);
	goto input;
else_digit:
	int no=atoi(buffer);
	stack.push(no);
	goto while;
quit:

int prime(int a) //a>=2
{
	int i,n;
	for(n=a;n>=2;n--){
		for(i=2;i<n;i++){
			if(n%i==0) break;
			if(i==n-1)  return n;
		}
	    continue;
	}
}

int prime(int a){
	int n=a, i=2;
loop1:
	if(n<2) goto endloop1;
loop2:
	if(i>n) goto endloop2;
	if(n%i==0) goto endloop2;
	if(i==n-1) goto endloop1;
	i++;
	goto loop2;
endloop2:
	n--;
	goto loop1;
endloop1:
	return n
}


prime:
	pushl	%ebp
	movl 	%esp, %ebp
