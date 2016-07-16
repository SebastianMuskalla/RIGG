#ifndef RIGG_AFATRANSITION_H
#define RIGG_AFATRANSITION_H

#include <c++/4.8.3/set>
#include "../common/Letter.h"

using namespace std;

/**
 * transition of an alternating finite automaton
 */
class AFATransition : public Printable<AFATransition>
{
public:
    Letter* label;
    Letter* source;
    /**
     * Each set {q_1, ..., q_n} represents the conjunction q_1 and ... and q_n.
     * Disjunctions are captured by nondeterminism.
     */
    set<Letter*> targets;

    AFATransition (Letter* source, Letter* label, const set<Letter*> &targets) :
            label(label),
            source(source),
            targets(targets)
    { }

    virtual string toString () const;

    bool operator== (const AFATransition &other) const
    {
        return label == other.label && source == other.source && targets == other.targets;
    }

    AFATransition (AFATransition const &) = delete;

    AFATransition &operator= (AFATransition const &) = delete;
};

#endif //RIGG_AFATRANSITION_H
