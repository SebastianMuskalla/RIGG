#include <iostream>
#include <chrono>
#include "common/Alphabet.h"
#include "common/NFA.h"
#include "dfa/WorklistKleene.h"
#include "cachat/Cachat.h"
#include "randomgen/TVGrammarGen.h"
#include "randomgen/TVAutomataGen.h"
#include "benchmark/BenchmarkThread.h"
#include "benchmark/NaiveKleeneRunnable.h"
#include "benchmark/WorklistKleeneRunnable.h"
#include "benchmark/CachatRunnable.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
using namespace std;


int main ()
{
    srand(time(NULL) * getpid());

    uint JUMP_FACTOR = 5;
    uint NR_TRIES = 50;

    uint SECONDS_FROM_MS = 1000ll;
    uint SECONDS_FROM_US = 1000000ll;
    uint TIMEOUT = 10 * SECONDS_FROM_US;
    uint WAIT = 11 * SECONDS_FROM_MS;

    for (uint i = 3;; ++i)
    {
        for (uint j = 1; j < i; ++j)
        {
            for (uint k = 1; k < (i - j); ++k)
            {
                uint l = i - j - k;
                uint nr_states = 5 + JUMP_FACTOR * j;
                uint nr_terminals = 5 + JUMP_FACTOR * k;
                uint nr_nonterminals = 5 + JUMP_FACTOR * l;

                uint timeouts_naive_kleene = 0;
                uint timeouts_worklist_kleene = 0;
                uint timeouts_cachat = 0;

                uint spaceout_naive_kleene = 0;
                uint spaceout_worklist_kleene = 0;
                uint spaceout_cachat = 0;

                uint total_naive_kleene = 0;
                uint total_worklist_kleene = 0;
                uint total_cachat = 0;

                for (uint t = 0; t < NR_TRIES; ++t)
                {
                    NFA* A = TVAutomataGen(nr_terminals, nr_states, 0.8, 0.8).generate();
                    GameGrammar* G = TVGrammarGen(A->Sigma, nr_nonterminals, nr_nonterminals, 0.75, 0.85, 0.85,
                                                  0.85).generate();
                    Letter* init;
                    if (rand() % 2)
                    {
                        init = G->Nrefuter->get(0);
                    }
                    else
                    {
                        init = G->Nprover->get(0);
                    }

                    // benchmark naive kleene for this instance
                    BenchmarkThread* thread_naive_kleene;
                    try
                    {
                        Event done_naive_kleene;
                        NaiveKleeneRunnable* runnable_naive_kleene = new NaiveKleeneRunnable(A, G, {init});

                        thread_naive_kleene = new BenchmarkThread("naive_kleene", 99, nullptr, -1,
                                                                  false,
                                                                  0, runnable_naive_kleene,
                                                                  &done_naive_kleene);
                        thread_naive_kleene->resume();

                        auto status = done_naive_kleene.wait(WAIT);

                        if (status != 0)
                        {
                            thread_naive_kleene->terminate();
                            timeouts_naive_kleene++;
                        }
                        else
                        {
                            long long int time = thread_naive_kleene->getTime();
                            if (time <= TIMEOUT)
                            {
                                total_naive_kleene += time;
                            }
                            else
                            {
                                timeouts_naive_kleene++;
                            }
                        }
                        delete thread_naive_kleene;
                    }
                    catch (std::bad_alloc &ba)
                    {
                        spaceout_naive_kleene++;
                        delete thread_naive_kleene;
                    }

                    // benchmark worklist kleene for this instance
                    BenchmarkThread* thread_worklist_kleene;
                    try
                    {
                        Event done_worklist_kleene;
                        WorklistKleeneRunnable* runnable_worklist_kleene = new WorklistKleeneRunnable(A, G, {init});

                        thread_worklist_kleene = new BenchmarkThread("worklist_kleene", 99, nullptr,
                                                                     -1,
                                                                     false,
                                                                     0, runnable_worklist_kleene,
                                                                     &done_worklist_kleene);
                        thread_worklist_kleene->resume();
                        auto status = done_worklist_kleene.wait(WAIT);

                        if (status != 0)
                        {
                            thread_worklist_kleene->terminate();
                            timeouts_worklist_kleene++;
                        }
                        else
                        {
                            long long int time = thread_worklist_kleene->getTime();
                            if (time <= TIMEOUT)
                            {
                                total_worklist_kleene += time;
                            }
                            else
                            {
                                timeouts_worklist_kleene++;
                            }
                        }
                        delete thread_worklist_kleene;
                    }
                    catch (std::bad_alloc &ba)
                    {
                        spaceout_worklist_kleene++;
                        delete thread_worklist_kleene;
                    }

                    // benchmark cachat for this instance
                    BenchmarkThread* thread_cachat;
                    try
                    {
                        Event done_cachat;
                        CachatRunnable* runnable_cachat = new CachatRunnable(A, G, {init});

                        thread_cachat = new BenchmarkThread("cachat", 99, nullptr, -1,
                                                            false,
                                                            0, runnable_cachat,
                                                            &done_cachat);
                        thread_cachat->resume();
                        auto status = done_cachat.wait(WAIT);

                        if (status != 0)
                        {
                            thread_cachat->terminate();
                            timeouts_cachat++;
                        }
                        else
                        {
                            long long int time = thread_cachat->getTime();
                            if (time <= TIMEOUT)
                            {
                                total_cachat += time;
                            }
                            else
                            {
                                timeouts_cachat++;
                            }
                        }
                        delete thread_cachat;
                    }
                    catch (std::bad_alloc &ba)
                    {
                        spaceout_cachat++;
                        delete thread_cachat;
                    }

                    Alphabet* Sigma = A->Sigma;
                    delete G;
                    delete A;
                    delete Sigma;

                }

                uint avg_naive_kleene = 0;
                uint avg_worklist_kleene = 0;
                uint avg_cachat = 0;

                if (NR_TRIES != (timeouts_naive_kleene + spaceout_naive_kleene))
                {
                    avg_naive_kleene =
                            total_naive_kleene / (NR_TRIES - timeouts_naive_kleene - spaceout_naive_kleene);
                }

                if (NR_TRIES != (timeouts_worklist_kleene + spaceout_worklist_kleene))
                {
                    avg_worklist_kleene =
                            total_worklist_kleene /
                            (NR_TRIES - timeouts_worklist_kleene - spaceout_worklist_kleene);
                }

                if (NR_TRIES != (timeouts_cachat + spaceout_cachat))
                {
                    avg_cachat = total_cachat / (NR_TRIES - timeouts_cachat - spaceout_cachat);
                }

                cout
                        << nr_states << "/" << nr_terminals << "/" << nr_nonterminals << ":    "
                        << avg_naive_kleene << " / " << timeouts_naive_kleene << " / " << spaceout_naive_kleene << "; "
                        << avg_worklist_kleene << " / " << timeouts_worklist_kleene << " / " << spaceout_worklist_kleene
                        <<
                        "; "
                        << avg_cachat << " / " << timeouts_cachat << " / " << spaceout_cachat << "; "
                        << endl << endl;

            }
        }

    }
    return 0;
}


#pragma clang diagnostic pop
