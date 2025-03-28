#define _CRT_SECURE_NO_WARNINGS   
#include<stdio.h>
#include<algorithm>
#include<cmath>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>
#include <conio.h>
#include <windows.h>
#include<cstddef>
#include<iostream>
using namespace std;
double coefs[80];  //��ϵ�� 
int expns[80];  //��ָ�� 
int cnt, m;
double get_coef(char* str)  //��������ַ�������ȡϵ��
{
	long double s = 0.0;
	long double d = 10.0;
	bool flag = false;
	while (*str == ' ')  str++;
	if (*str == '-')//��¼��������  
	{
		flag = true;  str++;
		if (*str == 'x') 	return -1.0;
	}
	else if ((*str == '+' && *(str + 1) == 'x') || (*str == 'x')) return 1.0;
	if (*str == '+' && (*(str + 1) >= '0' && *(str + 1) <= '9'))str++;
	if (!(*str >= '0' && *str <= '9'))     return s; //���һ��ʼ���������˳�������0.0     
	while (*str >= '0' && *str <= '9' && *str != '.')//����С����ǰ��������  
	{
		s = s * 10.0 + *str - '0';
		str++;
	}
	if (*str == '.')    str++;         //�Ժ�ΪС������      
	while (*str >= '0' && *str <= '9')  //����С������  
	{
		s = s + (*str - '0') / d;
		d *= 10.0;
		str++;
	}
	return s * (flag ? -1.0 : 1.0);
}
void getNums() //��������ַ�������ȡϵ����ָ��
{
	int i = 0;
	cnt = 0;
	double coef=0.0;
	int expn=0;
	char str[80];
	cin >> str;
	while (*(str + i))
	{
		coef = get_coef(str + i);
		if (*(str + i) != 'x') i++;
		while ((*(str + i) >= '0' && *(str + i) <= '9') || (*(str + i) == '.'))  	i++;
		if (*(str + i) == '+' || *(str + i) == '-' || *(str + i) == '\0')            expn = 0;
		else if (*(str + i) == 'x')
		{
			i++;
			if (*(str + i) == '+' || *(str + i) == '-' || *(str + i) == '\0') expn = 1;
			else if (*(str + i) == '^')
			{
				i++;
				expn = (int)get_coef(str + i);
				while ((*(str + i) >= '0' && *(str + i) <= '9') || (*(str + i) == '.'))i++;
			}
		}
		coefs[cnt] = coef;
		expns[cnt] = expn;
		cnt++;
	}
}
typedef struct Polynomial //����ʽ
{
	double coef; //ϵ��
	int expn;   //ָ��
	struct Polynomial* next;
} Polynomial, * Polyn;
//����һ��ͷָ��Ϊhead������Ϊm��һԪ����ʽ
void CreatPolyn(Polyn head, int m)  //���������ڲ��������ʵ�ֵ���������
{
	for (int i = 0; i < m; i++)
	{
		Polyn p = (Polyn)malloc(sizeof(struct Polynomial));
		p->coef = coefs[i];
		p->expn = expns[i];
		if (p->coef == 0) free(p);
		else
		{
			Polyn q1, q2;
			q1 = head;
			q2 = head->next;
			while (q2 != NULL && p->expn < q2->expn)
			{
				q1 = q2;
				q2 = q2->next;
			}
			if (q2 != NULL && p->expn == q2->expn)
			{
				q2->coef += p->coef;
				if (q2->coef == 0)
				{
					q1->next = q2->next;
					free(q2);
				}
				free(p);
			}
			else
			{
				p->next = q2;
				q1->next = p;
			}
		}
	}
}



