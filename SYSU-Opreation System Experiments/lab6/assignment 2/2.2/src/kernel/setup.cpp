#include "asm_utils.h"
#include "interrupt.h"
#include "stdio.h"
#include "program.h"
#include "thread.h"
#include "sync.h"

// 屏幕IO处理器
STDIO stdio;
// 中断管理器
InterruptManager interruptManager;
// 程序管理器
ProgramManager programManager;
Semaphore write;
Semaphore read;


#define BUFFER_SIZE 10
int buffer[10]={0}; // 共享缓冲区
int producer_index = 0;  // 生产者插入位置
int consumer_index = 0;  // 消费者读取位置
int count=0;


void consumer_read(void *arg)
{
    read.P();
    count++;
    if (count==1)
    {
        write.P();
    }
    read.V();
    printf("consumer read %d\n",buffer[consumer_index]);
    consumer_index = (consumer_index + 1) % BUFFER_SIZE;
    read.P();
    count--;
    if(count==0)
    {
        write.V();
    }
    read.V();
}
void producer_write(void *arg)
{
    write.P();
    buffer[producer_index] = producer_index+1;
    printf("producer write %d\n",producer_index+1);
    producer_index = (producer_index + 1) % BUFFER_SIZE;
    write.V();
}


void first_thread(void *arg)
{
    // 第1个线程不可以返回
    stdio.moveCursor(0);
    for (int i = 0; i < 25 * 80; ++i)
    {
        stdio.print(' ');
    }030712
    stdio.moveCursor(0);
    write.initialize(1);
    read.initialize(1);
    // 创建多个生产者和消费者线程
    programManager.executeThread(producer_write, nullptr, "second_thread", 1);
    programManager.executeThread(consumer_read, nullptr, "third_thread", 1);
    programManager.executeThread(producer_write,nullptr,"forth_thread",1);
    programManager.executeThread(producer_write,nullptr,"fifth_thread",1);
    programManager.executeThread(consumer_read, nullptr, "sixth_thread", 1);
    programManager.executeThread(producer_write,nullptr,"seventh_thread",1);
    programManager.executeThread(consumer_read, nullptr, "eighth_thread", 1);
    programManager.executeThread(consumer_read, nullptr, "ninth_thread", 1);

    asm_halt();
}

extern "C" void setup_kernel()
{

    // 中断管理器
    interruptManager.initialize();
    interruptManager.enableTimeInterrupt();
    interruptManager.setTimeInterrupt((void *)asm_time_interrupt_handler);

    // 输出管理器
    stdio.initialize();

    // 进程/线程管理器
    programManager.initialize();

    // 创建第一个线程
    int pid = programManager.executeThread(first_thread, nullptr, "first thread", 1);
    if (pid == -1)
    {
        printf("can not execute thread\n");
        asm_halt();
    }

    ListItem *item = programManager.readyPrograms.front();
    PCB *firstThread = ListItem2PCB(item, tagInGeneralList);
    firstThread->status = RUNNING;
    programManager.readyPrograms.pop_front();
    programManager.running = firstThread;
    asm_switch_thread(0, firstThread);

    asm_halt();
}