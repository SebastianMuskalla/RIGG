#ifndef RIGG_PDSTRANSITION_H
#define RIGG_PDSTRANSITION_H


#include <c++/4.8.3/vector>
#include "../common/Letter.h"

using namespace std;

class PDSTransition : public Printable<PDSTransition>
{
public:

    Letter* source_state;
    Letter* read;
    vector<Letter*> write;
    Letter* target_state;


    PDSTransition (Letter* source_state, Letter* read, const vector<Letter*> &write, Letter* target_state) :
            source_state(source_state),
            read(read),
            write(write),
            target_state(target_state)
    { }

    virtual string toString () const;
};


#endif //RIGG_PDSTRANSITION_H