void printPoLlyn(Polyn head) //���и�ʽ����ӡ���
{
	Polyn q = head->next;
	int flag = 0;     //��¼�Ƿ�Ϊ��һ��
	if (!q)
	{
		puts(" \t");
		return;
	}
	while (q)
	{
		if (q->coef > 0 && flag == 1)
		{
			printf("+");
		}
		flag = 1;
		if (q->coef != 1 && q->coef != -1)
		{
			printf("%g", q->coef);

			if (q->expn == 1) printf("x");
			else if (q->expn != 0) printf("x^%d", q->expn);
		}
		else
		{
			if (q->coef == 1)
			{
				if (q->expn == 0) printf("1");
				else if (q->expn == 1) printf("x");
				else printf("x^%d", q->expn);
			}
			if (q->coef == -1)
			{
				if (q->expn == 0) printf("-1");
				else if (q->expn == 1) printf("-x");
				else printf("-x^%d", q->expn);
			}
		}
		q = q->next;
	}
	printf("\t\t");
}
int compare(Polyn a, Polyn b)//�Ƚ���������ʽ�Ĵ�С
{
	if (a && b)  // ����ʽa��b����Ϊ��
	{
		if (a->expn > b->expn) return 1;// a��ָ������b��ָ��
		else if (a->expn < b->expn) return -1;
		else return 0;
	}
	else if (!a && b) return -1; //aΪ�գ�b��Ϊ��
	else if (a && !b) return 1;  //bΪ�գ�a��Ϊ�� 
	else if (!a && !b)return 0;  //a,b��Ϊ��
}
void clear(Polyn c)
{
	Polyn p, q;
	p = c;
	while (p->next != NULL)
	{
		q = p->next;
		p->next = q->next;
		free(q);
	}
	c->next = NULL;
}

//����ʽ��������
void downPolynomial(Polyn head)
{
	double coef = 0.0;
	int expn = 0;
	while (head != NULL)
	{
		Polyn q = head->next;
		while (q != NULL)
		{
			if (compare(head, q) == -1)
			{
				coef = head->coef;
				head->coef = q->coef;
				q->coef = coef;
				expn = head->expn;
				head->expn = q->expn;
				q->expn = expn;
			}
			else if (compare(head, q) == 0)
			{
				head->coef = head->coef + q->coef;
				head->next = q->next;
			}
			q = q->next;
		}
	}
}

void addPolyn(Polyn a1, Polyn b1, Polyn c1)  //���a+b
{
	Polyn a = a1;
	Polyn b = b1;
	Polyn c = c1;
	clear(c1);
	Polyn head, qc;
	Polyn qa = a->next;
	Polyn qb = b->next;
	head = c;
	while (qa || qb)
	{
		qc = (Polyn)malloc(sizeof(Polynomial));
		if (compare(qa, qb) == 1)
		{
			qc->coef = qa->coef;
			qc->expn = qa->expn;
			qa = qa->next;
		}
		else if (compare(qa, qb) == 0) //ָ����ͬ��ֱ�����
		{
			qc->coef = qa->coef + qb->coef;
			qc->expn = qa->expn;
			qa = qa->next;
			qb = qb->next;
		}
		else
		{
			qc->coef = qb->coef;
			qc->expn = qb->expn;
			qb = qb->next;
		}

		if (qc->coef != 0) //���ýڵ����������
		{
			qc->next = c->next;
			c->next = qc;
			c = qc;
		}
	}
}
void subPolyn(Polyn a, Polyn b, Polyn c)// a-b������a+b����⣬��b�ĳ�-b
{
	Polyn h = b;
	Polyn p = b->next;
	while (p)
	{
		p->coef *= -1;
		p = p->next;
	}
	addPolyn(a, h, c);
	for (Polyn i = h->next; i != 0; i = i->next)
	{
		i->coef *= -1;
	}
}

void value(Polyn head, int flag)  //����x��ֵ
{
	printf("x=");
	double sum = 0, x;
	cin >> x;
	for (Polyn p = head->next; p != 0; p = p->next)
	{
		double tmp = 1;
		int expn = p->expn;
		while (expn != 0) //ָ����Ϊ0
		{
			if (expn < 0)      tmp /= x, expn++;
			else if (expn > 0) tmp *= x, expn--;
		}
		sum += p->coef * tmp;
	}
	if (flag == 1) printf("A(%g)��ֵ=%g", x, sum);
	if (flag == 2) printf("B(%g)��ֵ=%g", x, sum);
	if (flag == 3) printf("C(%g)��ֵ=%g", x, sum);
}

