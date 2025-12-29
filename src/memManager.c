#include "memManager.h"


static void seqListFree(void** seqList)
{
    sequence_free((Sequences*)*seqList);
}


static void parListFree(void** parList)
{
    param_free((Parameters*)*parList);
}


static void keyListFree(void** keyList)
{
    keys_free((Keys*)*keyList);
}


static void sockClientListFree(void** sockClientList)
{
    socketClientListAll_free((SOCKETCLIENTLIST*)*sockClientList);
}


void memoryFree(void* mem)
{
    if(mem == NULL) return;
    
    free(mem);
}