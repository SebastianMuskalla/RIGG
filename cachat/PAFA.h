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


    PAFA (Alphabet* Gamma, GamePDS* P) :
            Gamma(Gamma),
            P(P),
            control_states(),
            pds_state_to_afa_state(),
            final_states(),
            transitions()
    { }

    set<set<Letter*>> reachableFromControlState (Letter* control_state, vector<Letter*> word);

    bool acceptsFromControlState (Letter* control_state, vector<Letter*> word);

    virtual string toString () const;

    bool addTransition (Letter* source, Letter* label, set<Letter*> targets);

};

template<typename T>
struct pointer_values_equal
{
    const T* to_find;

    pointer_values_equal (const T* to_find) :
            to_find(to_find)
    { }

    bool operator() (const T* other) const
    {
        return *to_find == *other;
    }
};


#endif //RIGG_PAFA_H
