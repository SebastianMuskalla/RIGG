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

#ifndef RIGG_BOX_H
#define RIGG_BOX_H

class NFA;

class Clause;

#include "../common/Letter.h"
#include "../common/NFA.h"
#include "Clause.h"
#include <map>
#include <algorithm>
#include "../common/Printable.h"
#include "Ternary.h"

using namespace std;

/**
 * box, i.e. set of state-pairs in the automaton
 */
class Box : public Printable<Box>
{
    friend class NFA;

private:
/**
 * memoization: values will be computed on first usage
 */
    Ternary rejecting = UNDEFINED;

    Box (NFA* A, Alphabet* Q, string name);

public:

    /**
     * automaton for which the box is
     */
    NFA* A;
    /**
     * state set of the automaton
     */
    Alphabet* Q;
    /**
     * name of the box.
     *
     * usually comes from the word that was used to generate the box
     *
     * name is ignored when checking for equality, because different words might induce the same box (behavior on the automaton)
     */
    string name;

    /**
     * state-pairs forming the box
     */
    multimap<Letter*, Letter*> content;

    /**
     * compose two boxes according to relational composition
     */
    Box* composeWith (Box* other);

    /**
     * compose a box with a whole clause, which results in a list of boxes
     */
    vector<Box*> composeWith (Clause* r);

    /**
     * string representation
     */
    string toString () const override;

    bool isRejecting ();

    /**
     * check boxes for equality of the content (ignores the name)
     */
    bool operator== (const Box& other) const
    {
        return content == other.content;
    }

    Box (Box const&) = delete;

    Box& operator= (Box const&) = delete;
};

#endif //RIGG_BOX_H