//����ʽ��
void deriPolynomial(Polyn a)
{
	Polyn p1;
	p1 = a->next;
	while (p1)
	{
		if (p1->expn == 0)
		{
			p1->coef = 0;
			p1 = p1->next;
		}
		p1->coef = p1->coef * p1->expn;
		p1->expn = p1->expn - 1;
		p1 = p1->next;
	}
	printPoLlyn(a);

}

//����ʽ����
void Attach(int c, int e, Polyn head)
{
	Polyn p=(Polyn)malloc(sizeof(struct Polynomial));
	p->coef = c;
	p->expn = e;
	p->next = NULL;
	head->next = p;
	head = p;
}
Polyn Copy(Polyn head)
{
	Polyn head_copy = NULL;
	Polyn copy = head;
	Polyn tail = head_copy;
	while (copy != NULL)
	{
		Polyn p = (Polyn)malloc(sizeof(struct Polynomial));
		p->coef= copy->coef;
		p->expn = copy->expn;
		if (tail == NULL)
		{
			p->next = NULL;
			head_copy = p;
			tail = p;
		}
		else
		{
			p->next = tail->next;
			tail->next = p;
			tail = p;
		}
		copy = copy->next;
	}
	return head_copy;
}
//����
Polyn Add_Poly(Polyn head1, Polyn head2)
{
	Polyn head = NULL;
	Polyn p1 = head1;
	Polyn p2 = head2;
	Polyn tail = head;

	while (p1 != NULL || p2 != NULL)
	{
		if (p1 && p2 && p1->expn > p2->expn)
		{
			Polyn p = (Polyn)malloc(sizeof(struct Polynomial));
			p->expn = p1->expn;
			p->coef = p1->coef;
			if (tail == NULL)
			{
				p->next = NULL;
				head = p;
				tail = p;
			}
			else
			{
				p->next = tail->next;
				tail->next = p;
				tail = p;
			}
			p1 = p1->next;
		}

		if (p1 && p2 && p2->expn > p1->expn)
		{
			Polyn p = (Polyn)malloc(sizeof(struct Polynomial));
			p->expn = p2->expn;
			p->coef = p2->coef;
			if (tail == NULL)
			{
				p->next = NULL;
				head = p;
				tail = p;
			}
			else
			{
				p->next = tail->next;
				tail->next = p;
				tail = p;
			}
			p2 = p2->next;
		}

		if (p1 && p2 && p1->expn == p2->expn)
		{
			Polyn p = (Polyn)malloc(sizeof(struct Polynomial));
			p->expn = p1->expn;
			p->coef = p1->coef + p2->coef;
			if (p->coef != 0)
			{
				if (tail == NULL)
				{
					p->next = NULL;
					head = p;
					tail = p;
				}
				else
				{
					p->next = tail->next;
					tail->next = p;
					tail = p;
				}
			}
			p1 = p1->next;
			p2 = p2->next;
		}

		//����һ������ʽ������֮�󣬽���һ������ʽʣ�����������
		if (p1 == NULL)
		{
			while (p2 != NULL)
			{
				Polyn p = (Polyn)malloc(sizeof(struct Polynomial));
				p->expn = p2->expn;
				p->coef = p2->coef;
				if (tail == NULL)
				{
					p->next = NULL;
					head = p;
					tail = p;
				}
				else
				{
					p->next = tail->next;
					tail->next = p;
					tail = p;
				}
				p2 = p2->next;
			}
		}

		if (p2 == NULL)
		{
			while (p1 != NULL)
			{
				Polyn p = (Polyn)malloc(sizeof(struct Polynomial));
				p->expn = p1->expn;
				p->coef = p1->coef;
				if (tail == NULL)
				{
					p->next = NULL;
					head = p;
					tail = p;
				}
				else
				{
					p->next = tail->next;
					tail->next = p;
					tail = p;
				}
				p1 = p1->next;
			}
		}
	}
	return head;
}
//����ʽ����
int countPolynomial(Polyn head)
{
	Polyn p = head->next;
	int count = 0;
	for (p; p != NULL; p = p->next)
	{
		++count;
	}
	return count;
}

