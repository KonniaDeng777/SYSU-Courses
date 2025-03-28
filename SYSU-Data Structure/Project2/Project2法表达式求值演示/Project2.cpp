#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include"math.h"
#include<stack>
#include<iostream>
#include<iomanip>

using namespace std;

void tran(char* input, char* output)//中缀转后缀
{
	stack<int> str;
	int i = 0, j = 0,count=0;
	int left = 0, right = 0;
	int flag = 0;//flag=1说明是之前是数，用来判断是否是连续两个运算符 ,left和right分别为左右括号的个数 
	for (i = 0, j = 0; input[i] != '\0'; i++)
	{
		switch (input[i])
		{
		case'0':
		case'1':
		case'2':
		case'3':
		case'4':
		case'5':
		case'6':
		case'7':
		case'8':
		case'9':
			for (; (input[i] <= '9' && input[i] >= '0') || input[i]=='.'; i++, j++)
			{
				output[j] = input[i];
				if (input[i] == '.')
				{
					count++;
				}
			}
			i--;
			if (count > 0)
			{
				cout << "Error：输出的数字为非整数" << endl;
				exit(0);
			}
			output[j] = ' ';
			j++;
			flag = 1;
			break;
		case'(':
			if (flag)
			{
				cout << "Error：数字后直接跟括号" << endl;
				exit(0);
			}
			str.push(input[i]);
			left++;
			break;
		case')':
			right++;
			while (str.top() != '(')
			{
				output[j] = str.top();
				str.pop();
				j++;
			}
			str.pop();
			break;
		case'+':
		case'-':
			if (i == 0 && input[i] == '-')//输入的第一个数为负数
			{
				output[0] = '0';
				output[1] = ' ';
				j += 2;
			}
			else if (!flag)//不考虑单目运算符
			{
				cout << "Error：连续两个运算符之间没有数字" << endl;
				exit(0);
			}
			while (!str.empty() && str.top() != '(')
			{
				output[j] = str.top();
				str.pop();
				j++;
			}
			str.push(input[i]);
			flag = 0;
			break;
		case'*':
		case'/':
			if (!flag)
			{
				cout << "Error：连续两个运算符之间没有数字" << endl;
				exit(0);
			}
			while (!str.empty() && (str.top() == '*' || str.top() == '/'))
			{
				output[j] = str.top();
				str.pop();
				j++;
			}
			str.push(input[i]);
			flag = 0;
			break;
		case'#':
			cout << "输入结束" << endl;
			break;
		default:cout << "Error：输入错误" << endl;
			exit(0);
		}
	}
	if (left != right)
	{
		cout << "Error：左右括号数量不等" << endl;
		exit(0);
	}
	while (!str.empty())
	{
		output[j] = str.top();
		str.pop();
		j++;
	}
	output[j] = '\0';
}

double cal(char* output)
{
	int i = 0, j = 0;
	int length = 0;
	int top = -1;
	double num_stack[10]={};//数字栈，存放表达式中的数字
	double temp=0;//计算数字
	char ch[10]={};//先把数字的表达式存到ch[10]中，再转化为数字存到num_stack中
	for (i = 0, j = 0; output[i] != '\0'; i++)
	{
		if (output[i]>='0' && output[i] <= '9')
		{
			j = 0;
			while (output[i] != ' ')//将output复制到ch中，直到数字结束
			{
				ch[j] = output[i];
				i++;
				j++;
			}
			ch[j] = '\0';
			for (j = 0; ch[j] != '\0'; j++)
			{
				length = j - 1;
			}
			for (j = 0, temp = 0.0; ch[j] != '\0'; j++)
			{
				temp += (double)(ch[j] - '0') * pow(10, length - j +1);
			}
			top++;
			num_stack[top] = temp;
		}
		else//如果是运算符
		{
			temp = num_stack[top--];
			switch (output[i])
			{
			case'+':num_stack[top] += temp;
				break;
			case'-':num_stack[top] -= temp;
				break;
			case'*':num_stack[top] *= temp;
				break;
			case'/':
				if (temp == 0)
				{
					cout << "Error:分母为0" << endl;
					exit(0);
				}
				num_stack[top] /= temp;
				break;
			}
		}
	}
	return num_stack[top];
}

