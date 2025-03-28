#include "sync.h"
#include "asm_utils.h"
#include "stdio.h"
#include "os_modules.h"
#include "program.h"

SpinLock::SpinLock()
{
    initialize();
}

void SpinLock::initialize()
{
    bolt = 0;
}

void SpinLock::lock()
{
    uint32 key = 1;

    do
    {
        asm_atomic_exchange(&key, &bolt);
        //printf("pid: %d\n", programManager.running->pid);
    } while (key);
}

void SpinLock::unlock()
{
    bolt = 0;
}

Semaphore::Semaphore()
{
    initialize(0);
}

void Semaphore::initialize(uint32 counter)
{
    this->counter = counter;
    semLock.initialize();
    waiting.initialize();
}

void Semaphore::P()
{
    PCB *cur = nullptr;

    while (true)
    {
        semLock.lock();
        if (counter > 0)
        {
            --counter;
            semLock.unlock();
            return;
        }

        cur = programManager.running;
        waiting.push_back(&(cur->tagInGeneralList));
        cur->status = ProgramStatus::BLOCKED;

        semLock.unlock();
        programManager.schedule();
    }
}

void Semaphore::V()
{
    semLock.lock();
    ++counter;
    if (waiting.size())
    {
        PCB *program = ListItem2PCB(waiting.front(), tagInGeneralList);
        waiting.pop_front();
        semLock.unlock();
        programManager.MESA_WakeUp(program);
    }
    else
    {
        semLock.unlock();
    }
}

Monitor::Monitor()
{
  for(int id = 0; id < NUM+1; id++)
    {
      state[id] = 2;
      chopsticks[id].initialize(1);
    }
}

void Monitor::pickup(int id)
{
    state[id] = 0;
    int left=(id+4)%NUM;
    int right=(id+1)%NUM;
    if((state[left] != 1) &&(state[id] == 0) &&(state[right] != 1))
    {
        state[id] = 1;
        chopsticks[id].V();
    }
    if(state[id] != 1) chopsticks[id].P();
}

void Monitor::putdown(int id)
{
    state[id] = 2;
    for (int i = 0; i < NUM; i++) 
    {
        int now = (id + i) % NUM;
        int right = (id + i + 1) % NUM;
        int left=(now+NUM-1)%NUM;
        if (state[now] == 0 && state[right] != 1 && state[left] != 1) 
        {
            state[now] = 1;
            chopsticks[now].V();
            break;
        }
    }
}


