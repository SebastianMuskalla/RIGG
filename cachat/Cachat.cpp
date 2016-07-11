#include "Cachat.h"

void Cachat::saturate ()
{
    bool done = false;
    int itr = 0;

    while (!done)
    {
        itr++;
        done = true;

        // player 0 states (refuter)
        for (Letter* p : P->player0_states->letters)
        {
            for (Letter* a : P->stack_alphabet->letters)
            {
                for (PDSTransition* t : P->transitions)
                {
                    if (t->read == a && t->source_state == p)
                    {
                        Letter* q = t->target_state;
                        Letter* q_afa = A->pds_state_to_afa_state[q];

                        vector<Letter*> v = t->write;
                        set<set<Letter*>> sets_S = A->reachableFromControlState(q_afa, v);


                        for (set<Letter*> S : sets_S)
                        {

                            // try to add transition (do not add it if already present)
                            if (A->addTransition(convertToAFAState(p), a, S))
                            {
                                done = false;
                            }
                        }
                    }

                }
            }
        } // for p : player0


        cout << "iteration " << itr << endl;
        cout << *A << endl;

        // player 1 states (prover)
        for (Letter* p : P->player1_states->letters)
        {
            for (Letter* a : P->stack_alphabet->letters)
            {
                bool important = false;
                if (p->name == "{}_p" && a->name == "X")
                {
                    important = true;
                }

                if (important)
                {
                    cout << "state: " << *p << ", Letter: " << *a << endl;
                }


                // collect (q_i, v_i) states such that (p,a) -> (q_i, v_i) in the PDS
                set<pair<Letter*, vector<Letter*>>> all_qvi;

                for (PDSTransition* t : P->transitions)
                {
                    if (t->read == a && t->source_state == p)
                    {
                        Letter* q = t->target_state;
                        Letter* q_afa = A->pds_state_to_afa_state[q];

                        vector<Letter*> v = t->write;
                        all_qvi.insert(make_pair(q_afa, v));

                    }
                }

                if (important)
                {
                    for (auto qvi : all_qvi)
                    {
                        cout << "q: " << *qvi.first << ", v: ";
                        for (auto itr : qvi.second)
                        {
                            cout << *itr;
                        }
                        cout << endl;
                    }

                }



//                    // compute for each i all possible S such that q_i - v_i -> S in the AFA
//                    map<pair<Letter*, vector<Letter* >>, set<set<Letter* >>> possible_Si;
//                    for (auto qvi : all_qvi)
//                    {
//                        possible_Si.emplace(qvi, A->reachableFromControlState(qvi.first, qvi.second));
//                    }

                // now we need to compute all possible unions... this is ugly!

                set<set<Letter*>> unions;
                set<set<Letter*>> unions_new;

                // add empty set as initial element
                unions.insert(set<Letter*>());

                for (auto qvi : all_qvi)
                {
                    unions_new.clear();
                    // compute for each i all possible S such that q_i - v_i -> S in the AFA
                    set<set<Letter*>> possible_S_i = A->reachableFromControlState(qvi.first, qvi.second);

                    for (set<Letter*> S_i : possible_S_i)
                    {
                        if (important)
                        {
                            cout << "    current S_i: ";
                            for (Letter* s : S_i)
                            {
                                cout << *s;
                            }
                            cout << endl;
                        }

                        for (set<Letter*> old_S : unions)
                        {
                            set<Letter*> new_S;
                            new_S.insert(old_S.begin(), old_S.end());
                            new_S.insert(S_i.begin(), S_i.end());

                            unions_new.insert(new_S);
                        }
                    }

                    unions = unions_new;
                }

                if (important)
                {
                    for (set<Letter*> S : unions)
                    {
                        cout << "    possible S: ";
                        for (Letter* q : S)
                        {
                            cout << *q;
                        }
                        cout << endl;


                    }

                }


                for (set<Letter*> S : unions)
                {

                    // try to add transition (do not add it if already present)
                    if (A->addTransition(convertToAFAState(p), a, S))
                    {
                        done = false;
                    }
                }
            }
        } // for p : player1

        cout << "iteration " << itr << ", player1 " << endl;
        cout << *A << endl;

    } // while !done
}

Letter* Cachat::convertToAFAState (Letter* p)
{
    return A->pds_state_to_afa_state[p];
}

set<Letter*> Cachat::convertToAFAStates (set<Letter*> S)
{
    set<Letter*> res;
    for (Letter* s : S)
    {
        res.insert(A->pds_state_to_afa_state[s]);
    }
    return res;
}




