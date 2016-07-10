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


public:
    Cachat (GamePDS* P, PAFA* A) :
            P(P),
            A(A)
    { }


    uint increment (uint* func, uint k, uint g,
                    uint start);

    void saturate ();


#endif //RIGG_CACHAT_H
