#ifndef RIGG_FORMULA_H
#define RIGG_FORMULA_H

#include "Clause.h"
#include "Ternary.h"

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
    mutable Ternary rejecting = UNDEFINED;
    mutable Ternary is_false = UNDEFINED;

    static Formula* FALSE_FORMULA;

public:
    /**
     * list of clauses of the formula
     */
    vector<Clause*> clauses;

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
    virtual string toString () const;

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
    static Formula* wrap (Box* b);

    /**
     * given a clause c, returns the singleton formula {c}
     */
    static Formula* wrap (Clause* c);

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
    static Formula* falseFormula ();

    /**
     * resets the values for rejecting and false to unfedin
     */
    void resetMemoization ()
    {
        rejecting = UNDEFINED;
        is_false = UNDEFINED;
    }

    Formula* simplify ();
};

#endif //RIGG_FORMULA_H