//����ʽ�˷�
Polyn mutiPolynomial(Polyn a, Polyn b)
{
	Polyn head = NULL;
	Polyn current = Copy(b);
	Polyn a_copy = Copy(a);
	Polyn p = a_copy;
	while (p != NULL)
	{
		p->coef *= current->coef;
		p->expn += current->expn;
		p = p->next;
	}
	head = a_copy;
	current = current->next;
	while (current != NULL)
	{
		Polyn a_copy_2 = Copy(a);
		Polyn q = (Polyn)malloc(sizeof(struct Polynomial));
		q = a_copy_2;
		while (q != NULL)
		{
			q->coef *= current->coef;
			q->expn += current->expn;
			q = q->next;
		}
		head=Add_Poly(head, a_copy_2);
		current = current->next;

	}
	int count = countPolynomial(head);
	int i = 0;
	Polyn temp = head->next;
	Polyn cnt = head;
	for (i = 0; i < count; i++)
	{
		if ((temp->expn>=50) || (temp->expn<=-50)  )
		{
			Polyn t;
			t = temp;
			temp = temp->next;
			cnt->next = p;
			free(t);
		}
		else
		{
			cnt = cnt->next;
			temp = temp->next;
		}
		
	}
	return head;
}

void show(Polyn a, Polyn b, Polyn c) //����ʵ��
{

	cout << "һԪϡ�����ʽ������" << endl << endl;
	cout << "A: ";
	printPoLlyn(a);
	cout << endl<<endl;
	cout << "B: ";
	printPoLlyn(b);
	cout << endl<<endl;
	cout << "C: ";
	printPoLlyn(c);
	cout << endl<<endl;
	cout << "��0���ж���ʽA��B������" << endl << endl;
	cout << "��1�������ʽA��ֵ" << endl << endl;
	cout << "��2�������ʽB��ֵ" << endl << endl;
	cout << "��3�������ʽC��ֵ" << endl << endl;
	cout << "��4���ж���ʽ���" << endl << endl;
	cout << "��5���ж���ʽA��" << endl << endl;
	cout << "��6���ж���ʽB��" << endl << endl;
	cout << "��7���ж���ʽ���" << endl << endl;
	cout << "��8���ж���ʽ���" << endl << endl;
	cout << "��enterִ��ȷ������" << endl << endl;
	cout << "��t�˳�����ʽ������" << endl << endl;
}
void create(Polyn a1, Polyn b1)
{
	Polyn a = a1;
	Polyn b = b1;
	clear(a1);
	clear(b1);
	printf("���������ʽa :  ");
	getNums();
	m = cnt;
	CreatPolyn(a, m);
	printf("���������ʽb :  ");
	getNums();
	m = cnt;
	CreatPolyn(b, m);
}
int main()
{
	Polyn a = (Polyn)malloc(sizeof(struct Polynomial)); a->next = NULL;
	Polyn b = (Polyn)malloc(sizeof(struct Polynomial)); b->next = NULL;
	Polyn c = (Polyn)malloc(sizeof(struct Polynomial)); c->next = NULL;
	system("һԪϡ�����ʽ������ ");
	system("mode con cols=50 lines=40");
	system("color b0");
	char ch, ch1;
	while (1)
	{
		system("cls");
		show(a, b, c);
		ch = _getch();
		if (ch == '0')
		{
			create(a, b);
		}
		else if (ch == '4')
		{
			c=mutiPolynomial(a,b);
			printPoLlyn(c);
		}
		else if (ch == '5')
		{
			deriPolynomial(a);
		}
		else if (ch == '6')
		{
			deriPolynomial(b);
		}
		else if (ch == '7')
		{
			addPolyn(a, b, c);
		}
		else if (ch == '8')
		{
			subPolyn(a, b, c);
		}
		else if (ch == 't')
		{
			exit(0);
		}
		else if (ch == '1')
		{
			value(a, 1);
			ch1 = _getch();
		}
		else if (ch == '2')
		{
			value(b, 2);
			ch1 = _getch();
		}
		else if (ch == '3')
		{
			value(c, 3);
			ch1 = _getch();
		}
	}
	return 0;
}


