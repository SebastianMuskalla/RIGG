//
// Created by solai on 16.07.2016.
//

#include "BenchmarkThread.h"
#include <c++/4.8.3/chrono>

BenchmarkThread::BenchmarkThread (const char* name, int priority, void* reference, int cpu_affinity, bool running,
                                  unsigned int stacksize,
                                  Runnable* runnable, Event* e) :
        BaseThread(name, priority, reference, cpu_affinity, running, stacksize),
        runnable(runnable),
        e(e)
{
}

unsigned int BenchmarkThread::Execute (void* ref)
{
    auto start = chrono::high_resolution_clock::now();
    auto res = runnable->execute(e);
    auto end = chrono::high_resolution_clock::now();
    time = chrono::duration_cast<chrono::microseconds>(end - start).count();
    return res;
}


