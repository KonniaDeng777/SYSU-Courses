#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum 
{
    CREATED,
    RUNNING,
    READY,
    BLOCKED,
    DEAD,
} State;

typedef struct 
{
    int id;
    int priority;//优先级越高，数字越小
    State state;
} PCB;

typedef struct 
{
    void (*Run)(void*);
    PCB pcb;
} Thread;

Thread* threads[3];
int num_threads = 0;

void thread_start(Thread* t) 
{
    t->pcb.state = RUNNING;
    t->Run(t);
    t->pcb.state = DEAD;
}

void thread_create(Thread* t, void (*Run)(void*), int id, int priority) 
{
    t->Run = Run;
    t->pcb.id = id;
    t->pcb.priority = priority;
    t->pcb.state = CREATED;
    threads[num_threads] = t;
    num_threads++;
}

void thread_start_execute(Thread* t) 
{
    t->pcb.state = READY;
    thread_start(t);
}


void thread_dead(Thread* t) 
{
    t->pcb.state = DEAD;
}

void my_thread_run(void* arg) 
{
    Thread* t = (Thread*) arg;
    printf("Thread %d is running\n", t->pcb.id);
    printf("Thread %d is finished\n", t->pcb.id);
    thread_dead(t);
}

void sort_threads() 
{
    int i, j;
    Thread* temp;
    for (i = 0; i < num_threads - 1; i++) 
    {
        for (j = i + 1; j < num_threads; j++) 
        {
            if (threads[i]->pcb.priority > threads[j]->pcb.priority) 
            {
                temp = threads[i];
                threads[i] = threads[j];
                threads[j] = temp;
            }
        }
    }
}

Thread* get_next_thread() 
{
    int i;
    Thread* next = NULL;
    for (i = 0; i < num_threads; i++) 
    {
        if (threads[i]->pcb.state == CREATED) 
        {
            next = threads[i];
            break;
        }
    }
    return next;
}

int main() 
{
    Thread t1, t2, t3;
    thread_create(&t1, my_thread_run, 1, 3);
    thread_create(&t2, my_thread_run, 2, 2);
    thread_create(&t3, my_thread_run, 3, 1);
    while (1) 
    {
        sort_threads();
        Thread* next = get_next_thread();
        if (next) 
        {
            thread_start_execute(next);
        } 
        else 
        {
            break;
        }
    }
    return 0;
}