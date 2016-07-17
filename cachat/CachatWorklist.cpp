#include "CachatWorklist.h"

CachatWorklist::CachatWorklist (GamePDS* P, PAFA* AFA) :
        P(P),
        AFA(AFA)
{
    populate();
}

void CachatWorklist::populate ()
{
    populateWorklist();
    populateDependencies();
}

void CachatWorklist::populateWorklist ()
{
    for (Letter* p : P->player0_states->letters)
    {
        worklist.insert(p);
    }
    for (Letter* p : P->player1_states->letters)
    {
        worklist.insert(p);
    }
}

void CachatWorklist::populateDependencies ()
{
    for (PDSTransition* t : P->transitions)
    {
        for (Letter* a : t->write)
        {
            dependencies.emplace(a, t->source);
        }
    }
}


void CachatWorklist::saturate ()
{
    while (!worklist.empty())
    {
        auto itr = worklist.begin();
        Letter* p = *itr;
        worklist.erase(itr);


        for (Letter* a : P->Gamma->letters)
        {
            bool added_rule = false;

            // player 0 states (refuter)
            if (p->alphabet == P->player0_states)
            {

                for (PDSTransition* t : P->transitions)
                {
                    if (t->read == a && t->source == p)
                    {
                        Letter* q = t->target;
                        Letter* q_afa = AFA->pds_state_to_afa_state[q];

                        vector<Letter*> v = t->write;
                        set<set<Letter*>> sets_S = AFA->reachableFromControlState(q_afa, v);

                        for (set<Letter*> S : sets_S)
                        {
                            // try to add transition (do not add it if already present)
                            if (AFA->addTransition(convertToAFAState(p), a, S))
                            {
                                added_rule = true;
                            }
                        }
                    }

                }
            }
                // player 1 states (prover)
            else
            {
                // collect (q_i, v_i) states such that (p,a) -> (q_i, v_i) in the PDS
                set<pair<Letter*, vector<Letter* >>> all_qvi;

                for (PDSTransition* t : P->transitions)
                {
                    if (t->read == a && t->source == p)
                    {
                        Letter* q = t->target;
                        Letter* q_afa = AFA->pds_state_to_afa_state[q];

                        vector<Letter*> v = t->write;
                        all_qvi.insert(make_pair(q_afa, v));
                    }
                }


                // now we need to compute for each (qi vi) all possible S_i_j such that q_i - v_i -> S_i_j in the AFA
                // then we need to compute all unions S_1_j(1) cup ... cup S_imax_j(imax)
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
                    set<set<Letter*>> possible_S_i = AFA->reachableFromControlState(qvi.first, qvi.second);


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
                        }
                    }
                    unions = unions_new;
                }

                for (set<Letter*> S : unions)
                {
                    // try to add transition (do not add it if already present)
                    if (AFA->addTransition(convertToAFAState(p), a, S))
                    {
                        added_rule = true;
                    }
                }
            }

            if (added_rule)
            {
                // insert dependencies
                auto itrpair = dependencies.equal_range(a);
                for (auto itr = itrpair.first; itr != itrpair.second; ++itr)
                {
                    worklist.insert(itr->second);
                }
            }

        } // for a : Gamma
    } // while worklist not empty
}







