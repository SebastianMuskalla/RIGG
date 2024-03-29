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

#ifndef RIGG_FORMULA_H
#define RIGG_FORMULA_H

#include "Clause.h"
#include "Ternary.h"
#include "FormulaStorage.h"

using namespace std;

/**
 * positive boolean formula over the box-domain in conjunctive normal form
 */
class Formula : public Printable<Formula>
{
private:
    /**
     * memoization: values will be computed on first usage
     */
    mutable Ternary rejecting;
    mutable Ternary unsatisfiable;

    FormulaStorage* storage;

    static unordered_map<FormulaStorage*, Formula*> FALSE_FORMULA;


public:


    explicit Formula (FormulaStorage* storage) :
            rejecting(UNDEFINED),
            unsatisfiable(UNDEFINED),
            storage(storage)
    {}

/**
     * list of clauses of the formula
     */
    vector<Clause*> clauses;

    /**
     * We should not delete the FALSE_FORMULA, since it is shared
     *
     * Clauses should not be shared between formulas, so we can delete them
     */
    virtual ~Formula ();

    /**
     * returns true iff the formulas is rejecting, i.e. every clause in it is rejecting
     *
     * Memoization is used: upon the first call, rejecting will be initialized, afterwards, it will just be returned.
     *
     * The internal structure of the formula should not be modified after this function has been called, otherwise strange things will happen
     *
     * after modifying the structure, resetMemoization has to be called
     */
    bool isRejecting ();

    /**
     * generate string representation
     */
    virtual string toString () const override;

    /**
     * F.composeWith(G) computes the composition F;G
     */
    Formula* composeWith (Formula* G);

    /**
     * F.implies(G) returns true iff F |= G
     */
    bool implies (Formula* G);

    /**
     * given a box b, returns the singleton formula {{b}}
     */
    static Formula* wrap (Box* b, FormulaStorage* storage);

    /**
     * given a clause c, returns the singleton formula {c}
     */
    static Formula* wrap (Clause* c, FormulaStorage* storage);

    /**
     * F.formulaAnd(G) computes F and G, which is just the union of the clauses
     */
    Formula* formulaAnd (Formula* other);

    /**
     * F.formulaOr(G) computes F or G, which is a formula whose clauses are unions of a clause of F and a clause of G
     */
    Formula* formulaOr (Formula* other);

    /**
     * returns true iff the formulas is logically equivalent to FALSE
     *
     * Memoization is used: upon the first call, rejecting will be initialized, afterwards, it will just be returned.
     *
     * The internal structure of the formula should not be modified after this function has been called, otherwise strange things will happen
     *
     * after modifying the structure, resetMemoization has to be called
     */
    bool isFalse () const;

    /**
     * returns the formula {{}} representing the constant false
     */
    static Formula* falseFormula (FormulaStorage* storage);

    /**
     * resets the values for rejecting and false to UNDEFINED
     */
    void resetMemoization ()
    {
        rejecting = UNDEFINED;
        unsatisfiable = UNDEFINED;
    }

    Formula* simplify ();

    Formula (Formula const&) = delete;

    Formula& operator= (Formula const&) = delete;
};

#endif //RIGG_FORMULA_H
