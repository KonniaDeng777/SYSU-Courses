#include <algorithm>
#include <iostream>
#include <string.h>
#include <queue>
#include <stack>
#include <malloc.h>
#include <conio.h>
#include <stdlib.h>
#include "tool.h"
#include <vector>
#define MAX 31
using namespace std;

vector<int> visited(31, 0);
int k, vi, vj, W;

typedef struct ArcNode //边结点
{
    int adjvex;              //该邻接点在顶点数组中的下标
    struct ArcNode *nextarc; //链域指向下一个邻接点
} ArcNode;
typedef struct VNode //顶点结点
{
    string location;   //顶点信息
    ArcNode *firstarc; //边链头指针
} VNode, AdjList[MAX]; //顶点数组(结构体数组)
typedef struct ALGraph
{
    AdjList vertices;     //邻接表
    int vexnum, arcnum;   //顶点数和边数
    int weight[MAX][MAX]; //权值
} ALGraph;

void CreateGraph(ALGraph &G)
{
    cout << "请输入地点数和路径数" << endl;
    cin >> G.vexnum >> G.arcnum;
    cout << "请按顺序输入地点名称" << endl;
    for (k = 0; k < G.vexnum; k++)
    {
        cin >> G.vertices[k].location;
        G.vertices[k].firstarc = NULL;
    }
    cout << "===========================" << endl;
    cout << "输入每条路径上的两个地点的序号和距离：" << endl;
    for (k = 0; k < G.arcnum; k++)
    {    
        cin >> vi >> vj >> W;
        G.weight[vi-1][vj-1] = W;
        G.weight[vj-1][vi-1] = W;
        ArcNode *pArcNode = (ArcNode *)malloc(sizeof(ArcNode));
        pArcNode->adjvex = vj-1;
        pArcNode->nextarc = G.vertices[vi-1].firstarc;
        G.vertices[vi-1].firstarc = pArcNode;

        pArcNode = (ArcNode *)malloc(sizeof(ArcNode));
        pArcNode->adjvex = vi-1;
        pArcNode->nextarc = G.vertices[vj-1].firstarc;
        G.vertices[vj-1].firstarc = pArcNode;
    }
}

class node
{
public:
    int data;
    node *next;
};

class Stack
{
public:
    node *top;
    Stack()
    { //利用构造函数将栈指针设置为NULL,否则top指针就成了野指针,因为没有指向一块内存
        top = NULL;
    }
    void push(int x) //进栈操作
    {
        node *s = new node;
        s->data = x;
        s->next = top; //结点的next域指向头指针
        top = s;       //头指针再指向新结点
    }
    int pop() //出栈操作
    {
        if (top == NULL)
        {
            cout << "Error" << endl;
            return 0;
        }
        else
        {
            node *p = top;
            int x = top->data;
            top = top->next;
            delete p;
            return x;
        }
    }
    bool empty()
    {
        if (top == NULL)
        {
            return true;
        }
        else
            return false;
    }
    int Top()
    {
        return top->data;
    }
};

void DFS(ALGraph &G, int source)
{
    visited.assign(visited.size(), 0);
    Stack s;
    s.push(source);
    visited[source] = 1;
    cout << G.vertices[source].location << " ";
    while (!s.empty())
    {
        int current;
        current = s.Top();
        ArcNode *p = G.vertices[current].firstarc;
        while (p)
        {
            if (visited[p->adjvex])
            {
                p = p->nextarc;
            }
            else
            {
                cout << G.vertices[p->adjvex].location << " ";
                s.push(p->adjvex);
                visited[p->adjvex] = 1;
                break;
            }
            if (p == NULL)
            {
                s.pop();
            }
        }
    }
    cout << endl;
}

void DFSB(ALGraph G, int source) //深度优先遍历的边集
{

    visited[source] = 1;
    ArcNode *y;
    y = (ArcNode *)malloc(sizeof(ArcNode));
    if (!y)
        exit(-1);
    y = G.vertices[source].firstarc;
    string u = G.vertices[source].location;
    int w;
    for (; y; y = y->nextarc)
    {
        w = y->adjvex;
        if (visited[w] == 0)
        {
            cout << u << "--->" << G.vertices[w].location << ":" << G.weight[source][w] << endl;
            DFSB(G, w);
        }
    }
}

void BFS(ALGraph G, int source) //广度优先遍历
{
    visited.assign(visited.size(), 0);
    int u;
    queue<int> Q;
    if (visited[source] == 0)
    {
        visited[source] = 1;
        cout << G.vertices[source].location << " ";
        Q.push(source);
        while (!Q.empty())
        {
            u = Q.front();
            Q.pop();
            ArcNode *z;
            z = (ArcNode *)malloc(sizeof(ArcNode));
            if (!z)
                exit(-1);
            z = G.vertices[u].firstarc;
            int w;
            for (; z; z = z->nextarc)
            {
                w = z->adjvex;
                if (visited[w] == 0)
                {
                    visited[w] = 1;
                    cout << G.vertices[w].location << " ";
                    Q.push(w);
                }
            }
        }
    }
    cout << endl;
}

