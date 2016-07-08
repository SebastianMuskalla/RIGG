#ifndef RIGG_NFA_H
#define RIGG_NFA_H

class Box;

#include <c++/4.8.3/set>
#include <c++/4.8.3/unordered_map>
#include "Alphabet.h"
#include "Letter.h"
#include "Transition.h"
#include "../dfa/Box.h"

using namespace std;

class NFA
{


public:
    Alphabet* Sigma;
    Alphabet* states;
    set<Transition*> transitions;
    unordered_map<Letter*, Box*> boxes;

    NFA (Alphabet* Sigma, Alphabet* states, Letter* initial_state, set<Letter*> &final_states);

    Transition* addTransition (Letter* origin, Letter* label, Letter* target);

    Box* boxFor (Letter* a);

    Letter* initial_state;
    set<Letter*> final_states;
};


#endif //RIGG_NFA_H
