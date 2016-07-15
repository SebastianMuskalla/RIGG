#ifndef RIGG_TRANSITION_H
#define RIGG_TRANSITION_H

#include "Letter.h"

using namespace std;

/**
 * represents transition of an NFA
 */
class Transition : public Printable<Transition>
{

public:
    Transition (Letter* origin, Letter* label, Letter* target) :
            source(origin),
            label(label),
            target(target)
    { }

    Letter* label;
    Letter* source;
    Letter* target;

    virtual string toString () const;

    bool operator== (const Transition &other) const
    {
        return label == other.label && source == other.source && target == other.target;
    }
};

#endif //RIGG_TRANSITION_H
