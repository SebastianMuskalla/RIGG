#include "Determinizer.h"

using namespace std;

string Determinizer::setToString (set<Letter*> set)
{
    string res = "{";
    bool first = true;
    for (Letter* l : set)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            res.append(",");
        }
        res.append(l->toString());
    }
    res.append("}");
    return res;
}

Determinizer::Determinizer (NFA* A) :
        A(A),
        Q(A->Q),
        Sigma(A->Sigma),
        nfa_trans(A->transitions),
        set_to_state(),
        worklist(),
        done(),
        dfa_trans(),
        dfa_final_states()
{
    PQ = new Alphabet();
    set<Letter*> init_set;
    init_set.insert(A->initial_state);
    worklist.insert(init_set);

    init_letter = PQ->addLetter(setToString(init_set));
    set_to_state.emplace(init_set, init_letter);

    if (A->final_states.find(A->initial_state) != A->final_states.end())
    {
        dfa_final_states.insert(init_letter);
    }
}

NFA* Determinizer::determinize ()
{
    while (!worklist.empty())
    {
        // pick state set from worklist for which transitions have not yet been processed
        auto source_set = *worklist.begin();
        worklist.erase(worklist.begin());
        done.insert(source_set);


        Letter* source_letter = set_to_state[source_set];

        for (Letter* a : Sigma->letters)
        {

            set<Letter*> target_set;
            bool isFinal = false;

            // construct target set for given source set and letter
            for (Transition* t : nfa_trans)
            {
                if (t->label == a && source_set.find(t->source) != source_set.end())
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
                worklist.insert(target_set);

                if (isFinal)
                {
                    dfa_final_states.insert(target_letter);
                }
            }
            else
            {
                target_letter = target_letter_itr->second;
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
