//
// Created by solai on 16.07.2016.
//

#ifndef RIGG_BASETHREAD_H
#define RIGG_BASETHREAD_H


#include <c++/4.8.3/cstdlib>

class BaseThread
{
public:
    /*! constructor. */
    BaseThread (const char* name,            //!< name of the thread
                int priority,                //!< priority 0..99
                void* ref,                    //!< value will be pased to virtual Execute methode
                int cpu = -1,                //!< cpu affinity
                bool crrunning = true,        //!< if true thread will run after creation or hold if false
                unsigned int stacksize = 0    //!< stacksize of the thread ( 0= default stacksize for plattform
    );

    /*! destructor. */
    virtual ~BaseThread ();

    /*! Resume() resumes a thread created with crrunnung=false */
    void Resume ();

    /*! Terminate() terminates a running thread */
    virtual bool Terminate ();

    /*! pure virtual Execute() methode */
    virtual unsigned int Execute (void* ref /*!<  ref */
    )
    { return 0; }

    /*! helper methode for threadstart */
    unsigned int ExecThread (void* ref);

    int m_priority;    //!< priority member
    void* m_ref;        //!< ref for Execute
    int m_cpu;        //!< cpu affinity
    unsigned int m_activ;        //!< thread is active
    unsigned int m_crrunnung;    //!< thread marker run after create

protected:
    size_t imp_buf_size;    /*!<  size of os object bufferspace */
public:
    unsigned char* imp_buf;        /*!<  buffer space for os object */

};


#endif //RIGG_BASETHREAD_H
