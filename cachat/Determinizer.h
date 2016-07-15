#ifndef RIGG_DETERMINIZER_H
#define RIGG_DETERMINIZER_H

#include <c++/4.8.3/set>
#include <c++/4.8.3/algorithm>
#include "../common/NFA.h"

using namespace std;

class Determinizer
{
    NFA* A;
    Alphabet* Q;
    Alphabet* Sigma;
    Alphabet* PQ;
    map<set<Letter*>, Letter*> set_to_state;
    set<set<Letter*>> worklist;
    set<set<Letter*>> done;
    set<Transition*> nfa_trans;
    set<Transition*> dfa_trans;
    set<Letter*> dfa_final_states;
    Letter* init_letter;

public:
    string setToString (set<Letter*> set);

    Determinizer (NFA* A);

    NFA* determinize ();
};

#endif //RIGG_DETERMINIZER_H
