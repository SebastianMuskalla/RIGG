#ifndef RIGG_DETERMINIZER_H
#define RIGG_DETERMINIZER_H

#include <c++/4.8.3/set>
#include <c++/4.8.3/algorithm>
#include "../common/NFA.h"

using namespace std;

/**
 * given an NFA A, it computes a DFA (unique and complete) that has the same language
 *
 * the resulting DFA may not be minimal, and it is recommended to minimize it before using it
 *
 * the resulting DFA will not have unreachable states, since the powersets are explored starting from the set {q_0} only containing the initial state q_0 of A
 */
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

    /**
     * helper function that computes a string representation for a set
     */
    string setToString (set<Letter*> set);

public:

    /**
     * given the NFA A that should be determinize
     */
    Determinizer (NFA* A);

    /**
     * determinize and return the result
     */
    NFA* determinize ();
};

#endif //RIGG_DETERMINIZER_H
