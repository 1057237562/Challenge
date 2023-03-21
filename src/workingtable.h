#ifndef _WT_H_
#define _WT_H_


#include "scheduler.h"

class WorkingTable
{
    friend class Scheduler;

private:

public:
    int type;
    Pos position;
    int remainTime;
    int resourceState;
    bool productState;
};


#endif