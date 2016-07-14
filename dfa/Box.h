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

/**
 * box, i.e. set of state-pairs in the automaton
 */
class Box : public Printable<Box>
{
private:
    /**
 * memoization: values will be computed on first usage
 */
    Ternary rejecting = UNDEFINED;

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
     * name is ignored when checking for equality, because differents words might induce the same box (behavior on the automaton)
     */
    string name;

    Box (NFA* A, Alphabet* Q, string name);

public:
    /**
     * state-pairs forming the box
     */
    multimap<Letter*, Letter*> content;

    /**
     * compose two boxes according to relational composition
     */
    Box* composeWith (Box* r);

    /**
     * compose a box with a whole clause, which results in a list of boxes
     */
    vector<Box*> composeWith (Clause* r);

    /**
     * string representation
     */
    virtual string toString () const;

    bool isRejecting ();

    /**
     * check boxes for equality of the content (ignores the name)
     */
    bool operator== (const Box &other) const
    {
        return content == other.content;
    }

    /**
     * test function that contains empty boxes with a specified name
     */
    static Box* test (string name)
    {
        return new Box(nullptr, new Alphabet(), name);
    }
};

#endif //RIGG_BOX_H
