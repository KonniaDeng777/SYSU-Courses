#include "sync.h"
#include "asm_utils.h"
#include "stdio.h"
#include "os_modules.h"


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
    while (key)
    {
        asm_atomic_exchange(&key, &bolt);
        if (!key)
        {
            asm_lock_bts(&bolt);
        }
    }
}

void SpinLock::unlock()
{
    bolt=0;
}