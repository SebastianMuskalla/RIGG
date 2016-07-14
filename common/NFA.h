#ifndef RIGG_NFA_H
#define RIGG_NFA_H

class Box;

#include <c++/4.8.3/set>
#include <c++/4.8.3/unordered_map>
#include "Alphabet.h"
#include "Letter.h"
#include "Transition.h"
#include "../dfa/Box.h"

using namespace std;

/**
 * non-deterministical finite automaton
 */
class NFA : public Printable<NFA>
{
public:
    /**
     * input alphabet
     */
    Alphabet* Sigma;
    /**
     * control states
     */
    Alphabet* states;
    /**
     * transitions
     */
    set<Transition*> transitions;

    /**
     * pointer to initial state
     */
    Letter* initial_state;
    /**
     * set of final states
     */
    set<Letter*> final_states;

    /**
     * memoize boxes to avoid unnecessary recomputations
     */
    unordered_map<Letter*, Box*> boxes;

    /**
     * construct automaton with given input alphabet, control states, initial state and final states
     *
     * transitions can be added later
     */
    NFA (Alphabet* Sigma, Alphabet* states, Letter* initial_state, set<Letter*> &final_states);

    /**
     * construct a given transition with specified origin, label and target, add it to the NFA and return a pointer to it
     */
    Transition* addTransition (Letter* origin, Letter* label, Letter* target);

    /**
     * as "addTransition", but check whether transition already exists
     *
     * return true if transition was added successfully
     *
     * return false if transition already existed and nothing has been done
     */
    bool tryAddTransition (Letter* origin, Letter* label, Letter* target);

    /**
     * add the given transition
     */
    void addTransition (Transition* t);

    /**
     * compute the box for the given letter form the input alphabet
     *
     * if the box has been computed before, it is taken from the map
     */
    Box* boxFor (Letter* a);

    /**
     * string representation
     */
    virtual string toString () const;
};

#endif //RIGG_NFA_H
