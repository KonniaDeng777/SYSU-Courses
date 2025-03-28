#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include"math.h"
#define N 50
typedef struct mystack {
	int a[N];
	int top;
}ST;//栈 

int isempty(ST* T) {//判断栈是否为空 
	if (T->top < 0)
		return 1;
	else
		return 0;
}

int isfull(ST* T) {//判断栈是否为满 
	if (T->top == N - 1)
		return 1;
	else
		return 0;
}

int gettop(ST* T) {//得到栈顶元素 
	return T->a[T->top];
}

int pop(ST* T) {//弹栈 
	int x;
	if (T->top < 0) {
		printf("can not pop\n");
		exit(0);
	}
	else {
		x = T->a[T->top];
		(T->top)--;
		return x;
	}
}

void push(ST* T, int x) {//入栈 
	if (T->top == N - 1) {
		printf("can not push\n");
		exit(0);
	}
	else {
		(T->top)++;
		T->a[T->top] = x;
	}
}

void transfer(char* in, char* post) {//将中缀表达式转化为后缀表达式 
	ST T;//栈 
	int i, j, count, countleft = 0, countright = 0, flag = 0;//flag=1说明是之前是数，用来判断是否是连续两个运算符 ,count为一个数中小数点的个数，countleft和countright分别为左右括号的个数 
	T.top = -1;
	for (i = 0, j = 0; in[i] != '\0'; i++) {
		switch (in[i]) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':for (count = 0; (in[i] <= '9' && in[i] >= '0') || in[i] == '.'; i++, j++) {
			post[j] = in[i];
			if (in[i] == '.') {//记录每个数中小数点的个数，超过一个则表达式有误 
				count++;
			}
		}
				i--;
				if (count > 1) {
					printf("表达式有误（数中有两个小数点）\n");
					exit(0);
				}
				post[j] = ' ';//用空格来分割两个数 
				j++;
				flag = 1;
				break;
		case '(':if (flag) {//如果括号前是数字，则表达式有误 
			printf("表达式有误（数字后直接跟括号）\n");
			exit(0);
		}
				push(&T, in[i]);
				countleft++;//左括号个数加一 
				break;
		case ')':countright++;
			while (gettop(&T) != '(') {
				post[j] = pop(&T);
				j++;
			}
			pop(&T);
			break;
		case '+':
		case '-':if (i == 0 && in[i] == '-') { //第一个数为负数的情况 
			post[0] = '0';
			post[1] = ' ';
			j += 2;
		}
				else if (!flag) {//如果运算符前是运算符，则表达式有误 
			printf("表达式有误（有连续两个运算符之间没有数字）\n");
			exit(0);
		}
				while (!isempty(&T) && gettop(&T) != '(') {
					post[j] = pop(&T);
					j++;
				}
				push(&T, in[i]);
				flag = 0;
				break;
		case '*':
		case '/':if (!flag) {//如果运算符前是运算符，则表达式有误 
			printf("表达式有误（有连续两个运算符之间没有数字）\n");
			exit(0);
		}
				while (!isempty(&T) && (gettop(&T) == '*' || gettop(&T) == '/')) {
					post[j] = pop(&T);
					j++;
				}
				push(&T, in[i]);
				flag = 0;
				break;
		default:printf("表达式有误（输入了非数字非运算符的东西）\n");
			exit(0);
		}
	}
	if (countleft != countright) {
		printf("表达式有误（左右括号数量不等）\n");
		exit(0);
	}
	while (!isempty(&T)) {
		post[j] = pop(&T);
		j++;
	}
	post[j] = '\0';
}

double cal(char* post) {
	int i, j, top, length, flag;//top为栈顶，length表示数字小数点前的长度，flag判断数字是否存在小数点 
	double temp, shu[10];//shu【10】是数字栈，存放表达式中的数字，temp用来计算数字并放进shu中 
	char ch[10];//先把数字的表达式存到ch【10】中，再转化为数字存到shu中 
	for (i = 0, j = 0, top = -1; post[i] != '\0'; i++) {
		if (post[i] >= '0' && post[i] <= '9') {//如果当前为数字，先将数字存到ch中，再转化为double类型并存到shu中 
			flag = 0; j = 0;
			while (post[i] != ' ') {//将这一串复制到ch中，直到数字结束 
				if (post[i] == '.') {//判断是否有小数点，分别讨论 
					flag = 1;
				}
				ch[j] = post[i];
				i++;
				j++;
			}
			ch[j] = '\0';
			if (flag) {//有小数点的情况，先算小数点前的，再算小数点后的 
				for (j = 0; ch[j] != '.'; j++);//先求长度 
				length = j - 1;
				for (j = 0, temp = 0.0; ch[j] != '.'; j++) {
					temp += (double)(ch[j] - '0') * pow(10, length - j);
				}
				for (j++, length++; ch[j] != '\0'; j++) {
					temp += (double)(ch[j] - '0') * pow(10, length - j);
				}
			}
			else {//没小数点的情况，直接算 
				for (j = 0; ch[j] != '\0'; j++);//先求长度 
				length = j - 1;
				for (j = 0, temp = 0.0; ch[j] != '\0'; j++) {
					temp += (double)(ch[j] - '0') * pow(10, length - j);
				}
			}
			top++;
			shu[top] = temp;//temp入栈 
		}
		else {//如果是运算符，栈顶两个数出栈并把这两个数的运算结果入栈 
			temp = shu[top--];
			switch (post[i]) {
			case'+':shu[top] += temp;
				break;
			case'-':shu[top] -= temp;
				break;
			case'*':shu[top] *= temp;
				break;
			case'/':shu[top] /= temp;
			}
		}
	}
	return shu[top];
}

main() {
	char in[N], post[N];//in为中缀表达式 post为后缀表达式 
	double jieguo;//计算结果 
	printf("请输入中缀表达式:\n");
	scanf("%s", in);
	while (strcmp(in, "#") != 0) {
		transfer(in, post);
		printf("后缀表达式：\n");
		printf("%s\n", post);
		jieguo = cal(post);
		printf("计算结果(保留两位小数)：%.2lf", jieguo);
		printf("\n请输入中缀表达式:\n");
		scanf("%s", in);
	}
}//共207行  

#pragma once
