#include<stdio.h>
#include<iostream>
#include<math.h>
#include<ctype.h>
#include<stdlib.h>
#include<conio.h>
#include<stack>
#include<string.h>
#include<cstring>
using namespace std;

//二叉树节点类型
typedef enum
{
    INT,CHAR
}ElemTag;

//二叉树数据域
typedef struct treeNode
{
    ElemTag tag;
    union 
    {
        int num;
        char c;
    };  
}treeNode;

//二叉树定义
typedef struct BiTNode
{
    treeNode data;
    struct BiTNode *lchild;
    struct BiTNode *rchild;
}BiTNode,*BiTree;

typedef BiTree SElemType;


//判断是否为操作符
int isoperator(char opt)
{
    switch(opt)
    {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':return 1;
        default:return 0;
    }
}

//判断字符类型，数字or字符
void judge(BiTree &E,char a)
{
    if(isdigit(a))
    {
        E->data.tag=INT;
        E->data.num=a-'0';
    }
    else
    {
        E->data.tag=CHAR;
        E->data.c=a;
    }
}

//以字符序列的形式输入语法正确的前缀表达式并构成表达式 
int ReadExpr(BiTree &E,char *exp)
{
    int i,length;//length为exp的长度
    stack<SElemType> S;//操作符栈
    BiTree p,q;
    gets(exp);
    length=strlen(exp);
    E=(BiTree)malloc(sizeof(BiTNode));
    E->lchild=NULL;
    E->rchild=NULL;
    if(length==1)
    {
        if(isdigit(exp[0]))
        {
            E->data.tag=INT;
            E->data.num=exp[0]-'0';
            return 1;
        }
        else if(isalpha(exp[0]))
        {
            E->data.tag=CHAR;
            E->data.c=exp[0];
            return 1;
        }
        else
        {
            cout<<"ERROR"<<endl;
            return 0;
        }
    }
    else
    {
        judge(E,exp[0]);
        q=E;
        S.push(q);
        S.push(q);
        for(i=1;i<length && !S.empty();i++)
        {
            p=(BiTree)malloc(sizeof(BiTNode));
            judge(p,exp[i]);
            p->lchild=NULL;
            p->rchild=NULL;
            if(isoperator(exp[i]))
            {
                if(!q->lchild)//左非空，往左走
                {
                    q->lchild=p;
                    S.push(p);
                    q=p;
                }
                else//右非空，往右走
                {
                    q->rchild=p;
                    S.push(p);
                    q=p;
                }
            }
            else//非运算符，出栈
            {
                if(!q->lchild)//左非空，往左走
                {
                    q->lchild=p;
                    q=S.top();
                    S.pop();
                }
                else
                {
                    q->rchild=p;
                    q=S.top();
                    S.pop();
                }
            }
        }
        if(S.empty() && i>=length)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

//运算符优先级比较
int Compare(char a,char b)
{
    if(isoperator(a)&&isoperator(b))
    {
        if(a=='^')
        {
            if(b!='^')
            {
                return 1;
            }
            else return 0;
        }
        else if(a=='*' || a=='/')
        {
            if(b=='*' || b=='/' || b=='^')
            {
                return 0;
            }
            else return 1;
        }
        else return 0;
    }
    else return 0;
}

//中缀表达式（中序遍历）
void WriteExpr(BiTree E)
{
    if(E)//树非空
    {
        //左子树
        if(E->lchild && E->lchild->data.tag==CHAR)//左子树非空且为字符
        {
           if(Compare(E->data.c,E->lchild->data.c))
           //当前节点比左子树优先级要高
           {
              cout<<"(";
              WriteExpr(E->lchild);
              cout<<")";
           }
           else WriteExpr(E->lchild);
        }
        else
           WriteExpr(E->lchild);
        //根节点
        if(E->data.tag==INT)
        {
            cout<<E->data.num;
        }
        else
        {
            cout<<E->data.c;
        }
        //右子树
        if(E->rchild && E->rchild->data.tag==CHAR)
        {
            if(Compare(E->data.c,E->rchild->data.c))
            {
                cout<<"(";
                WriteExpr(E->rchild);
                cout<<")";
            }
            else WriteExpr(E->rchild);
        }
        else WriteExpr(E->rchild);
    }
}

//实现对变量V的赋值(V=c)，变量的初值为0
void Assign(BiTree &E,char V,int num,int &flag)//flag判断赋值成功
{
    if(E)
    {
        if(E->data.tag==CHAR && E->data.c==V)
        {
            E->data.tag==INT;
            E->data.num=num;
            flag=1;
        }
        Assign((E->lchild),V,num,flag);
        Assign((E->rchild),V,num,flag);
    }
}

//计算
int operate(int a,char opt,int b)
{
    switch(opt)
    {
        case '+':return a+b;
        case '-':return a-b;
        case '*':return a*b;
        case '/':
            if(b!=0)
            {
                return a/b;
            }
            else
            {
                cout<<"ERROR"<<endl;
                break;
            }
        case '^':return pow(a,b);
        default:return 0;
    }
    return 0;
}

//check没有赋值的变量
int check(BiTree E)
{
    if(E && E->data.tag==CHAR)
    {
        if(isalpha(E->data.c))
        {
            return 0;
        }
        check(E->lchild);
        check(E->rchild);
        return 1;
    }
    else return 1;
}

//对算术表达式E求值
int Value(BiTree E)
{
    if(E)
    {
        //叶子节点
        if(!E->lchild && !E->rchild && E->data.tag==INT)
        {
            return E->data.num;
        }
        return operate(Value(E->lchild),E->data.c,Value(E->rchild));
    }
    else return 0;
}

//构成一个新的复合表达式(E1)P(E2)
void CompoundExpr(char P,BiTree &E1,BiTree E2)
{
    BiTree E;
    E=(BiTree)malloc(sizeof(BiTNode));
    E->data.tag=CHAR;
    E->data.c=P;
    E->lchild=E1;
    E->rchild=E2;
    E1=E;
    cout<<"Expression E compounded successfully!"<<endl;
    cout<<"Its expression is"<<endl;
    WriteExpr(E);
}

int show()
{
    cout<<"Enter 1:请输入前缀表达式"<<endl;
    cout<<"Enter 2:输出中缀表达式"<<endl;
    cout<<"Enter 3:对变量赋值"<<endl;
    cout<<"Enter 4:计算表达式的值"<<endl;
    cout<<"Enter 5:创建复合表达式"<<endl;
    cout<<"Enter 6:Exit"<<endl;
    cout<<endl;
    cout<<"Please enter your choice"<<endl;
    int choice=-1;
    cin>>choice;
    getchar();
    return choice;
}

int main()
{
    BiTree E,newE;
    char EXPR[100];//存储前缀表达式
    char newExpr[100];
    int flag=0,flag2=0;
    char V;//赋值变量
    char P;//新操作符
    while(1)
    {
        switch(show())
        {
            case 1:
                do
                {
                    cout<<"Please enter the correct prefix expression："<<endl;
                    flag=ReadExpr(E,EXPR);
                    if(flag==1)
                    {
                        cout<<"Expression constructed successfully!"<<endl;
                    }
                    else
                    {
                        cout<<"Failed!"<<endl;
                    }
                }while (flag==0);
                if(flag==1)
                {
                    cout<<"The midfix expression is"<<endl;
                    WriteExpr(E);
                    cout<<endl<<endl;
                }
                break;
            case 2:
               if(flag==1)
               {
                    cout<<"The midfix expression is"<<endl;
                    WriteExpr(E);
                    cout<<endl;
               }
               else
                   cout<<"ERROR"<<endl;
                break;
            case 3:
                if(flag==1)
                {
                    int assign_tag=0;
                    char ch;
                    int n;
                    cout<<"Please enter the name of the variable to be assigned:"<<endl;
                    ch=getchar();
                    cout<<endl;
                    cout<<"The value to be assigned to is"<<endl;
                    cin>>n;
                    Assign(E,ch,n,assign_tag);
                    if(assign_tag==1)
                    {
                        cout<<"The expression after assignment is:"<<endl;
                        WriteExpr(E);
                        cout<<endl;
                    }
                    else
                    {
                        cout<<"ERROR"<<endl;
                    }
                }
                else
                    cout<<"ERROR"<<endl;
                break;
            case 4:
                if(flag==1)
                {
                    int temp=0;
                    temp=check(E);
                    if(temp==1)
                    {
                        WriteExpr(E);
                        cout<<"="<<Value(E)<<endl;
                    }
                    else
                    {
                        cout<<"ERROR"<<endl;
                    }
                }
                break;
            case 5:
                if(flag==1)
                {
                    do
                    {
                        cout<<"Please enter another expression:"<<endl;
                        flag2=ReadExpr(newE,newExpr);
                        if(flag2)
                        {
                            cout<<"New expression constructed successfully!"<<endl;
                        }
                        else
                        {
                            cout<<"Failed."<<endl;
                        }
                    } while (flag2==0);
                    cout<<"The midfix of new expression is:"<<endl;
                    WriteExpr(newE);
                    cout<<endl<<endl;
                do
                {
                    cout<<"Please enter an operator:"<<endl;
                    P=getchar();
                } while (!isoperator(P));
                CompoundExpr(P,E,newE);
                cout<<"The midfix representation of a composite prefix expression is:"<<endl;
                WriteExpr(E);
                cout<<endl<<endl;
                }
                break;
            case 6:
                cout<<"Press any key to exit!"<<endl;
                getch();
                exit(0);
                break;
            default:
                cout<<"ERROR"<<endl;
                getch();
                break;  
        }
    }
}