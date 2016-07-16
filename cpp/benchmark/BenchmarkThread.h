//
// Created by solai on 16.07.2016.
//

#ifndef RIGG_BENCHMARKTHREAD_H
#define RIGG_BENCHMARKTHREAD_H


#include "BaseThread.h"
#include "Runnable.h"

class BenchmarkThread : public BaseThread
{
    Runnable* runnable;
    Event* e;


public:
    BenchmarkThread (const char* name, int priority, void* ref, int cpu, bool crrunning, unsigned int stacksize,
                     Runnable* runnable, Event* e) :
            BaseThread(name, priority, ref, cpu, crrunning, stacksize),
            runnable(runnable),
            e(e)
    { }


    virtual ~BenchmarkThread ()
    {
        delete runnable;
    }

    virtual unsigned int Execute (void* ref /*!<  ref */)
    {
        // measure time here
        return runnable->execute(e);
    }
};


#endif //RIGG_BENCHMARKTHREAD_H
