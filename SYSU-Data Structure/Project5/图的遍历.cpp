/*��Ŀ�� 
���ڽӱ�Ϊ�洢�ṹ��ʵ����ͨ����ͼ��������Ⱥ͹�����ȱ��������û�ָ���Ľ��Ϊ��㣬
�ֱ����ÿ�ֱ����µĽ��������к���Ӧ�������ı߼���
*/
#include<iostream>
#include <string.h>
#include <malloc.h>
#include <conio.h>
using namespace std;
int visited[30];        //ȫ������,��¼ͼ�нڵ����״̬
int k,vi,vj;
//=======================================�ڽӱ�
#define MAX_N 30
typedef struct ArcNode  //�߽�� 
{ 
	int adjvex;         //���ڽӵ��ڶ��������е��±�
	struct ArcNode *nextarc;  //����ָ����һ���ڽӵ� 
}ArcNode;
typedef struct VNode     	//������
{	
	int	data;		        //������Ϣ         
	ArcNode *firstarc;	   //����ͷָ��
}VNode, AdjList[MAX_N];       //��������(�ṹ������)
typedef struct
{   
	AdjList vertices;  //�ڽӱ�
	int vexnum,arcnum;   //�������ͱ���
}ALGraph;

/****************************************************************************************
*������ ��CreateGraph
*������������ ��ͨ������ߵ���������ͼ 
*�������� ��ALGraph &G
*��������ֵ ���� 
*****************************************************************************************/
void CreateGraph(ALGraph &G) {
	// ͼG���ڽӱ��ʾ,����ͼ
	cout<<"�����붥�����ͱ���"<<endl;
    cin>>G.vexnum>>G.arcnum;
	cout<<"���붥������"<<endl;
    for(k = 1; k <= G.vexnum; k++)
	{ 
		cin>>G.vertices[k].data; //���붥������
		G.vertices[k].firstarc = NULL;   // ��ʼ��ͷָ��
	}
	printf("===========================\n");
    for (k = 1; k <= G.arcnum; ++k)
	{
        printf("����ÿ�����ϵĶ������: ");
        scanf("%d%d",&vi,&vj);

        ArcNode *pArcNode = (ArcNode*)malloc(sizeof(ArcNode));
        pArcNode->adjvex = vj;
        pArcNode->nextarc = G.vertices[vi].firstarc;
        G.vertices[vi].firstarc = pArcNode;

        pArcNode = (ArcNode*)malloc(sizeof(ArcNode));
        pArcNode->adjvex = vi;
        pArcNode->nextarc = G.vertices[vj].firstarc;
        G.vertices[vj].firstarc = pArcNode;
    }        												//��vi��vj���㶼ָ��Է� 
         
}  

/****************************************************************************************
*������ ��DFS
*������������ �����������������ͼG 
*�������� ��G-������ͼ   v-�������� 
*��������ֵ ���� 
*****************************************************************************************/

void DFS(ALGraph G,  int v)  {
  
    visited[v] = 1;                   //���ʺ���Ϊ1 
  	cout<<G.vertices[v].data<<"  "; 
	ArcNode *x = (ArcNode*)malloc(sizeof(ArcNode));
	if(!x)   exit(-1);               //û���ɳɹ��˳� 
	x = G.vertices[v].firstarc; 
	int w; 
	for (; x; x = x->nextarc)     //��x���ڽӵ���� 
	{ 
		w = x->adjvex;  
		if(!visited[w]) 
			DFS(G,w); 			//�ݹ����DFS 
	}
}



void DFSB (ALGraph G,int v)//��������ı߼�
{ 
	visited[v]=1; 
	ArcNode *y; 
	y=(ArcNode*)malloc(sizeof(ArcNode));
	if(!y) exit(-1);
	y=G.vertices[v].firstarc; 
	int u=G.vertices[v].data; 
	int w; 
	for(;y;y=y->nextarc)
	{ 
		w=y->adjvex;  
		if(visited[w]==0)
		{ 
			cout<<u<<"--->"<<w<<endl; 
			DFSB(G,w);  
		} 
	}
}


