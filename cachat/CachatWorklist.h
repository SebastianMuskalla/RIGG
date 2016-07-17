#ifndef RIGG_CACHAT_WORKLIST_H
#define RIGG_CACHAT_WORKLIST_H

#include <c++/4.8.3/chrono>
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
class CachatWorklist
{
    GamePDS* P;
    PAFA* AFA;

    bool cout_debug = false;

    set<Letter*> worklist;

    multimap<Letter*, Letter*> dependencies;

    void populate ();

    void populateWorklist ();

    void populateDependencies ();

public:
    CachatWorklist (GamePDS* P, PAFA* AFA);


    void saturate ();

    Letter* convertToAFAState (Letter* p)
    {
        return AFA->pds_state_to_afa_state[p];
    }

    CachatWorklist (CachatWorklist const &) = delete;

    CachatWorklist &operator= (CachatWorklist const &) = delete;
};

#endif //RIGG_CACHAT_WORKLIST_H
