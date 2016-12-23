#ifndef RIGG_PAFA_H
#define RIGG_PAFA_H

#include <map>
#include <algorithm>
#include "GamePDS.h"
#include "../common/Alphabet.h"
#include "AFATransition.h"

using namespace std;

/**
 * alternating automaton representing a regular set of configurations of a (game) pushdown system P
 *
 * it has one "initial state" for each control state of P, accessiable by the map pds_state_to_afa_state
 *
 * its input alphabet coincides with the stack alphabet of P
 */
class PAFA : public Printable<PAFA>
{
public:
    /**
     * input alphabet = stack alphabet of P
     */
    Alphabet* Gamma;
    GamePDS* P;
    Alphabet* control_states;
    map<Letter*, Letter*> pds_state_to_afa_state;
    set<Letter*> final_states;
    set<AFATransition*> transitions;

    /**
     * construct empty PAFA for a given pushdown system
     */
    PAFA (GamePDS* P);

    /**
     * compute all sets (conjunctions) S such that
     * control_state -- word --> S in the AFA
     */
    set<set<Letter*>> reachableFromControlState (Letter* control_state, vector<Letter*> word);

    /**
     * check whether a word is accepted by the AFA from a given control state (usually will be a control state of P)
     *
     * check whether there is a set S such that
     * control_state -- word --> S
     * and S only consists of final states
     *
     * could be implemented using reachableFromControlState, but we use a backtracking approach here, starting from the set of all final states
     */
    bool acceptsFromControlState (Letter* control_state, vector<Letter*> word);

    /**
     * string representation
     */
    virtual string toString () const;

    /**
     * tries to add a transition with the specified source, label, targets
     *
     * does nothing and returns false if the targets are empty or if an equal transition already exists
     *
     * returns true if the transition was successfully inserted
     */
    bool addTransition (Letter* source, Letter* label, set<Letter*> targets);

    /**
     * Gamma is not destroyed because it is owned by the pushdown system
     */
    virtual ~PAFA ();

    PAFA (PAFA const &) = delete;

    PAFA &operator= (PAFA const &) = delete;
};

#endif //RIGG_PAFA_H
