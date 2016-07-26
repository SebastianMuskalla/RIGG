//
// Created by solai on 16.07.2016.
//

#ifndef RIGG_EVENT_H
#define RIGG_EVENT_H


#include <cstdlib>

/**
 * implements a mutex
 */
class Event
{
public:

    Event ();

    virtual ~Event ();

    /**
     * wait until the event has been triggered
     */
    virtual unsigned int wait ();


    /**
     * wait until the event has been trigger, or after the specified time has elapsed
     *
     * returns 0 on succes,!=0 on error
     */
    virtual unsigned int wait (unsigned long long max_wait_milliseconds);


    /**
     * triggers the event
     */
    virtual void signal ();

protected:
    size_t buffer_size;
    unsigned char* buffer;
};

#endif //RIGG_EVENT_H
