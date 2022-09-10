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

#ifndef RIGG_DETERMINIZER_H
#define RIGG_DETERMINIZER_H

#include <set>
#include <algorithm>
#include "../common/NFA.h"

using namespace std;

/**
 * given an NFA A, it computes a DFA (unique and complete) that has the same language
 *
 * the resulting DFA may not be minimal, and it is recommended to minimize it before using it
 *
 * the resulting DFA will not have unreachable states, since the powersets are explored starting from the set {q_0} only containing the initial state q_0 of A
 */
class Determinizer
{
    /**
     * helper function that computes a string representation for a set
     */
    static string setToString (const set<Letter*>& set);

public:
    /**
     * determinize and return the result
     */
    static NFA* determinize (NFA* A);

    Determinizer (Determinizer const&) = delete;

    Determinizer& operator= (Determinizer const&) = delete;
};

#endif //RIGG_DETERMINIZER_H
