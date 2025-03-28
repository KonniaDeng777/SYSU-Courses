#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include"math.h"
#define N 50
typedef struct mystack {
	int a[N];
	int top;
}ST;//ջ 

int isempty(ST* T) {//�ж�ջ�Ƿ�Ϊ�� 
	if (T->top < 0)
		return 1;
	else
		return 0;
}

int isfull(ST* T) {//�ж�ջ�Ƿ�Ϊ�� 
	if (T->top == N - 1)
		return 1;
	else
		return 0;
}

int gettop(ST* T) {//�õ�ջ��Ԫ�� 
	return T->a[T->top];
}

int pop(ST* T) {//��ջ 
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

void push(ST* T, int x) {//��ջ 
	if (T->top == N - 1) {
		printf("can not push\n");
		exit(0);
	}
	else {
		(T->top)++;
		T->a[T->top] = x;
	}
}

void transfer(char* in, char* post) {//����׺���ʽת��Ϊ��׺���ʽ 
	ST T;//ջ 
	int i, j, count, countleft = 0, countright = 0, flag = 0;//flag=1˵����֮ǰ�����������ж��Ƿ���������������� ,countΪһ������С����ĸ�����countleft��countright�ֱ�Ϊ�������ŵĸ��� 
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
			if (in[i] == '.') {//��¼ÿ������С����ĸ���������һ������ʽ���� 
				count++;
			}
		}
				i--;
				if (count > 1) {
					printf("���ʽ��������������С���㣩\n");
					exit(0);
				}
				post[j] = ' ';//�ÿո����ָ������� 
				j++;
				flag = 1;
				break;
		case '(':if (flag) {//�������ǰ�����֣�����ʽ���� 
			printf("���ʽ�������ֺ�ֱ�Ӹ����ţ�\n");
			exit(0);
		}
				push(&T, in[i]);
				countleft++;//�����Ÿ�����һ 
				break;
		case ')':countright++;
			while (gettop(&T) != '(') {
				post[j] = pop(&T);
				j++;
			}
			pop(&T);
			break;
		case '+':
		case '-':if (i == 0 && in[i] == '-') { //��һ����Ϊ��������� 
			post[0] = '0';
			post[1] = ' ';
			j += 2;
		}
				else if (!flag) {//��������ǰ�������������ʽ���� 
			printf("���ʽ�������������������֮��û�����֣�\n");
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
		case '/':if (!flag) {//��������ǰ�������������ʽ���� 
			printf("���ʽ�������������������֮��û�����֣�\n");
			exit(0);
		}
				while (!isempty(&T) && (gettop(&T) == '*' || gettop(&T) == '/')) {
					post[j] = pop(&T);
					j++;
				}
				push(&T, in[i]);
				flag = 0;
				break;
		default:printf("���ʽ���������˷����ַ�������Ķ�����\n");
			exit(0);
		}
	}
	if (countleft != countright) {
		printf("���ʽ�������������������ȣ�\n");
		exit(0);
	}
	while (!isempty(&T)) {
		post[j] = pop(&T);
		j++;
	}
	post[j] = '\0';
}

double cal(char* post) {
	int i, j, top, length, flag;//topΪջ����length��ʾ����С����ǰ�ĳ��ȣ�flag�ж������Ƿ����С���� 
	double temp, shu[10];//shu��10��������ջ����ű��ʽ�е����֣�temp�����������ֲ��Ž�shu�� 
	char ch[10];//�Ȱ����ֵı��ʽ�浽ch��10���У���ת��Ϊ���ִ浽shu�� 
	for (i = 0, j = 0, top = -1; post[i] != '\0'; i++) {
		if (post[i] >= '0' && post[i] <= '9') {//�����ǰΪ���֣��Ƚ����ִ浽ch�У���ת��Ϊdouble���Ͳ��浽shu�� 
			flag = 0; j = 0;
			while (post[i] != ' ') {//����һ�����Ƶ�ch�У�ֱ�����ֽ��� 
				if (post[i] == '.') {//�ж��Ƿ���С���㣬�ֱ����� 
					flag = 1;
				}
				ch[j] = post[i];
				i++;
				j++;
			}
			ch[j] = '\0';
			if (flag) {//��С��������������С����ǰ�ģ�����С������ 
				for (j = 0; ch[j] != '.'; j++);//���󳤶� 
				length = j - 1;
				for (j = 0, temp = 0.0; ch[j] != '.'; j++) {
					temp += (double)(ch[j] - '0') * pow(10, length - j);
				}
				for (j++, length++; ch[j] != '\0'; j++) {
					temp += (double)(ch[j] - '0') * pow(10, length - j);
				}
			}
			else {//ûС����������ֱ���� 
				for (j = 0; ch[j] != '\0'; j++);//���󳤶� 
				length = j - 1;
				for (j = 0, temp = 0.0; ch[j] != '\0'; j++) {
					temp += (double)(ch[j] - '0') * pow(10, length - j);
				}
			}
			top++;
			shu[top] = temp;//temp��ջ 
		}
		else {//������������ջ����������ջ����������������������ջ 
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
	char in[N], post[N];//inΪ��׺���ʽ postΪ��׺���ʽ 
	double jieguo;//������ 
	printf("��������׺���ʽ:\n");
	scanf("%s", in);
	while (strcmp(in, "#") != 0) {
		transfer(in, post);
		printf("��׺���ʽ��\n");
		printf("%s\n", post);
		jieguo = cal(post);
		printf("������(������λС��)��%.2lf", jieguo);
		printf("\n��������׺���ʽ:\n");
		scanf("%s", in);
	}
}//��207��  

#pragma once
