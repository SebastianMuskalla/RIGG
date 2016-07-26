#ifndef RIGG_CACHAT_H
#define RIGG_CACHAT_H

#include <chrono>
#include "GamePDS.h"
#include "PAFA.h"

typedef unsigned long long int uint;

using namespace std;

/**
 * implements Cachat's saturation algorithm
 *
 * given a pushdown system with an ownership partitioning of the control states and an alternating finite automaton accepting the goal configurations, it satures the AFA to accepts the whole winning region, i.e. the set of configurations from which player0 can enforce reaching a goal configuration after finitely many steps
 *
 * the provided AFA is modified!
 *
 * has exponential worst-case time complexity (in the number of states of the AFA)
 */
class Cachat
{
    GamePDS* P;
    PAFA* AFA;

    bool cout_debug = false;

    chrono::steady_clock::time_point start;
    uint timeout;


public:
    Cachat (GamePDS* P, PAFA* AFA, uint timeout = 0) :
            P(P),
            AFA(AFA),
            timeout(timeout)
    {}

    bool timeout_flag = false;

    void saturate ();

    Letter* convertToAFAState (Letter* p)
    {
        return AFA->pds_state_to_afa_state[p];
    }

    Cachat (Cachat const &) = delete;

    Cachat &operator= (Cachat const &) = delete;
};

#endif //RIGG_CACHAT_H
