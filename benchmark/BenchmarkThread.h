//
// Created by solai on 16.07.2016.
//

#ifndef RIGG_BENCHMARKTHREAD_H
#define RIGG_BENCHMARKTHREAD_H


#include "BaseThread.h"
#include "Runnable.h"

using namespace std;

class BenchmarkThread : public BaseThread
{
    Runnable* runnable;
    Event* e;
    long long int time;


public:
    BenchmarkThread (const char* name, int priority, void* ref, int cpu, bool crrunning, unsigned int stacksize,
                     Runnable* runnable, Event* e);


    virtual ~BenchmarkThread ()
    {
        if (runnable)
        {
            delete runnable;
        }
    }

    virtual unsigned int Execute (void* ref /*!<  ref */);

    long long int getTime ()
    {
        return time;
    }
};


#endif //RIGG_BENCHMARKTHREAD_H
