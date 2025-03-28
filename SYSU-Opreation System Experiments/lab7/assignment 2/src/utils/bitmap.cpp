#include "bitmap.h"
#include "stdlib.h"
#include "stdio.h"

BitMap::BitMap()
{
}

void BitMap::initialize(char *bitmap, const int length)
{
  this->bitmap = bitmap;
  this->length = length;
  int bytes = ceil(length, 8);
  for (int i = 0; i < bytes; ++i)
  {
    bitmap[i] = 0;
  }
  for(int i = 0; i < length; i++)
  {
    MemoryTypeList[i] = UNALLOCATED;
  }
  const int UsedBlockSize = 3546;
  for(int i = 0; i < UsedBlockSize; i++)
  {
    set(200+i, 1);
    set(4346+i, 1);     
    set(8392+i, 1);
    set(12238+i, 1);
    MemoryTypeList[200+i] = USED;      
    MemoryTypeList[4346+i] = USED;      
    MemoryTypeList[8392+i] = USED;
    MemoryTypeList[12238+i] = USED;      
  }

}
void BitMap::PrintMemory(MemoryType MemType, int startAddress, int endAddress)
{
  switch(MemType)
  {
    case USED:
      //printf("Total Space: ");
      //break;
      return;
    case UNALLOCATED:
      printf("Available  : ");
      break;
    case P1:
      printf("P1         : ");
      break;
    case P2: 
      printf("P2         : ");
      break;
    case P3:
      printf("P3         : ");
      break;
    case P4:
      printf("P4         : ");      
      break;
  }
  int BlockByteSize =  (endAddress - startAddress + 1)*4;
  printf("%d KB\n", BlockByteSize);
}
void BitMap::PrintSpace()
{
  printf("\n");
  MemoryType CurrentType = MemoryTypeList[0];
  int MemoryTypeStartAddress = 0;
  int i = 0;
  for(; i < length; i++)
  {
    if(MemoryTypeList[i] == CurrentType) continue;
    else
    {
      PrintMemory(CurrentType, MemoryTypeStartAddress, i-1);
      MemoryTypeStartAddress = i;
      CurrentType = MemoryTypeList[i];
    }	
  }
  PrintMemory(CurrentType, MemoryTypeStartAddress, i-1);
}

bool BitMap::get(const int index) const
{
    int pos = index / 8;
    int offset = index % 8;

    return (bitmap[pos] & (1 << offset));
}

void BitMap::set(const int index, const bool status)
{
    int pos = index / 8;
    int offset = index % 8;

    // 清0
    bitmap[pos] = bitmap[pos] & (~(1 << offset));

    // 置1
    if (status)
    {
        bitmap[pos] = bitmap[pos] | (1 << offset);
    }
}

int BitMap::allocate(const int count)
{
    if (count == 0)
        return -1;

    int index, empty, start;

    index = 0;
    while (index < length)
    {
        // 越过已经分配的资源
        while (index < length && get(index))
            ++index;

        // 不存在连续的count个资源
        if (index == length)
            return -1;

        // 找到1个未分配的资源
        // 检查是否存在从index开始的连续count个资源
        empty = 0;
        start = index;
        while ((index < length) && (!get(index)) && (empty < count))
        {
            ++empty;
            ++index;
        }

        // 存在连续的count个资源
        if (empty == count)
        {
            for (int i = 0; i < count; ++i)
            {
                set(start + i, true);
            }

            return start;
        }
    }

    return -1;
}


void BitMap::FirstFit(int ProcessSize[4])
{
  MemoryType ProcessList[4] = {P1, P2, P3, P4};
  for(int i = 0; i < 4; i++)
  {
    int start = allocate(ProcessSize[i]/4);
    if(start != -1)
    {
      for(int j = 0; j < ProcessSize[i]/4; j++)
      {
        set(start+j, 1);
	      MemoryTypeList[start + j] = ProcessList[i];
      }
    }
  }
}
const int UserPages = 15984;

int BitMap::BestFitAddress(int ProcessPages)
{
  int BestFitStartAddress;
  int TotalUnallocated = UserPages;
  int StartAddress;
  int unallocated = 0;
  int Flag = 0;
  for(int i = 0; i < UserPages; i++)
  { 
    if(get(i) == 0 && !Flag) 
    {
      Flag = 1;
      StartAddress = i;
      unallocated++;
    }
    if(get(i) == 0 && Flag)
      unallocated++;
    
    if((get(i) == 1  || i == 15983) && Flag)
    {
      if(ProcessPages < unallocated && unallocated < TotalUnallocated)
      {
	      TotalUnallocated = unallocated;
	      BestFitStartAddress = StartAddress;	
      }
      unallocated = 0;      
      Flag = 0;
    }
    else continue;

  }
  return TotalUnallocated == UserPages  ? -1 : BestFitStartAddress;
}
void BitMap::BestFit(int ProcessSize[4])
{
  MemoryType ProcessList[4] = {P1, P2, P3, P4};
  for(int i = 0; i < 4; i++)
  {
    int start = BestFitAddress(ProcessSize[i]/4); 
    if(start != -1)
    {
      for(int j = 0; j < ProcessSize[i]/4; j++)
      {
	      set(start+j, 1);
	      MemoryTypeList[start + j] = ProcessList[i];
      }
    }
  }
}
void BitMap::release(const int index, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        set(index + i, false);
    }
}

char *BitMap::getBitmap()
{
    return (char *)bitmap;
}

int BitMap::size() const
{
    return length;
}