void BFSB(ALGraph G, int source) //广度优先遍历的边集
{
    int u;
    visited.assign(visited.size(), 0);
    queue<int> Q;
    if (visited[source] == 0)
    {
        visited[source] = 1;
        Q.push(source);
        while (!Q.empty())
        {
            u = Q.front();
            Q.pop();
            ArcNode *r;
            r = (ArcNode *)malloc(sizeof(ArcNode));
            if (!r)
                exit(-1);
            r = G.vertices[u].firstarc;
            int w;
            for (; r != NULL; r = r->nextarc)
            {
                w = r->adjvex;
                if (visited[w] == 0)
                {
                    visited[w] = 1;
                    cout << G.vertices[u].location << "--->" << G.vertices[w].location << ":" << G.weight[u][w] << endl;
                    Q.push(w);
                }
            }
        }
    }
}


//求图G从顶点v出发的深度优先生成树
void DFSTree(ALGraph G, int v)
{
    ArcNode *p;
    visited[v] = 1;             //置已访问标记
    p = G.vertices[v].firstarc; // p指向顶点v的第一个相邻点
    while (p != NULL)
    {
        if (visited[p->adjvex] == 0) //若p->adjvex顶点未访问,递归访问它
        {
            //printf("(%d,%d) ", v, p->adjvex);
            cout<<"("<<G.vertices[v].location<<","<<G.vertices[p->adjvex].location<<")";
            DFSTree(G, p->adjvex);
        }
        p = p->nextarc; // p指向顶点v的下一个相邻点
    }
    
}

//求图G从顶点v出发的广度优先生成树
void BFSTree(ALGraph G, int v)
{
    int que[MAX]; //定义环形队列
    int que_front = 0, que_rear = 0;
    ArcNode *p;
    int Visited[MAX]; //定义顶点访问标志数组
    int i;
    int adjvex;

    for (i = 0; i < G.vexnum; i++)
        Visited[i] = 0; //顶点访问标志数组初始化
    Visited[v] = 1;     //置已访问标记
    que_rear++;         //顶点v进队
    que[que_rear] = v;
    while (que_front != que_rear) //队不空循环
    {
        que_front = (que_front + 1) % (MAX - 1);
        adjvex = que[que_front];         //出队一个顶点adjvex
        p = G.vertices[adjvex].firstarc; // p指向adjvex的第一个相邻点
        while (p != NULL)                //查找adjvex的所有相邻点
        {
            if (Visited[p->adjvex] == 0) //若当前邻接点未被访问
            {
                //printf("(%d,%d) ", adjvex, p->adjvex);
                cout<<"("<<G.vertices[adjvex].location<<","<<G.vertices[p->adjvex].location<<")";
                Visited[p->adjvex] = 1; //置已访问标记
                que_rear = (que_rear + 1) % (MAX - 1);
                que[que_rear] = p->adjvex; //顶点p->adjvex入队
            }
            p = p->nextarc; // p指向顶点v的下一个邻接点
        }
    }
    cout<<endl;
    printBFSTree();
}

int show()
{
    cout << "Enter 1:创建图" << endl;
    cout << "Enter 2:输出深度优先遍历序列" << endl;
    cout << "Enter 3:输出深度优先遍历的边集" << endl;
    cout << "Enter 4:输出广度优先遍历序列" << endl;
    cout << "Enter 5:输出广度优先遍历的边集" << endl;
    cout << "Enter 6:输出深度优先生成树" << endl;
    cout << "Enter 7:输出广度优先生成树" << endl;
    cout << "Enter 0:Exit" << endl;
    cout << "Please enter your choice:";
    int choice;
    cin >> choice;
    return choice;
}


int main()
{
    int source;
    ALGraph G;
    while (1)
    {
        switch (show())
        {
        case 1:
            CreateGraph(G);
            break;
        case 2:
            cout << "Please enter the source point" << endl;
            cin >> source;
            source=source-1;
            DFS(G, source);
            break;
        case 3:
            cout << "Please enter the source point" << endl;
            cin >> source;
            source=source-1;
            visited.assign(visited.size(), 0);
            DFSB(G, source);
            break;
        case 4:
            cout << "Please enter the source point" << endl;
            cin >> source;
            source=source-1;
            BFS(G, source);
            break;
        case 5:
            cout << "Please enter the source point" << endl;
            cin >> source;
            source=source-1;
            BFSB(G, source);
            break;
        case 6:
            cout << "Please enter the source point" << endl;
            cin >> source;
            source=source-1;
            visited.assign(visited.size(), 0);
            DFSTree(G, source);
            cout<<endl;
            printDFSTree();
            break;
        case 7:
            cout << "Please enter the source point" << endl;
            cin >> source;
            source=source-1;
            BFSTree(G, source);
            break;
        case 0:
            cout << "Enter any key to exit!" << endl;
            getchar();
            exit(0);
            break;
        default:
            cout << "Error" << endl;
            break;
        }
    }
    system("pause");
}