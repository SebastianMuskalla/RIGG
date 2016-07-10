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


    virtual string toString () const;
};


#endif //RIGG_PDSTRANSITION_H
