#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<fstream>
using namespace std;

//结构体-单词
typedef struct Str
{
    char str[50];//存放单词
    int cnt;//记录单词长度
}Str;
//count the number of vocabularies in the text
int vcount(char* p)
{
    FILE *fp;
    char temp[50];//排序中临时存储单词
    char tmp[50];//读取文件中临时存储单词
    Str words[200];
    int num=0;//the number of different vocabularies
    int count=0;//the sum of vocabularies
    int dig=0;//临时存放的数字
    int i,j,neww=1;//neww:whether it is a new word
    int m,n;
    if((fp=fopen(p,"r"))==NULL)
    {
        cout<<"File does not exist"<<endl;
        return -1;
    }
    loop:while(fscanf(fp,"%s",tmp)!=EOF)
    {
        neww=1;
        for(i=0;i<50;i++)
        {
            if(tmp[i]>='0' && tmp[i]<='9')//数字不计入单词串
            {
                goto loop;
            }
        }
        for(i=0;i<50;i++)
        {
            if(tmp[i]=='#' || tmp[i]=='.' || tmp[i]==',' || tmp[i]=='?' || tmp[i]=='!')//标点符号,自定义结束符 "$"断开
            {
                tmp[i]='\0';
            }
        }
        
        for(i=0;i<num;i++)
        {
            if(strcmp(tmp,words[i].str)==0)
            {
                neww=0;
                words[i].cnt++;
            }
        }
        //若是新单词，则copy字符串到words数组
        if(neww)
        {
            for(j=0;tmp[j]!='\0';j++)
            {
                words[num].str[j]=tmp[j];
            }
            words[num].str[j]='\0';//词尾加结束符
            words[num++].cnt=1;//新单词数+1
        }
    }
    //词频排序
    for(m=0;m<num-1;m++)
    {
        for(n=0;n<num-1;n++)
        {
            if(words[n].cnt>words[n+1].cnt)
            {
                strcpy(temp,words[n].str);
                strcpy(words[n].str,words[n+1].str);
                strcpy(words[n+1].str,temp);
                dig=words[n].cnt;
                words[n].cnt=words[n+1].cnt;
                words[n+1].cnt=dig;

            }
        }
    }
    for(m=0;m<num;m++)
    {
        count+=words[m].cnt;
    }
    cout<<"There are "<<num<<" different words."<<endl;
    for(i=num-1;i>=0;i--)
    {
        cout<<words[i].str<<" "<<words[i].cnt<<endl;
    }
    cout<<"The number of words is "<<count<<endl;
    fclose(fp);
    return 0;
}

int Index(char* p)
{									
	int j=0,i=0;//j用于记录输入的子串的长度,i用于记录主串的长度
	char S[1000],T[50];	//s[]存放文件中的字符，t[]存放要搜索的单词 
    char ch1,ch2;									
    FILE *fp;
    if((fp=fopen(p,"r"))==NULL)
    {
        cout<<"Error"<<endl;
        return -1;
    }
    ch1=fgetc(fp);			//将文件读取到一维数组当中
    while(ch1!='#')
    {
    	S[i]=ch1;
        i++;
        ch1=fgetc(fp);
    }
    S[i]='#';
    cout<<"Please enter the vocabulary:"<<endl;
    cin>>T;
	ch2=T[0];
    while(ch2!='\0')
    {				
    	j++;
    	ch2=T[j]; 
	}
    int flag_row=0;
    int row=1,col=1;
    int m=0,n=0,q=1;
    int num=0;
    int left=0; 
    while(m<i){
        if(S[m]=='"')
        {								//加引号的单词处理：跳过引号，读取下一个字符，并设置标记left=1 
        	m=m+1;
        	left=1;
		}
		if(S[m]==T[n])
        {
            m++;
            n++;
        }
        else{
            m=m-n+1;
            n=0;
        }

        if(n==j&&S[m+1]!=' ')
        {						//匹配到相应单词 
            num++;									//出现次数加1 
            if(row==1)
            {
                col=m-n+1;
                if(flag_row!=row)
                {
                    cout<<"Line "<<row<<endl;
                    flag_row=row;
                }
                
            }
            else
            {
                col=m-n-q; 
                if(flag_row!=row)
                {
                    cout<<"Line "<<row<<endl;
                    flag_row=row;
                }
            }
        }

		if(S[m]==' '||(S[m]=='"' &&left==1))
        {					
			col++;
			left=0;
			while(S[m]==' '||S[m]=='"') 
				m=m+1;
		}
    	if(S[m]=='\n')
        {
        	row++;
        	q=m;
    	}
    	if(S[m]=='#')
        {
        	cout<<"The sum of this vocabulary is: "<<num<<endl;
        	return num;
    	}
	}
    return 0;
}


