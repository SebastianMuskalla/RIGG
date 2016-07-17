//
// Created by solai on 16.07.2016.
//

#include "Event.h"
#include <windows.h>

Event::Event ()
{
#if defined(__MINGW32__)
    HANDLE* p_sema;
#endif
#if defined(__GNUCLINUX__)
    sem_t** p_sema;
#endif

    buffer_size = 0;
    buffer = NULL;

#if defined(__MINGW32__)
    buffer_size = sizeof(HANDLE);
#endif
#if defined(__GNUCLINUX__)
    buffer_size = sizeof(sem_t * );
#endif

    buffer = (unsigned char*) malloc(buffer_size);
    if (buffer)
    {
        memset(buffer, 0, buffer_size);


#if defined(__MINGW32__)
        p_sema = (HANDLE*) buffer;
        *p_sema = CreateSemaphore(NULL,  // LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
                                  0,     // LONG lInitialCount,
                                  0xFFFF,// LONG lMaximumCount,
                                  NULL);
#endif
#if defined(__GNUCLINUX__)
        p_sema = (sem_t**) buffer;
        /* Open a named semaphore NAME with open flags OFLAG.  */
        *p_sema = sem_open(&m_name[0], O_CREAT, ALLPERMS, 0);
        if (*p_sema == SEM_FAILED)
        {
            *p_sema = NULL;
        }

#endif
    }
    //sys.registerevent(this);
}


Event::~Event ()
{
#if defined(__MINGW32__)
    HANDLE* p_sema;
#endif
//#if defined(__GNUCLINUX__)
//    sem_t** p_sema;
//#endif


    if ((buffer != NULL) && (buffer_size != 0))
    {
#if defined(__MINGW32__)
        p_sema = (HANDLE*) buffer;
        CloseHandle(*p_sema);
        *p_sema = NULL;
#endif
//#if defined(__GNUCLINUX__)
//        p_sema = (sem_t**) buffer;
//        sem_close(*p_sema);
//#endif
        free(buffer);
        buffer = NULL;
        buffer_size = 0;
    }
}

void Event::signal ()
{
#if defined(__MINGW32__)
    HANDLE* p_sema;
#endif
//#if defined(__GNUCLINUX__)
//    sem_t** p_sema;
//#endif
#if defined(__MINGW32__)
    p_sema = (HANDLE*) buffer;
    ReleaseSemaphore(*p_sema,   // handle to semaphore
                     1,           // increase count by one
                     NULL);     // not interested in previous count
#endif
//#if defined(__GNUCLINUX__)
//    p_sema = (sem_t**) buffer;
//    sem_post(*p_sema);
//#endif
}

unsigned int Event::wait ()
{
#if defined(__MINGW32__)
    HANDLE* p_sema;
#endif
//#if defined(__GNUCLINUX__)
//    sem_t** p_sema;
//#endif

#if defined(__MINGW32__)
    p_sema = (HANDLE*) buffer;
    if (WaitForSingleObject(*p_sema, (DWORD) INFINITE) != WAIT_OBJECT_0)
    {
        return 0xFFFFFFFF;
    }
    return 0;
#endif
//#if defined(__GNUCLINUX__)
//    p_sema = (sem_t**) buffer;
//    sem_wait(*p_sema);
//#endif
    return 0;
}

unsigned int Event::wait (unsigned long long max_wait_milliseconds)
{
#if defined(__MINGW32__)
    HANDLE* p_sema;
#endif
//#if defined(__GNUCLINUX__)
//    sem_t** p_sema;
//#endif

#if defined(__MINGW32__)
    p_sema = (HANDLE*) buffer;
    if (WaitForSingleObject(*p_sema, (DWORD) (max_wait_milliseconds)) != WAIT_OBJECT_0)
    {
        return 0xFFFFFFFF;
    }
    return 0;
#endif
//#if defined(__GNUCLINUX__)
//    struct timespec tw;
//    unsigned long nsw;
//
//    p_sema = (sem_t**) buffer;
//
//    clock_gettime(CLOCK_REALTIME, &tw);
//    tw.tv_sec += (maxtns / 1000000000);
//    nsw = (maxtns % 1000000000) * 1000000; // ns
//    tw.tv_nsec += nsw;
//    if (tw.tv_nsec >= 1000000000)
//    {
//        tw.tv_nsec -= 1000000000;
//        tw.tv_sec += 1;
//    }
//    if (sem_timedwait(*p_sema, &tw) == 0)
//    {
//        return 0;
//    }
//    return 0xFFFFFFFF;
//#endif
}
