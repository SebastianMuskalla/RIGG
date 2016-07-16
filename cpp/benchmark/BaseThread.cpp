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
    unsigned int m_thread;
    unsigned int m_id;            //!< thread id for plattform
} HALTHREADID;
#endif


#if defined(__GNUCLINUX__)
typedef struct _HALTHREADID
{
    pthread_mutex_t m_startmutex;
    pthread_t m_thread;
} HALTHREADID;

static void* __Start_thread (void* obj)
{
    BaseThread* tobj = NULL;
    unsigned long stat;
    HALTHREADID* pthread;

    tobj = (BaseThread*) obj;

    pthread = (HALTHREADID*) tobj->imp_buf;

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);


    if (tobj)
    {
        // wait for start mutex
        pthread_mutex_lock(&pthread->m_startmutex);


        tobj->m_activ = true;
        stat = tobj->ExecThread(tobj->m_ref);
        tobj->m_activ = false;
        return (void*) stat;
    }
    return NULL;
}

#endif


#if defined(__MINGW32__)

static unsigned int __stdcall __Start_thread (void* obj)
{
    BaseThread* tobj = NULL;
    unsigned int stat;

    tobj = (BaseThread*) obj;

    if (tobj)
    {
        tobj->m_activ = true;
        stat = tobj->ExecThread(tobj->m_ref);
        tobj->m_activ = false;
        return stat;
    }
    return 27;
}

#endif

BaseThread::BaseThread (const char* name, int priority, void* ref, int cpu, bool crrunning,
                        unsigned int stacksize)
{
    HALTHREADID* pthread;

    m_priority = priority;
    m_ref = ref;
    m_activ = false;
    m_cpu = cpu;
    m_crrunnung = crrunning;

    //sys.registerthread(this);

    imp_buf_size = sizeof(HALTHREADID);
    imp_buf = (unsigned char*) malloc(imp_buf_size);
    if (imp_buf)
    {
        memset(imp_buf, 0, imp_buf_size);
        pthread = (HALTHREADID*) imp_buf;


#if defined(__MINGW32__)
        pthread->m_thread = _beginthreadex(NULL,                         // void *security,
                                           0,                            // unsigned stack_size,
                                           &__Start_thread,  //unsigned ( __stdcall *start_address )( void * ),
                                           (void*) this,
                                           CREATE_SUSPENDED,            // unsigned initflag,
                                           &pthread->m_id                          // unsigned * thrdaddr
        );
        if (priority != 0)
        {
            BOOL stat = SetThreadPriority((HANDLE) pthread->m_thread, priority);
        }
        if (pthread->m_id != NULL)
        {
            if (crrunning) ResumeThread((HANDLE) pthread->m_thread);
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
        pthread_create(&pthread->m_thread, &attr, &__Start_thread, (void*) this);

        pthread_attr_destroy(&attr);


        if (crrunning)
        {
            pthread_mutex_unlock(&pthread->m_startmutex);
        }
#endif
    }
}


void BaseThread::Resume ()
{
    HALTHREADID* pthread;

    pthread = (HALTHREADID*) imp_buf;

#if defined(__MINGW32__)
    if (pthread->m_id != NULL)
    {
        ResumeThread((HANDLE) pthread->m_thread);
    }
#endif
#if defined(__GNUCLINUX__)

    pthread_mutex_unlock(&pthread->m_startmutex);
#endif
}

BaseThread::~BaseThread ()
{

//    m_isdyn = false;
    if ((imp_buf != NULL) && (imp_buf_size != 0))
    {
        Terminate();

        imp_buf_size = 0;
        free(imp_buf);
        imp_buf = NULL;
    }
    m_activ = false;
    //sys.unregisterthread(this);
}

bool BaseThread::Terminate ()
{
    HALTHREADID* pthread;
    pthread = (HALTHREADID*) imp_buf;
    bool term = false;

    if ((imp_buf != NULL) && (imp_buf_size != 0))
    {
#if defined(__MINGW32__)
        term = (TerminateThread((HANDLE) pthread->m_thread, 0xFFFFFFFF) != 0);
#endif
#if defined(__GNUCLINUX__)
        if (pthread->m_thread)
        {
            pthread_mutex_destroy(&pthread->m_startmutex);

            pthread_cancel(pthread->m_thread);
            pthread_join(pthread->m_thread, NULL);
            memset(&pthread->m_thread, 0, sizeof(pthread->m_thread));
            term = true;
        }
#endif
    }
    if (term)
    {
        m_activ = false;
    }
    return term;
}


unsigned int BaseThread::ExecThread (void* ref)
{
    unsigned int stat;

    stat = Execute(ref);
    return stat;
}

