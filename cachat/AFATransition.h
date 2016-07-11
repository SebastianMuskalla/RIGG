#ifndef RIGG_AFATRANSITION_H
#define RIGG_AFATRANSITION_H


#include <c++/4.8.3/set>
#include "../common/Letter.h"

using namespace std;

class AFATransition : public Printable<AFATransition>
{
public:
    Letter* label;
    Letter* origin;
    /**
     * Each set {q_1, ..., q_n} represents the conjunction q_1 and ... and q_n.
     * Disjunctions are captured by nondeterminism.
     */
    set<Letter*> targets;


    AFATransition (Letter* origin, Letter* label, const set<Letter*> &targets) :
            label(label),
            origin(origin),
            targets(targets)
    { }

    virtual string toString () const;

    bool operator== (const AFATransition &other) const
    {
        return label == other.label && origin == other.origin && targets == other.targets;
    }
};


#endif //RIGG_AFATRANSITION_H
