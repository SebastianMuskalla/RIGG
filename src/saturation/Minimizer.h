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

//
// Created by Sebastian on 15.07.2016.
//

#ifndef RIGG_MINIMIZER_H
#define RIGG_MINIMIZER_H

#include "../common/Alphabet.h"
#include "../common/NFA.h"

/**
 * given an DFA with no unreachable states (e.g. one produced by "Determinizer"), compute the (up to isomorphy) unique minimal DFA for the language of the given DFA, using Hopcroft's algorithm as specified by the following pseudo-code (taken from https://en.wikipedia.org/wiki/DFA_minimization)
 *
 * P := {F, Q \ F};
 * W := {F};
 * while (W is not empty) do
 *      choose and remove a set A from W
 *      for each c in Σ do
 *           let X be the set of states for which a transition on c leads to a state in A
 *           for each set Y in P for which X ∩ Y is nonempty and Y \ X is nonempty do
 *                replace Y in P by the two sets X ∩ Y and Y \ X
 *                if Y is in W
 *                     replace Y in W by the same two sets
 *                else
 *                     if |X ∩ Y| <= |Y \ X|
 *                          add X ∩ Y to W
 *                     else
 *                         add Y \ X to W
 *            end;
 *      end;
 * end;
 */
class Minimizer
{

public:
    /**
     * minimize the given DFA and return the minimal DFA
     *
     * it is the obligation of the caller to guarantee that the NFA is deterministic
     */
    static NFA* minimize (NFA* DFA);

    Minimizer (Minimizer const&) = delete;

    Minimizer& operator= (Minimizer const&) = delete;
};

#endif //RIGG_MINIMIZER_H
