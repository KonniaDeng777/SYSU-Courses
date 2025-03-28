#include<iostream>
#include<stdio.h>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

#define NAME_NO 30
#define HASH_LENGTH 50
#define M 50;

typedef struct NAME
{
	char const* name;      //名字的拼音
	int key;         //拼音所对应的关键字
}NAME;

typedef struct HASH        //哈希表
{
	char const* name;     //名字的拼音
	int key;        //拼音所对应的关键字
	int size;       //查找长度
}HASH;

NAME NameList[HASH_LENGTH];
HASH HashList[HASH_LENGTH];

void InitNameList()
{
	int r,i;
	NameList[0].name = "Huang Xin";
	NameList[1].name = "Zheng Bei Chen";
	NameList[2].name = "Xu Zhan Chao";
	NameList[3].name = "Huang Lan Shu";
	NameList[4].name = "Lin Wen Jun";
	NameList[5].name = "Deng Xu Ying";
	NameList[6].name = "Zhong Xin Yu";
	NameList[7].name = "Zhang Jia Jun";
	NameList[8].name = "Jin Si Qi";
	NameList[9].name = "Chen Jin Qiang";
	NameList[10].name = "Zhong Zheng Zhou";
	NameList[11].name = "Li Zhong Tian";
	NameList[12].name = "He Qian Ying";
	NameList[13].name = "Li Hao Hui";
	NameList[14].name = "Zheng Yue";
	NameList[15].name = "Chen Rui Qi";
	NameList[16].name = "Liu Jia Yi";
	NameList[17].name = "Wei Jia Zhuo";
	NameList[18].name = "Ma Jun Jie";
	NameList[19].name = "Liu Ming Liang";
	NameList[20].name = "Liu Kun Sheng";
	NameList[21].name = "Wang Liang Zhi";
	NameList[22].name = "Sun Hang Qi";
	NameList[23].name = "Liang Zhi Zhao";
	NameList[24].name = "Liang Quan Feng";
	NameList[25].name = "Zhang Wen Kai";
	NameList[26].name = "Wen Zi Jian";
	NameList[27].name = "Ma Feng Rui";
	NameList[28].name = "Pang Hao";
	NameList[29].name = "Yang Bo";
	for (i = 0; i < NAME_NO; i++)
	{
        for (r = 0; (*(NameList[i].name + r)) != '\0'; r++)
            NameList[i].key += toascii(*(NameList[i].name+r)); 
	}
}
void CreateHashList()
{
	int i;
	for (i = 0; i < HASH_LENGTH; i++)
	{
		HashList[i].name = "";
		HashList[i].key = 0;
		HashList[i].size = 0;
	}
	for (i = 0; i < NAME_NO; i++)
	{
		int sum = 0;
		int adr = (NameList[i].key) % M;//哈希函数
		int d = adr;
		if (HashList[adr].size == 0)//如果不冲突
		{
			HashList[adr].key = NameList[i].key;
			HashList[adr].name = NameList[i].name;
			HashList[adr].size = 1;
		}
		else//冲突  
		{
			do
			{
				d = (d + NameList[i].key % 10 + 1) % M;//伪随机探测再散列法处理冲突    
				sum = sum + 1;                 //查找次数加1    
			} while (HashList[d].key != 0);
			HashList[d].key = NameList[i].key;
			HashList[d].name = NameList[i].name;
			HashList[d].size = sum + 1;
		}
	}
}
void  FindList() //查找    
{
	char name[20];
	int s0 = 0, r, sum = 1, adr, d;
	printf("请输入姓名的拼音:");
    gets(name);
	for (r = 0;name[r]!='\0'; r++)   //求出姓名的拼音所对应的整数(关键字)
		s0 += name[r];
	adr = s0 % M;   //使用哈希函数
	d = adr;
	if (HashList[adr].key == s0)       //分3种情况进行判断
		printf("\n姓名:%s   关键字:%d   查找长度: 1", HashList[d].name, s0);
	else if (HashList[adr].key == 0)
    {
        printf("Not Found!");
    }
	  	
	else
	{
		int g = 0;
		do
		{
			d = (d + s0 % 10 + 1) % M;       //伪随机探测再散列法处理冲突                     
			sum = sum + 1;
			if (HashList[d].key == 0)
			{
				printf("Not Found!");
				g = 1;
			}
			if (HashList[d].key == s0) 
			{
				printf("\n姓名:%s   关键字:%d   查找长度:%d", HashList[d].name, s0, sum);
				g = 1;
			}
		}while (g == 0);
	}
}
void Display()
{
	int i;
	float average = 0;
	printf("\n地址\t关键字\t\t搜索长度\tH(key)\t 姓名\n"); //显示的格式
	for (i = 0; i < 50; i++)
	{
		printf("%d ", i);
		printf("\t%d ", HashList[i].key);
		printf("\t\t%d ", HashList[i].size);
		printf("\t\t%d ", HashList[i].key % 30);
		printf("\t %s ", HashList[i].name);
		printf("\n");
	}
	for (i = 0; i < HASH_LENGTH; i++)
		average += HashList[i].size;
	average /= NAME_NO;
	printf("\n平均查找长度: ASL(%d)=%f \n", NAME_NO, average);
}

int input()
{
	cout << "Enter 1:显示哈希表" << endl;
	cout << "Enter 2:查找" << endl;
	cout << "Enter 0:Exit" << endl;
	cout << "Please enter your choice!" << endl;
	int choice;
	cin >> choice;
	return choice;
}
int main()
{
	InitNameList();
	CreateHashList();
	while (1)
	{
		switch (input())
		{
		case 1:Display(); cout << endl; break;
		case 2:getchar();FindList(); cout << endl; break;
		case 0:cout << "Press any key to exit!" << endl; exit(0);
        default:cout<<"Error Input"<<endl;break;
		}
	}
}