typedef struct QNode
{ 
	int data;
	QNode *next;
}QNode,*QueuePtr;
typedef struct
{
	QueuePtr front; 
	QueuePtr rear;
}LinkQueue;
void InitQueue (LinkQueue &Q)//����һ���ն���
{ 
	Q.front=Q.rear=(QueuePtr)malloc(sizeof(QNode)); 
	if(!Q.front) exit(-1); 
	Q.front->next=NULL;
}
void EnQueue (LinkQueue &Q,int e)//����
{ 
	QNode *p;
	p=(QNode*)malloc(sizeof(QNode)); 
	if(!p) exit(-1);
	p->data=e; 
	p->next=NULL; 
	Q.rear->next=p;
	Q.rear=p; //free(p);
}
int DeQueue (LinkQueue &Q,int &e)//����
{ 
	if(Q.front == Q.rear)  return -1; 
	QNode *p; 
	p = (QNode*)malloc(sizeof(QNode)); 
	if(!p) exit(-1); 
	p = Q.front->next;
	e = p->data; 
	Q.front->next = p->next;
	if(Q.rear==p)  Q.rear = Q.front; 
	free(p); 
	return e;
}
int QueueEmpty (LinkQueue Q)//�ж϶����Ƿ�Ϊ��
{ 
	if(Q.front == Q.rear)  return 1; 
	return 0;
}
/****************************************************************************************
*������ ��BFS
*������������ �����������������ͼG 
*�������� ��G-������ͼ   v-�������� 
*��������ֵ ���� 
*****************************************************************************************/
void BFS(ALGraph G,int v)//�������
{  
	int u;
	LinkQueue Q; 
	InitQueue(Q); 
	if(visited[v]==0) {
		visited[v]=1; 
		cout<<G.vertices[v].data<<"  "; 
		EnQueue(Q,v);  
		while(QueueEmpty(Q)!=1)  {   
			DeQueue(Q,u);  
			ArcNode *z;  
			z=(ArcNode*)malloc(sizeof(ArcNode));   
			if(!z) exit(-1);  
			z=G.vertices[u].firstarc;   
			int w;         
			for(;z;z=z->nextarc){  
				w=z->adjvex;   
				if(visited[w]==0){  
					visited[w]=1;  
					cout<<w<<"  ";    
					EnQueue(Q,w);    
				}   
			}  
		} 
	}
}
void BFSB (ALGraph G,int v)//��������ı߼�
{ 
 int u; 
 LinkQueue Q;
 InitQueue(Q); 
 if(visited[v]==0) { 
	 visited[v]=1; 
	 EnQueue(Q,v);  
	 while(QueueEmpty(Q)!=1){
		 DeQueue(Q,u);   
		 ArcNode *r;  
		 r=(ArcNode*)malloc(sizeof(ArcNode));   
		 if(!r) exit(-1);  
		 r=G.vertices[u].firstarc; 
		 int w; 
		 for(;r!=NULL;r=r->nextarc){ 
			 w=r->adjvex;   
			 if(visited[w]==0){    
				 visited[w]=1;    
				 cout<<u<<"--->"<<w<<endl;  
				 EnQueue(Q,w);    
			 }  
		 }  
	 } 
 }
}
int main()
{
	int i; 
	ALGraph G; 
	CreateGraph(G);       //����ͼ 
	
	cout<<"�������һ��Ҫ���ʵĽ����ţ�"<<endl;
	int n;
	scanf("%d",&n); 
	
	for( i = 0; i < 30; i++)  
	visited[i] = 0; 	//��ʼ�����ʵĽڵ� 
	cout<<"�������������"<<endl; 
	DFS(G,n);   		//�����������
	for( i = 0; i < 30; i++)  
	visited[i] = 0;  cout<<endl;
	cout<<"�߼���"<<endl;
	DFSB(G,n); 			//���ʵı߼�
	
	for( i = 0; i < 30; i++)    
	visited[i] = 0;     //��ʼ�����ʵĽڵ� 
	cout<<"�������������"<<endl;    
	BFS(G,n);         	//����������� 
	for( i = 0; i < 30; i++)    
	visited[i] = 0; 	    cout<<endl; 
	cout<<"�߼���"<<endl;       
	BFSB(G,n); 			//���ʵı߼�
	 
	
	return 0;
}

