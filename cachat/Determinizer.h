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
    set<set<Letter*>> todo;
    set<set<Letter*>> done;
    set<Transition*> nfa_trans;
    set<Transition*> dfa_trans;
    set<Letter*> dfa_final_states;
    Letter* init_letter;

public:
    string setToString (set<Letter*> set);

    Determinizer (NFA* A) :
            A(A),
            Q(A->states),
            Sigma(A->Sigma),
            nfa_trans(A->transitions)
    {
        PQ = new Alphabet();
        set<Letter*> init_set;
        init_set.insert(A->initial_state);
        todo.insert(init_set);

        init_letter = PQ->addLetter(setToString(init_set));
        set_to_state.emplace(init_set, init_letter);

    }

    NFA* determinize ()
    {
        while (!todo.empty())
        {
            // pick state set from worklist for which transitions have not yet been processed
            auto source_set = *todo.begin();
            todo.erase(todo.begin());
            done.insert(source_set);


            Letter* source_letter = set_to_state.find(source_set)->second;

            for (Letter* a : Sigma->letters)
            {

                set<Letter*> target_set;
                bool isFinal = false;

                // construct target set for given source set and letter
                for (Transition* t : nfa_trans)
                {
                    if (t->label == a && source_set.find(t->origin) != source_set.end())
                    {
                        target_set.insert(t->target);

                        if (A->final_states.find(t->target) != A->final_states.end())
                        {
                            isFinal = true;
                        }
                    }
                }

                // check if target set has already an associated letter
                // if not, create it, and add target set to worklist
                auto target_letter_itr = set_to_state.find(target_set);
                Letter* target_letter;
                if (target_letter_itr == set_to_state.end())
                {
                    target_letter = PQ->addLetter(setToString(target_set));
                    set_to_state.emplace(target_set, target_letter);
                    todo.insert(target_set);
                }
                else
                {
                    target_letter = target_letter_itr->second;
                }

                if (isFinal)
                {
                    dfa_final_states.insert(target_letter);
                }


                dfa_trans.insert(new Transition(source_letter, a, target_letter));
            }

        }

        NFA* res = new NFA(Sigma, PQ, init_letter, dfa_final_states);
        for (Transition* t : dfa_trans)
        {
            res->addTransition(t);
        }
        return res;
    }
};


#endif //RIGG_DETERMINIZER_H
