//
// Created by solai on 16.07.2016.
//


#ifndef RIGG_RUNNABLE_H
#define RIGG_RUNNABLE_H

class Event;

class Runnable
{

public:
    virtual unsigned int execute (Event* e) = 0;
};


#endif //RIGG_RUNNABLE_H