void printstackchar(stack<char> s)
{
	stack<char> temp;
	int length = s.size();
	int i = 0;
	if (s.empty())
	{
		return;
	}
	while (!s.empty())
	{
		temp.push(s.top());
		s.pop();
	}
	while (!temp.empty())
	{
		cout << temp.top()<<" ";
		temp.pop();
	}
	cout << " ";
}
void printstacknum(stack<double> s)
{
	stack<double> temp;
	int length = s.size();
	int i = 0;
	if (s.empty())
	{
		return;
	}
	while (!s.empty())
	{
		temp.push(s.top());
		s.pop();
	}
	while (!temp.empty())
	{
		cout << temp.top()<<" ";
		temp.pop();
	}
	cout << " ";
}

int isOPRT(char c)//判断c是不是运算符 
{
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '#')return 1;
	else return 0;
}

char compare(char a, char b) //优先级比较
{
	if (a == '+')
	{
		if (b == '*' || b == '/' || b == '(') return '<';
		else return '>';
	}
	else if (a == '-')
	{
		if (b == '*' || b == '/' || b == '(') return '<';
		else return '>';
	}
	else if (a == '*')
	{
		if (b == '(')return '<';
		else return '>';
	}
	else if (a == '/')
	{
		if (b == '(')return '<';
		else return '>';
	}
	else if (a == '(')
	{
		if (b == ')')return '=';
		else if (b == '#') return '!';
		else return '<';
	}
	else if (a == ')')
	{
		if (b == '(')return '!';
		else return '>';

	}
	else if (a == '#')
	{
		if (b == ')')return '!';
		if (b == '#')return '=';
		else return '<';
	}
}

double calculate(double left, double right, char operators)
{
	switch (operators)
	{
	case '+':
		return left + right;

	case '-':
		return 1.0 * left - right;

	case '*':
		return left * right;

	case '/':
		return 1.0 * left / right;
	}
}

void outputstack(char* input)
{
	stack<char> oprt;//运算符栈
	stack<double> num;//运算数栈
	stack<double> temp;//缓冲区
	int build = 0;//若干数位构成的数字
	double index = 0.0;//某个数位上的数字
	int complex = 1;//10的幂次
	char operators;//运算符
	double left, right;
	oprt.push('#');
	int error = 0;
	int length = strlen(input);
	int i = 0;
	for (i = 0; i < length; i++)
	{
		while (input[i] != '#' || (!oprt.empty() && oprt.top() != '#'))
		{
			while (!isOPRT(input[i]))
			{
				temp.push(input[i] - '0');
				i++;
			}
			while (!temp.empty())
			{
				index = temp.top();
				temp.pop();
				build += (index * complex);
				complex *= 10;
			}
			complex = 1;
			if (build)
			{
				num.push(build);
			}
			cout << endl << "运算符栈：";
			printstackchar(oprt);
			cout << "运算数栈：";
			printstacknum(num);
			build = 0;
			if (isOPRT(input[i]))
			{
				switch (compare(oprt.top(), input[i]))
				{
				case'<':
					oprt.push(input[i]);
					i++;
					cout << endl << "运算符栈：";
					printstackchar(oprt);
					cout << "运算数栈：";
					printstacknum(num);
					break;
				case'=':
					operators = oprt.top();
					oprt.pop();
					cout << endl << "运算符栈：";
					printstackchar(oprt);
					cout << "运算数栈：";
					printstacknum(num);
					break;
				case'>':
					operators = oprt.top();
					oprt.pop();
					right = num.top();
					num.pop();
					left = num.top();
					num.pop();
					num.push(calculate(left, right, operators));
					cout << endl << "运算符栈：";
					printstackchar(oprt);
					cout << "运算数栈：";
					printstacknum(num);
					break;
				case'!':
					error = 1;
					break;
				default:break;
				}
			}
			if (error)break;
		}
	}
}

int main()
{
	char input[50], output[50];
	double ans = 0.0;
	cout << "请输入中缀表达式（以‘#’结束）：" << endl;
	cin >> input;
	
	while (strcmp(input, "#") != 0)
	{
		int length = strlen(input);
		tran(input, output);
		cout << "后缀表达式为：" << endl;
		cout << output << endl;
		ans = cal(output);
		outputstack(input);
		cout << endl<<"计算结果为：" <<  ans << endl;
		if (input[length - 1] == '#')
		{
			exit(0);
		}
	}

}