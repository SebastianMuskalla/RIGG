#ifndef RIGG_BOX_H
#define RIGG_BOX_H

class NFA;

class Clause;

#include "../common/Letter.h"
#include "../common/NFA.h"
#include "Clause.h"
#include <c++/4.8.3/map>
#include <c++/4.8.3/algorithm>
#include "../common/Printable.h"
#include "Ternary.h"

using namespace std;


class Box : public Printable<Box>
{
private:
    Ternary rejecting = UNDEFINED;

public:
    NFA* A;
    Alphabet* Q;
    string name;


    Box (NFA* A, Alphabet* Q, string name) :
            A(A),
            Q(Q),
            name(name)
    { }

public:
    multimap<Letter*, Letter*> content;

    Box* composeWith (Box* r);

    vector<Box*> composeWith (Clause* r);

    virtual string toString () const;

    static Box* test (string name)
    {
        return new Box(nullptr, new Alphabet(), name);
    }

    bool isRejecting ();

    bool operator== (const Box &other) const
    {
//        if (A != other.A || Q != other.Q )
//            return false;
//        return is_permutation(content.begin(), content.end(), other.content.begin());
        return content == other.content;
    }
};


#endif //RIGG_BOX_H
