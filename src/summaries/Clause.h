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

#ifndef RIGG_CLAUSE_H
#define RIGG_CLAUSE_H

class Formula;

#include "Box.h"
#include "Formula.h"
#include "Ternary.h"

using namespace std;

/**
 * clause, i.e. sets of boxes
 */
class Clause : public Printable<Clause>
{
private:
    /**
     * memoization: values will be computed on first usage
     */
    Ternary rejecting = UNDEFINED;

public:

    Clause ()
    = default;

    /**
     * We do not delete the boxes in the clause, because the same object might be used in other clauses
     */
    virtual ~Clause ()
    = default;

/**
     * list of boxes of which the clause consists
     */
    vector<Box*> boxes;

    /**
     * computes the list of clauses that we get as the composition of the current clause with a whole formula
     */
    vector<Clause*> composeWith (Formula* G);

    /**
     * string representation
     */
    string toString () const override;

    /**
     * returns true iff the clause is rejecting, i.e. it contains a rejecting box
     *
     * Memoization is used: upon the first call, rejecting will be initialized, afterwards, it will just be returned.
     *
     * The internal structure of the formula should not be modified after this function has been called, otherwise strange things will happen
     */
    bool isRejecting ();

    /**
     * Checks whether this clause contains every box occurring in c
     */
    bool contains (Clause* c);

    /**
     * given a box b, returns the singleton clause {b}
     */
    static Clause* wrap (Box* b);

    Clause (Clause const&) = default;

    Clause& operator= (Clause const&) = default;

private:
    /**
     * Given an array of uint64s of length k with entries in 0...g, tries to increment position start
     *
     * If overflow is triggered, go to the next position and reset later position
     *
     * returns the next position on which the next increment should happen afterwards or k if no further increment is possible
     */
    static unsigned long long int
    increment (unsigned long long int* func, unsigned long long int k, unsigned long long int g,
               unsigned long long int start);
};

#endif //RIGG_CLAUSE_H
