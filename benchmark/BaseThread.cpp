//
// Created by solai on 16.07.2016.
//

#include <stdio.h>
#include <windows.h>
#include <process.h>
#include "BaseThread.h"


#if defined(__MINGW32__)
typedef struct _HALTHREADID
{
    unsigned int thread;
    unsigned int id;            //!< thread id for plattform
} HALTHREADID;
#endif


//#if defined(__GNUCLINUX__)
//typedef struct _HALTHREADID
//{
//    pthread_mutex_t m_startmutex;
//    pthread_t thread;
//} HALTHREADID;
//
//static void* __Start_thread (void* obj)
//{
//    BaseThread* tobj = NULL;
//    unsigned long stat;
//    HALTHREADID* pthread;
//
//    tobj = (BaseThread*) obj;
//
//    pthread = (HALTHREADID*) tobj->buffer;
//
//    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
//    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
//
//
//    if (tobj)
//    {
//        // wait for start mutex
//        pthread_mutex_lock(&pthread->m_startmutex);
//
//
//        tobj->active = true;
//        stat = tobj->ExecThread(tobj->reference);
//        tobj->active = false;
//        return (void*) stat;
//    }
//    return NULL;
//}
//
//#endif


#if defined(__MINGW32__)

static unsigned int __stdcall __Start_thread (void* obj)
{
    BaseThread* tobj = NULL;
    unsigned int stat;

    tobj = (BaseThread*) obj;

    if (tobj)
    {
        tobj->active = true;
        stat = tobj->ExecThread(tobj->reference);
        tobj->active = false;
        return stat;
    }
    return 27;
}

#endif

BaseThread::BaseThread (const char* name, int priority, void* reference, int cpu_affinity, bool running,
                        unsigned int stacksize)
        :
        priority(priority),
        reference(reference),
        active(false),
        cpu_affinity(cpu_affinity),
        running(running)
{
    HALTHREADID* pthread;


    buffer_size = sizeof(HALTHREADID);
    buffer = (unsigned char*) malloc(buffer_size);
    if (buffer)
    {
        memset(buffer, 0, buffer_size);
        pthread = (HALTHREADID*) buffer;


#if defined(__MINGW32__)
        pthread->thread = _beginthreadex(NULL,                         // void *security,
                                         0,                            // unsigned stack_size,
                                         &__Start_thread,  //unsigned ( __stdcall *start_address )( void * ),
                                         (void*) this,
                                         CREATE_SUSPENDED,            // unsigned initflag,
                                         &pthread->id                          // unsigned * thrdaddr
        );
        if (priority != 0)
        {
            BOOL stat = SetThreadPriority((HANDLE) pthread->thread, priority);
        }
        if (pthread->id != NULL)
        {
            if (running)
                ResumeThread((HANDLE) pthread->thread);
        }
        else
        {
            ::printf("thread creation failed!");
        }
#endif


#if defined(__GNUCLINUX__)
        pthread_mutex_init(&pthread->m_startmutex, NULL);
        pthread_mutex_lock(&pthread->m_startmutex);


        pthread_attr_t attr;
        sched_param schparam;

        pthread_attr_init(&attr);

        pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

        schparam.sched_priority = 1;
        if (priority > 1)
        {
            schparam.sched_priority = priority;
        }
        pthread_attr_setschedparam(&attr, &schparam);
        if (stacksize == 0)
        {
            stacksize = 128 * 1024;
        }
        if (stacksize != 0)
        {
            pthread_attr_setstacksize(&attr, (size_t) stacksize);
        }
        pthread_create(&pthread->thread, &attr, &__Start_thread, (void*) this);

        pthread_attr_destroy(&attr);


        if (running)
        {
            pthread_mutex_unlock(&pthread->m_startmutex);
        }
#endif
    }
}


void BaseThread::resume ()
{
    HALTHREADID* pthread;

    pthread = (HALTHREADID*) buffer;

#if defined(__MINGW32__)
    if (pthread->id != NULL)
    {
        ResumeThread((HANDLE) pthread->thread);
    }
#endif
//#if defined(__GNUCLINUX__)
//    pthread_mutex_unlock(&pthread->m_startmutex);
//#endif
}

BaseThread::~BaseThread ()
{

    if ((buffer != NULL) && (buffer_size != 0))
    {
        terminate();

        buffer_size = 0;
        free(buffer);
        buffer = NULL;
    }
    active = false;
}

bool BaseThread::terminate ()
{
    HALTHREADID* pthread;
    pthread = (HALTHREADID*) buffer;
    bool term = false;

    if ((buffer != NULL) && (buffer_size != 0))
    {
#if defined(__MINGW32__)
        term = (TerminateThread((HANDLE) pthread->thread, 0xFFFFFFFF) != 0);
#endif
//#if defined(__GNUCLINUX__)
//        if (pthread->thread)
//        {
//            pthread_mutex_destroy(&pthread->m_startmutex);
//
//            pthread_cancel(pthread->thread);
//            pthread_join(pthread->thread, NULL);
//            memset(&pthread->thread, 0, sizeof(pthread->thread));
//            term = true;
//        }
//#endif
    }
    if (term)
    {
        active = false;
    }
    return term;
}


unsigned int BaseThread::ExecThread (void* ref)
{
    unsigned int stat;

    stat = Execute(ref);
    return stat;
}