void getnext(char* p, int next[]) 
{
    char temp[200];
    FILE *fp;
    if((fp=fopen(p,"r"))==NULL)
    {
        cout<<"Error"<<endl;
        return;
    }
	int i, j;
	i = 0;//指向字符串每个字符的下标
	j = -1;
	next[i] = j;//next[0]放上-1 
    int length=0;
    while(fscanf(fp,"%s",temp)!=EOF)
    {
        length+=strlen(temp);
        while (i < length) 
        {
             //没有到达结尾的话 
		    if (j == -1 || temp[i] == temp[j]) 
            {//如果是第一个字符或遇到相同的字符
			    next[++i] = ++j;
		    }
		    else 
            {
			    j = next[j];
		    }
	    }
    }
	
}

int kmp(char* p, int next[]) 
{
    int counts=0;
	int i=0,j=0,t=0;
    char T[1000],S[50];
    FILE *fp;
    int flag_row=0;
    int row=1;
    char current_char;
    int num_characters=0;
    if((fp=fopen(p,"r"))==NULL)
    {
        cout<<"Error"<<endl;
    }
    ifstream my(p);
    if(my.is_open())
    {
        while(!my.eof())
        {
            my.getline(T,1000);
        }
    }
    t=strlen(T);
    T[t]='#';
    cout<<"Please enter the vocabulary:"<<endl;
    cin>>S;
	while (true) 
    {
		if (j == -1 || T[i] == S[j]) 
        {
			i++; 
            j++;
		}
		else 
        {
			j = next[j];
		}

		if (!(i < strlen(T) && j <strlen(S))) 
        {
			if (j == strlen(S)) 
            {
                if(flag_row!=row)
                {
                    cout<<"Line "<<row<<" ";
                    counts++;
                    flag_row=row;
				    j = 0;
                }       
			}
			if (i == strlen(T)) 
            {
				return 0;
			}
		}
        if(S[j]=='\n')
        {
            row=row+1;
        }
        if(S[j]=='#')
        {
            cout<<"The sum of this vocabulary is: "<<counts<<endl;
            break;
        }

	}
	if (j == strlen(S)) 
	{ 
		return counts;
	}
	else
	{
		return -1;
	}	
}

void createfile(char* p)
{
    FILE *fp;
    char ch;
    fp=fopen(p,"w+");
    ch=getchar();
    cout<<"Please enter the passage(end with #):"<<endl;
    while(ch!='#')
    {
        ch=getchar();
        fputc(ch,fp);
    }
    fclose(fp);
}

int main()
{
    int j=1,l=0;
    int count=-1,c=-1;
    char filename[255];
    char afilename[255];
    char *p;
    int next[200];
    while(true)
    {
        cout<<"文本分析工具"<<endl;
        cout<<"Enter 1: 创建文件"<<endl;
        cout<<"Enter 2: 单词查询及定位"<<endl;
        cout<<"Enter 3: 单词词频统计"<<endl;
        cout<<"Enter 4: Exit"<<endl;
        cin>>l;
        fflush(stdin);
        if(l!=1&&l!=2&&l!=3&&l!=4)
        {
            cout<<"Error,please enter 1-4"<<endl;
        }
        if(l==1)
        {
            cout<<"请输入所要创建的文件名（含拓展名）"<<endl;
            scanf("%[^\n]",filename);
            p=filename;
            createfile(p);
            cout<<"创建成功"<<endl;
        }
        if(l==2)
        {
            while(count==-1)
            {
                cout<<"请输入所需查询的文件名称（含拓展名）"<<endl;
                scanf("%[^\n]",afilename);
                fflush(stdin);
                p=afilename;
                getnext(p,next);
                count=kmp(p,next);
                //count=Index(p);
                if(count==0)
                {
                    cout<<"None"<<endl;
                }
                if(count==-1)
                {
                    cout<<"Error"<<endl;
                    break;
                }
            }
        }
        if(l==3)
        {
            while(c==-1)
            {
                cout<<"请输入所需查询的文件名称（含拓展名）"<<endl;
                scanf("%[^\n]",afilename);
                fflush(stdin);
                p=afilename;
                c=vcount(p);
                if(c==-1)
                {
                    cout<<"Error,please enter a correct filename"<<endl;
                    break;
                }
            }
        }
        if(l==4)
        {
            cout<<"Exit"<<endl;
            break;
        }
    }
    system("pause");
}
