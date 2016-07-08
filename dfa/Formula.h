#ifndef RIGG_FORMULA_H
#define RIGG_FORMULA_H


#include "Clause.h"
#include "Ternary.h"

using namespace std;

class Formula : public Printable<Formula>
{
private:
    mutable Ternary rejecting = UNDEFINED;
    mutable Ternary is_false = UNDEFINED;

    static Formula* FALSE_FORMULA;

public:
    bool isRejecting ();

    vector<Clause*> clauses;

    virtual string toString () const;

    Formula* composeWith (Formula* G);

    bool implies (Formula* G);

    static Formula* wrap (Box* pBox);

    static Formula* wrap (Clause* pBox);

    Formula* formulaAnd (Formula* other);

    Formula* formulaOr (Formula* other);

    static Formula* falseFormula ();

    bool isFalse () const;

};


#endif //RIGG_FORMULA_H
