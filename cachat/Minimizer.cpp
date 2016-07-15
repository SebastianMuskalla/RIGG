//
// Created by Sebastian on 15.07.2016.
//

#include "Minimizer.h"

Minimizer::Minimizer (NFA* DFA) :
        DFA(DFA),
        Sigma(DFA->Sigma),
        Q(DFA->states)
{
    set<Letter*> F;
    set<Letter*> nonF;

    for (Letter* q : Q->letters)
    {

        if (DFA->final_states.find(q) != DFA->final_states.end())
        {
            F.insert(q);
        }
        else
        {
            nonF.insert(q);
        }
    }

    if (F.empty())
    {
        trivial = true;
        return;
    }

    partition.insert(F);

    if (nonF.empty())
    {
        partition.insert(nonF);
    }

    worklist.push_back(F);
}

NFA* Minimizer::minimize ()
{
    if (trivial)
    {
        Alphabet* singleton = new Alphabet();
        Letter* q = singleton->addLetter("q");
        NFA* min_DFA = new NFA(Sigma, singleton, q, set<Letter*>());
        for (Letter* a : Sigma->letters)
        {
            min_DFA->addTransition(q, a, q);
        }
        return min_DFA;
    }

    while (!worklist.empty())
    {
        // pick state set A from worklist
        set<Letter*> A = *worklist.begin();
        worklist.erase(worklist.begin());

        // for each a in Sigma do
        for (Letter* a : Sigma->letters)
        {
            // let X be the set of states for which a transition on c leads to a state in A
            set<Letter*> X;
            for (Transition* t : DFA->transitions)
            {
                if (t->label == a && A.find(t->target) != A.end())
                {
                    X.insert(t->origin);
                }
            }

            // if X is empty, XcapY will also be empty
            if (!X.empty())
            {

                // for each set Y in partition ...
                for (auto itr = partition.begin(); itr != partition.end();)
                {
                    // ... for which XcapY is nonempty and Y \ X is nonempty
                    set<Letter*> Y = *itr;
                    set<Letter*> YcapX;
                    set<Letter*> YwithoutX;

                    for (Letter* q : Y)
                    {
                        if (X.find(q) != X.end())
                        {
                            YcapX.insert(q);
                        }
                        else
                        {
                            YwithoutX.insert(q);
                        }
                    }

                    if (!YcapX.empty() && !YwithoutX.empty())
                    {
                        auto new_itr = itr;
                        ++new_itr;
                        partition.erase(itr);
                        itr = new_itr;
                        partition.insert(YcapX);
                        partition.insert(YwithoutX);

                        auto w_itr = find(worklist.begin(), worklist.end(), Y);
                        if (w_itr != worklist.end())
                        {
                            worklist.erase(w_itr);
                            worklist.push_back(YcapX);
                            worklist.push_back(YwithoutX);
                        }
                        else
                        {
                            if (YcapX.size() <= YwithoutX.size())
                            {
                                worklist.push_back(YcapX);
                            }
                            else
                            {
                                worklist.push_back(YwithoutX);
                            }
                        }
                    }
                    else
                    {
                        ++itr;
                    }
                }
            }
        } // a in Sigma
    } // while worklist not empty

    Alphabet* PQ = new Alphabet();
    set<Letter*> new_final_states;
    map<Letter*, Letter*> old_state_to_new_state;

    bool found_initial = false;

    for (set<Letter*> Y : partition)
    {
        bool is_final;

        string name = "[";
        for (Letter* l : Y)
        {
            name.append(l->toString());
        }
        name.append("]");
        Letter* state = PQ->addLetter(name);

        for (Letter* l : Y)
        {
            old_state_to_new_state.emplace(l, state);
        }

        // if one state in Y is final, all states in Y are final
        if (DFA->final_states.find(*Y.begin()) != DFA->final_states.end())
        {
            new_final_states.insert(state);
        }

    }

    Letter* new_initial_state = old_state_to_new_state[DFA->initial_state];

    NFA* minDFA = new NFA(Sigma, PQ, new_initial_state, new_final_states);

    for (set<Letter*> Y : partition)
    {
        Letter* state_source = old_state_to_new_state[*Y.begin()];

        for (Transition* t : DFA->transitions)
        {
            if (t->origin == *Y.begin())
            {
                Letter* state_target = old_state_to_new_state[t->target];
                minDFA->addTransition(state_source, t->label, state_target);
            }
        }

    }

    return minDFA;

}
