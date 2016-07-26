#ifndef RIGG_CLAUSE_H
#define RIGG_CLAUSE_H

class Formula;

#include "Box.h"
#include "Formula.h"
#include "Ternary.h"
#include "../common/Types.h"

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
    {}

    /**
     * We do not delete the boxes in the clause, because the same object might be used in other clauses
     */
    virtual ~Clause ()
    {}

/**
     * list of boxes of which the clause consits
     */
    vector<Box*> boxes;

    /**
     * computes the list of clauses that we get as the composition of the current clause with a whole formula
     */
    vector<Clause*> composeWith (Formula* G);

    /**
     * string representation
     */
    virtual string toString () const;

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

    Clause (Clause const &) = default;

    Clause &operator= (Clause const &) = default;

private:
    /**
     * Given an array of uint64s of length k with entries in 0...g, tries to increment position start
     *
     * If overflow is triggered, go to the next position and reset later position
     *
     * returns the next position on which the next increment should happen afterwards or k if no further increment is possible
     */
    uint64 increment (uint64* func, uint64 k, uint64 g, uint64 start);
};

#endif //RIGG_CLAUSE_H
