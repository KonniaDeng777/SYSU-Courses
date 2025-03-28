/*题目： 
以邻接表为存储结构，实现连通无向图的深度优先和广度优先遍历。以用户指定的结点为起点，
分别输出每种遍历下的结点访问序列和相应生成树的边集。
*/
#include<iostream>
#include <string.h>
#include <malloc.h>
#include <conio.h>
using namespace std;
int visited[30];        //全局数组,记录图中节点访问状态
int k,vi,vj;
//=======================================邻接表
#define MAX_N 30
typedef struct ArcNode  //边结点 
{ 
	int adjvex;         //该邻接点在顶点数组中的下标
	struct ArcNode *nextarc;  //链域指向下一个邻接点 
}ArcNode;
typedef struct VNode     	//顶点结点
{	
	int	data;		        //顶点信息         
	ArcNode *firstarc;	   //边链头指针
}VNode, AdjList[MAX_N];       //顶点数组(结构体数组)
typedef struct
{   
	AdjList vertices;  //邻接表
	int vexnum,arcnum;   //顶点数和边数
}ALGraph;

/****************************************************************************************
*函数名 ：CreateGraph
*函数功能描述 ：通过输入边的数对生成图 
*函数参数 ：ALGraph &G
*函数返回值 ：空 
*****************************************************************************************/
void CreateGraph(ALGraph &G) {
	// 图G用邻接表表示,创建图
	cout<<"请输入顶点数和边数"<<endl;
    cin>>G.vexnum>>G.arcnum;
	cout<<"输入顶点序列"<<endl;
    for(k = 1; k <= G.vexnum; k++)
	{ 
		cin>>G.vertices[k].data; //输入顶点序列
		G.vertices[k].firstarc = NULL;   // 初始化头指针
	}
	printf("===========================\n");
    for (k = 1; k <= G.arcnum; ++k)
	{
        printf("输入每条边上的顶点序号: ");
        scanf("%d%d",&vi,&vj);

        ArcNode *pArcNode = (ArcNode*)malloc(sizeof(ArcNode));
        pArcNode->adjvex = vj;
        pArcNode->nextarc = G.vertices[vi].firstarc;
        G.vertices[vi].firstarc = pArcNode;

        pArcNode = (ArcNode*)malloc(sizeof(ArcNode));
        pArcNode->adjvex = vi;
        pArcNode->nextarc = G.vertices[vj].firstarc;
        G.vertices[vj].firstarc = pArcNode;
    }        												//让vi，vj顶点都指向对方 
         
}  

/****************************************************************************************
*函数名 ：DFS
*函数功能描述 ：深度优先搜索遍历图G 
*函数参数 ：G-遍历的图   v-出发顶点 
*函数返回值 ：空 
*****************************************************************************************/

void DFS(ALGraph G,  int v)  {
  
    visited[v] = 1;                   //访问后置为1 
  	cout<<G.vertices[v].data<<"  "; 
	ArcNode *x = (ArcNode*)malloc(sizeof(ArcNode));
	if(!x)   exit(-1);               //没生成成功退出 
	x = G.vertices[v].firstarc; 
	int w; 
	for (; x; x = x->nextarc)     //将x的邻接点访问 
	{ 
		w = x->adjvex;  
		if(!visited[w]) 
			DFS(G,w); 			//递归调用DFS 
	}
}



void DFSB (ALGraph G,int v)//深度搜索的边集
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
void InitQueue (LinkQueue &Q)//建立一个空队列
{ 
	Q.front=Q.rear=(QueuePtr)malloc(sizeof(QNode)); 
	if(!Q.front) exit(-1); 
	Q.front->next=NULL;
}
void EnQueue (LinkQueue &Q,int e)//进队
{ 
	QNode *p;
	p=(QNode*)malloc(sizeof(QNode)); 
	if(!p) exit(-1);
	p->data=e; 
	p->next=NULL; 
	Q.rear->next=p;
	Q.rear=p; //free(p);
}
int DeQueue (LinkQueue &Q,int &e)//出队
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
int QueueEmpty (LinkQueue Q)//判断队列是否为空
{ 
	if(Q.front == Q.rear)  return 1; 
	return 0;
}
/****************************************************************************************
*函数名 ：BFS
*函数功能描述 ：广度优先搜索遍历图G 
*函数参数 ：G-遍历的图   v-出发顶点 
*函数返回值 ：空 
*****************************************************************************************/
void BFS(ALGraph G,int v)//广度搜索
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
void BFSB (ALGraph G,int v)//广度搜索的边集
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
	CreateGraph(G);       //建立图 
	
	cout<<"请输入第一个要访问的结点序号："<<endl;
	int n;
	scanf("%d",&n); 
	
	for( i = 0; i < 30; i++)  
	visited[i] = 0; 	//初始化访问的节点 
	cout<<"深度优先搜索："<<endl; 
	DFS(G,n);   		//深度优先搜索
	for( i = 0; i < 30; i++)  
	visited[i] = 0;  cout<<endl;
	cout<<"边集："<<endl;
	DFSB(G,n); 			//访问的边集
	
	for( i = 0; i < 30; i++)    
	visited[i] = 0;     //初始化访问的节点 
	cout<<"广度优先搜索："<<endl;    
	BFS(G,n);         	//广度优先搜索 
	for( i = 0; i < 30; i++)    
	visited[i] = 0; 	    cout<<endl; 
	cout<<"边集："<<endl;       
	BFSB(G,n); 			//访问的边集
	 
	
	return 0;
}

