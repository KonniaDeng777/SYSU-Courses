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

const int NUM_PHILOSOPHERS = 5; // 哲学家的数量
int map(int x) 
{
    switch (x) 
    {
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        case 4:
            return 4;
        case 5:
            return 5;
        case 6:
            return 1;
        default:
            return -1; 
    }
}

Monitor monitor;

// void philosopher(void *arg)
// {
//     int id = (int)arg; // 哲学家的编号
//     monitor.pickup(id);
//     printf("Philosopher %d picked up chopsticks %d and %d\n",id+1,id+1,map(id+2));
//     // 进入临界区，模拟就餐
//     printf("Philosopher %d is eating\n", id+1);
//     monitor.putdown(id);
//     printf("Philosopher %d put down chopsticks %d and %d\n",id+1,id+1,map(id+2));
//     // 模拟思考
//     printf("Philosopher %d is thinking\n", id+1);
// }
void philosopher(void *arg)
{
    int id = (int)arg; // 哲学家的编号
    monitor.pickup(id); 
    // 模拟思考
    int delay=0xfffffff;
    while(delay)
        delay--;
    printf("Philosopher %d is thinking\n", id+1);
    printf("Philosopher %d picked up chopsticks %d and %d\n",id+1,id+1,map(id+2));
    // 进入临界区，模拟就餐
    printf("Philosopher %d is eating\n", id+1);
    monitor.putdown(id);
    printf("Philosopher %d put down chopsticks %d and %d\n",id+1,id+1,map(id+2));
}

void first_thread(void *arg)
{
    stdio.moveCursor(0);
    for (int i = 0; i < 25 * 80; ++i)
    {
        stdio.print(' ');
    }
    stdio.moveCursor(0);

    programManager.executeThread(philosopher, (void *)0, "philosopher thread", 1);
    programManager.executeThread(philosopher, (void *)1, "philosopher thread", 1);
    programManager.executeThread(philosopher, (void *)2, "philosopher thread", 1);
    programManager.executeThread(philosopher, (void *)3, "philosopher thread", 1);
    programManager.executeThread(philosopher, (void *)4, "philosopher thread", 1);

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