#ifndef RIGG_CACHAT_H
#define RIGG_CACHAT_H

#include "GamePDS.h"
#include "PAFA.h"

typedef unsigned long long int uint;

using namespace std;

class Cachat
{
    GamePDS* P;
    PAFA* A;

    bool cout_debug = false;

public:
    Cachat (GamePDS* P, PAFA* A) :
            P(P),
            A(A)
    { }

    void saturate ();

    Letter* convertToAFAState (Letter* p)
    {
        return A->pds_state_to_afa_state[p];
    }
};

#endif //RIGG_CACHAT_H
