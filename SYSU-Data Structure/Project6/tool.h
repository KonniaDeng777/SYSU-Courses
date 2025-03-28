#include<iostream>
using namespace std;

void printDFSTree()
{
    cout<<"        北京"<<endl;
    cout<<"        呼和浩特"<<endl;
    cout<<"        兰州"<<endl;
    cout<<"    西安西宁乌鲁木齐"<<endl;
    cout<<"    成都"<<endl;
    cout<<"    贵阳"<<endl;
    cout<<"昆明        柳州"<<endl;
    cout<<"        株洲    南宁"<<endl;
    cout<<"    广州    武汉"<<endl;
    cout<<"    深圳    郑州"<<endl;
    cout<<"            徐州"<<endl;
    cout<<"        天津     上海"<<endl;
    cout<<"        沈阳     南昌"<<endl;
    cout<<"    长春     大连福州"<<endl;
    cout<<"    哈尔滨"<<endl;
}

void printBFSTree()
{
    cout<<"                        北京"<<endl;
    cout<<"    天津                郑州                呼和浩特"<<endl;
    cout<<"    沈阳        徐州     武汉        西安 兰州西宁乌鲁木齐"<<endl;
    cout<<"长春    大连    上海     株洲        成都"<<endl;
    cout<<"哈尔滨               南昌广州柳州 昆明   贵阳"<<endl;
    cout<<"                     福州深圳南宁"<<endl;
}