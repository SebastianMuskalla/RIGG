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

#ifndef RIGG_NFA_H
#define RIGG_NFA_H

class Box;

#include <set>
#include <unordered_map>
#include "Alphabet.h"
#include "Letter.h"
#include "Transition.h"
#include "../summaries/Box.h"

using namespace std;

/**
 * non-deterministic finite automaton
 */
class NFA : public Printable<NFA>
{
    friend class Box;

private:
    set<Box*> allBoxes;

public:
    /**
     * we do not delete the input alphabet!
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
    Letter* initialState;
    /**
     * set of final states
     */
    set<Letter*> finalStates;

    /**
     * memoize boxes to avoid unnecessary recomputations
     */
    unordered_map<Letter*, Box*> boxForLetter;

    /**
     * construct automaton with given input alphabet, control states, initial state and final states
     *
     * transitions can be added later
     */
    NFA (Alphabet* Sigma, Alphabet* Q, Letter* initialState, set<Letter*> finalStates);

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
    string toString () const override;

    NFA (NFA const&) = delete;

    NFA& operator= (NFA const&) = delete;

    void resetBoxes ();

    Box* identityBox ();
};

#endif //RIGG_NFA_H
