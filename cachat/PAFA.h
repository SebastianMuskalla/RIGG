#ifndef RIGG_PAFA_H
#define RIGG_PAFA_H


#include <c++/4.8.3/map>
#include <c++/4.8.3/algorithm>
#include "GamePDS.h"
#include "../common/Alphabet.h"
#include "AFATransition.h"

using namespace std;

class PAFA : public Printable<PAFA>
{
public:
    Alphabet* Gamma;
    GamePDS* P;
    Alphabet* control_states;
    map<Letter*, Letter*> pds_state_to_afa_state;
    set<Letter*> final_states;
    set<AFATransition*> transitions;

    set<set<Letter*>> reachableFromControlState (Letter* control_state, vector<Letter*> word);

    bool acceptsFromControlState (Letter* control_state, vector<Letter*> word);

    bool acceptsFromPState (Letter* p_state, vector<Letter*> word);

    virtual string toString () const;

    void addTransition (Letter* pLetter, Letter* pLetter1, set<Letter*> set);
};


#endif //RIGG_PAFA_H
