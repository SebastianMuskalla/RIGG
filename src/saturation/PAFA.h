/*
 * Copyright 2016-2022 Sebastian Muskalla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
 * it has one "initial state" for each control state of P, accessible by the map pdsStateToAFAState
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
    Alphabet* controlStates;
    map<Letter*, Letter*> pdsStateToAFAState;
    set<Letter*> finalStates;
    set<AFATransition*> transitions;

    /**
     * construct empty PAFA for a given pushdown system
     */
    explicit PAFA (GamePDS* P);

    /**
     * compute all sets (conjunctions) S such that
     * controlState -- word --> S in the AFA
     */
    set<set<Letter*>> reachableFromControlState (Letter* controlState, const vector<Letter*>& word);

    /**
     * check whether a word is accepted by the AFA from a given control state (usually will be a control state of P)
     *
     * check whether there is a set S such that
     * controlState -- word --> S
     * and S only consists of final states
     *
     * could be implemented using reachableFromControlState, but we use a backtracking approach here, starting from the set of all final states
     */
    bool acceptsFromControlState (Letter* controlState, vector<Letter*> word);

    /**
     * string representation
     */
    string toString () const override;

    /**
     * tries to add a transition with the specified source, label, targets
     *
     * does nothing and returns false if the targets are empty or if an equal transition already exists
     *
     * returns true if the transition was successfully inserted
     */
    bool addTransition (Letter* source, Letter* label, const set<Letter*>& targets);

    /**
     * Gamma is not destroyed because it is owned by the pushdown system
     */
    virtual ~PAFA ();

    PAFA (PAFA const&) = delete;

    PAFA& operator= (PAFA const&) = delete;
};

#endif //RIGG_PAFA_H
