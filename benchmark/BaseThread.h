//
// Created by solai on 16.07.2016.
//

#ifndef RIGG_BASETHREAD_H
#define RIGG_BASETHREAD_H


#include <c++/4.8.3/cstdlib>

class BaseThread
{
public:
    BaseThread (const char* name,
                int priority,
                void* ref,
                int cpu_affinity = -1,
                bool running = true,
                unsigned int stacksize = 0
    );

    virtual ~BaseThread ();

    /**
     * resume a thread that was created with "running = false "
     */
    void resume ();

    /*! terminate() terminates a running thread */
    virtual bool terminate ();

    /**
     * purely virtual execute method
     */
    virtual unsigned int Execute (void* ref)
    {
        return 0;
    }

    /**
     * helper method for executing
     */
    unsigned int ExecThread (void* ref);

    /**
     * thread priority, in 0..99
     */
    int priority;
    /**
     * reference of the Runnable that should be executed
     */
    void* reference;
    /**
     * CPU affinity
     */
    int cpu_affinity;
    /**
     * thread active?
     */
    bool active;
    /**
     * should thread run directly after it was created
     */
    bool running;

protected:
    /**
     * size of buffersize for OS object
     */
    size_t buffer_size;
public:
    /**
     * buffer for OS object
     */
    unsigned char* buffer;
};


#endif //RIGG_BASETHREAD_H
