//
// Created by solai on 16.07.2016.
//

#ifndef RIGG_EVENT_H
#define RIGG_EVENT_H


#include <c++/4.8.3/cstdlib>

class Event
{
public:

/*!
 A constructor. */
    Event ();

/*!
  A destructor. */
    virtual ~Event ();

/*! wait() waits for the event
     */
    virtual unsigned int wait ();

/*! signal() releases for the event ( sem=sem+1 )
     */
    virtual void signal ();

/*! wait(timems) releases for the event ( sem=sem+1 )
    \param maxtms - max wait time in ms
    \return 0 on succes,!=0 on error  */
    virtual unsigned int wait (unsigned long long max_wait_milliseconds);

protected:
    size_t imp_buf_size;    //!<  size of os object bufferspace
    unsigned char* imp_buf;        //!<  buffer space for os object
};

#endif //RIGG_EVENT_H
