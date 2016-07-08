#ifndef RIGG_CLAUSE_H
#define RIGG_CLAUSE_H

class Formula;

#include "Box.h"
#include "Formula.h"
#include "Rejecting.h"

typedef unsigned long long int uint;

using namespace std;

class Clause : public Printable<Clause>
{
private:
    Rejecting rejecting = UNDEFINED;
public:
    vector<Box*> boxes;

    vector<Clause*> composeWith (Formula* G);

    virtual string toString () const;

    bool isRejecting ();

    bool contains (Clause* c);

    static Clause* wrap (Box* pBox);

private:
    uint increment (uint* func, uint k, uint g, uint start);
};


#endif //RIGG_CLAUSE_H
