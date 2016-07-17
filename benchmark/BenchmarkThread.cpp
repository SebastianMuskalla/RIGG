//
// Created by solai on 16.07.2016.
//

#include "BenchmarkThread.h"
#include <c++/4.8.3/chrono>

unsigned int BenchmarkThread::Execute (void* ref /*!<  ref */)
{
    auto start = chrono::high_resolution_clock::now();
    auto res = runnable->execute(e);
    auto end = chrono::high_resolution_clock::now();
    time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return res;
}

BenchmarkThread::BenchmarkThread (const char* name, int priority, void* ref, int cpu, bool crrunning,
                                  unsigned int stacksize,
                                  Runnable* runnable, Event* e) :
        BaseThread(name, priority, ref, cpu, crrunning, stacksize),
        runnable(runnable),
        e(e)
{ }
