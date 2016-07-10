#include "Cachat.h"

using namespace std;

uint Cachat::increment (uint* func, uint k, uint g, uint start)
{
    bool reset = false;

    // assert(hint >= 0);
    for (uint i = start; i < k; ++i)
    {
        if (func[i] < g)
        {
            ++func[i];

            if (reset)
            {
                for (int j = 0; j < i; ++j)
                {
                    func[j] = 0;
                }
                return 0;
            }
            else
            {
                return i;
            }
        }
        else
        {
            reset = true;
        }
    }
    return k;
}

void Cachat::saturate ()
{
    bool done = false;
    while (!done)
    {
        done = true;
        for (Letter* p : P->player1_states->letters)
        {
            for (Letter* a : P->stack_alphabet->letters)
            {
                for (PDSTransition* t : P->transitions)
                {
                    if (t->read == p && t->source_state == a)
                    {
                        Letter* q = t->target_state;
                        vector<Letter*> v = t->write;
                        set<set<Letter*>> sets_S = A->reachableFromControlState(q, v);

                        for (set<Letter*> S : sets_S)
                        {
                            if (A->addTransition(p, a, S))
                            {
                                done = false;
                            }
                        }
                    }

                }
            }


            for (Letter* p : P->player2_states->letters)
            {
                for (Letter* a : P->stack_alphabet->letters)
                {

                    set<pair<Letter*, vector<Letter*>>> all_qvi;

                    for (PDSTransition* t : P->transitions)
                    {
                        if (t->read == p && t->source_state == a)
                        {
                            Letter* q = t->target_state;
                            vector<Letter*> v = t->write;
                            all_qvi.insert(make_pair(q, v));

                        }
                    }


                    map<pair<Letter*, vector<Letter*>>, set<set<Letter*>>> possible_Si;
                    for (auto qvi : all_qvi)
                    {
                        possible_Si.emplace(qvi, A->reachableFromControlState(qvi.first, qvi.second));
                    }

//
//
//                    set<set<Letter*>>* sets_S = new set<set<Letter*>>(A->reachableFromControlState(q, v));
//                    vector_sets_S.emplace(a, sets_S);
//
//                    uint k =
//                            uint * func = new uint[k]();
//                    uint G_size = G->clauses.size() - 1;
//                    uint start = 0;

                }
            }


        }

    }
}
