#ifndef RIGG_TRANSITION_H
#define RIGG_TRANSITION_H


#include "Letter.h"

using namespace std;

class Transition : public Printable<Transition>
{


public:
    Transition (Letter* origin, Letter* label, Letter* target) :
            origin(origin),
            label(label),
            target(target)
    { }

    Letter* label;
    Letter* origin;
    Letter* target;

    virtual string toString () const;
};


#endif //RIGG_TRANSITION_H
