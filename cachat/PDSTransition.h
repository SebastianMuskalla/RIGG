#ifndef RIGG_PDSTRANSITION_H
#define RIGG_PDSTRANSITION_H

#include <c++/4.8.3/vector>
#include "../common/Letter.h"

using namespace std;

/**
 * transition of a (game) pushdown system
 */
class PDSTransition : public Printable<PDSTransition>
{
public:

    Letter* source;
    /**
     * stack symbol that is popped when doing the transition
     */
    Letter* read;
    /**
     * word that is pushed when doing the transition
     */
    vector<Letter*> write;
    Letter* target;

    PDSTransition (Letter* source, Letter* read, const vector<Letter*> &write, Letter* target) :
            source(source),
            read(read),
            write(write),
            target(target)
    { }

    virtual string toString () const;

    PDSTransition (PDSTransition const &) = delete;

    PDSTransition &operator= (PDSTransition const &) = delete;
};

#endif //RIGG_PDSTRANSITION_H
