#include "Cachat.h"

void Cachat::saturate ()
{
    unsigned int iteration = 0;
    bool done = false;


    if (cout_debug)
    {
        cout << "initial AFA" << endl;
        cout << *A << endl;
        cout << endl;
        cout << "PDS" << endl;
        cout << *P << endl;
        cout << endl << endl;

    }


    while (!done)
    {
        done = true;
        iteration++;

        if (cout_debug)
        {
            cout << "ITERATION NR " << to_string(iteration) << endl;
        }

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

        if (cout_debug)
        {
            cout << "AFA after iteration " << to_string(iteration) << ", player 0" << endl;
            cout << *A << endl;
            cout << endl;
        }


        // player 1 states (prover)
        for (Letter* p : P->player1_states->letters)
        {
            for (Letter* a : P->stack_alphabet->letters)
            {

                if (cout_debug)
                {
                    cout << "    Handling state " << *p << " and letter " << *a << endl;
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

                if (cout_debug)
                {
                    for (auto qvi : all_qvi)
                    {
                        cout << "        q: " << *qvi.first << ", v: ";
                        for (Letter* l : qvi.second)
                        {
                            cout << *l;
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

                if (cout_debug)
                {
                    cout << "####" << endl;
                }
                for (auto qvi : all_qvi)
                {
                    unions_new.clear();
                    // compute for each i all possible S such that q_i - v_i -> S in the AFA
                    set<set<Letter*>> possible_S_i = A->reachableFromControlState(qvi.first, qvi.second);

//
//                    bool all_empty = true;
//                    for (set<Letter*> S_i : possible_S_i)
//                    {
//                        if (!S_i.empty())
//                        {
//                            all_empty = false;
//                        }
//                    }
//                    if (all_empty)
//                    {
//                        goto _postloop;
//                    }


                    if (cout_debug)
                    {

                        cout << "        q: " << *qvi.first << ", v: ";
                        for (Letter* l : qvi.second)
                        {
                            cout << *l;
                        }
                        cout << endl;

                        cout << "            reachable S_i:";
                        for (set<Letter*> S_i : possible_S_i)
                        {
                            cout << "(";

                            for (Letter* s : S_i)
                            {
                                cout << *s;
                            }

                            cout << ")";
                        }
                        cout << endl;
                    }

                    for (set<Letter*> S_i : possible_S_i)
                    {
                        for (set<Letter*> old_S : unions)
                        {
                            set<Letter*> new_S;
                            new_S.insert(old_S.begin(), old_S.end());
                            new_S.insert(S_i.begin(), S_i.end());

                            if (!new_S.empty())
                            {
                                unions_new.insert(new_S);
                            }
                            // unions_new.insert(new_S);
                        }
                    }
                    unions = unions_new;
                }

                for (set<Letter*> S : unions)
                {
                    // try to add transition (do not add it if already present)
                    if (A->addTransition(convertToAFAState(p), a, S))
                    {
                        done = false;
                    }
                }

                _postloop:;
            }
        } // for p : player1

        if (cout_debug)
        {
            cout << "AFA after iteration " << to_string(iteration) << ", player 1" << endl;
            cout << *A << endl;
            cout << endl;
        }


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




