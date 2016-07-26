#ifndef RIGG_NFA_H
#define RIGG_NFA_H

class Box;

#include <set>
#include <unordered_map>
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
    friend class Box;

private:
    set<Box*> all_boxes;

public:
    /**
     * We do not delete the input alphabet!
     */
    virtual ~NFA ();

/**
     * input alphabet
     */
    Alphabet* Sigma;
    /**
     * control states
     */
    Alphabet* Q;
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
    unordered_map<Letter*, Box*> box_for_letter;

    /**
     * construct automaton with given input alphabet, control states, initial state and final states
     *
     * transitions can be added later
     */
    NFA (Alphabet* Sigma, Alphabet* Q, Letter* initial_state, set<Letter*> final_states);

    /**
     * construct a given transition with specified source, label and target, add it to the NFA and return a pointer to it
     */
    Transition* addTransition (Letter* source, Letter* label, Letter* target);

    /**
     * as "addTransition", but check whether transition already exists
     *
     * return true if transition was added successfully
     *
     * return false if transition already existed and nothing has been done
     */
    bool tryAddTransition (Letter* source, Letter* label, Letter* target);

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

    NFA (NFA const &) = delete;

    NFA &operator= (NFA const &) = delete;

    void resetBoxes ();
};

#endif //RIGG_NFA_H
