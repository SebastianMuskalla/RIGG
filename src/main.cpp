#include <iostream>
#include <chrono>
#include <unistd.h>
#include "common/Alphabet.h"
#include "common/NFA.h"
#include "dfa/WorklistKleene.h"
#include "randomgen/TVGrammarGen.h"
#include "randomgen/TVAutomataGen.h"

using namespace std;

enum class Algo
{
        CACHAT,
        KLEENE
};

int main(int argc, char *argv[])
{
    if (argc < 5 || argc > 6) {
        cerr  << "Usage: " << argv[0] <<  " <Algo> <#States> <#Terminals> <#NonTerminalsPerPlayer> [<SEED>]" << endl;
        return 1;
    }


    string algoString(argv[1]);

    Algo algo;
    if (algoString == "Cachat")
    {
        algo = Algo::CACHAT;
    }
    else if (algoString == "Kleene")
    {
        algo = Algo::KLEENE;
    }
    else
    {
        cerr << "First argument <Algo> should be \"Cachat\" or \"Kleene\"" << endl;
        return 1;
    }

    int states = 0;
    try {
        states = stoi(argv[2]);
    } catch (exception const &e) {
        cerr << "Second argument <#States> should be a non-negative integer" << endl;
        return 1;
    }
    if (states <= 0)
    {
        cerr << "Second argument <#States> should be a non-negative integer" << endl;
        return 1;
    }

    int terminals = 0;
    try {
        terminals = stoi(argv[3]);
    } catch (exception const &e) {
        cerr << "Third argument <#Terminals> should be a non-negative integer" << endl;
        return 1;
    }
    if (terminals <= 0)
    {
        cerr << "Third argument <#Terminals> should be a non-negative integer" << endl;
        return 1;
    }

    int nonterminals = 0;
    try {
        nonterminals = stoi(argv[4]);
    } catch (exception const &e) {
        cerr << "Fourth argument <#NonTerminalsPerPlayer> should be a non-negative integer" << endl;
        return 1;
    }
    if (nonterminals <= 0)
    {
        cerr << "Fourth argument <#NonTerminalsPerPlayer> should be a non-negative integer" << endl;
        return 1;
    }


    unsigned int seed;

    if (argc == 6)
    {
        try {
            seed = stoul(argv[5]);
        } catch (exception const &e) {
            cerr << "Optional fifth argument <Seed> should be a non-negative integer" << &e << endl;
            return 1;
        }
        cout << "Using provided seed " << seed << endl;
    }
    else
    {
        seed = time(nullptr) * getpid();

        cout << "Using random seed " << seed << endl;
    }

    srand(seed);


    unsigned long long int SECONDS_FROM_MS = 1000ll;
    unsigned long long int SECONDS_FROM_US = 1000000ll;
    unsigned long long int TIMEOUT = 10 * SECONDS_FROM_US;
    unsigned long long int WAIT = 11 * SECONDS_FROM_MS;

    constexpr unsigned int DEBUG_LEVEL = 2;


    NFA* A = TVAutomataGen(terminals, states, 0.8, 0.8).generate();

    GameGrammar* G = TVGrammarGen(A->Sigma, nonterminals, nonterminals, 0.75, 0.85, 0.85,
                                                  0.85).generate();

    cout << "A: " << A->toString() << endl;

    cout << "G: " << G->toString() << endl;

    Letter* init;
    if (rand() % 2)
    {
        init = G->Nrefuter->get(0);
    }
    else
    {
        init = G->Nprover->get(0);
    }


    if (algo == Algo::KLEENE) {

        auto start = chrono::high_resolution_clock::now();
        asm volatile("" : : "g"(start) : "memory");

        WorklistKleene* s = new WorklistKleene(A, G);
        s->solve();
        Formula* sol = s->formulaFor({init});

        if (sol->isRejecting()) {
            cout << "Existential player wins" << endl;
        }
        else
        {
            cout << "Universal player wins" << endl;
        }

        auto end = chrono::high_resolution_clock::now();
        asm volatile("" : : "g"(end) : "memory");

        long long int time = chrono::duration_cast<chrono::microseconds>(end - start).count();


        cout << "Time consumption: " << time << endl;

    }

    return 0;

//
//                    // benchmark worklist kleene for this instance
//                    BenchmarkThread* thread_worklist_kleene;
//                    try
//                    {
//                        Event done_worklist_kleene;
//                        WorklistKleeneRunnable* runnable_worklist_kleene = new WorklistKleeneRunnable(A, G, {init});
//
//                        thread_worklist_kleene = new BenchmarkThread("worklist_kleene", 99, nullptr,
//                                                                     -1,
//                                                                     false,
//                                                                     0, runnable_worklist_kleene,
//                                                                     &done_worklist_kleene);
//                        thread_worklist_kleene->resume();
//                        auto status = done_worklist_kleene.wait(WAIT);
//
//                        if (status != 0)
//                        {
//                            thread_worklist_kleene->terminate();
//                            timeouts_worklist_kleene++;
//
//                            if (DEBUG_LEVEL > 1)
//                            {
//                                cout << "Worklist Kleene timeout" << endl;
//                            }
//
//                        }
//                        else
//                        {
//                            long long int time = thread_worklist_kleene->getTime();
//                            if (time <= TIMEOUT)
//                            {
//                                total_worklist_kleene += time;
//
//
//                                if (DEBUG_LEVEL > 1)
//                                {
//                                    cout << "Worklist Kleene completed in " << time << endl;
//                                }
//
//                            }
//                            else
//                            {
//                                timeouts_worklist_kleene++;
//
//
//                                if (DEBUG_LEVEL > 1)
//                                {
//                                    cout << "Worklist Kleene timeout" << endl;
//                                }
//                            }
//                        }
//                        delete thread_worklist_kleene;
//                    }
//                    catch (std::bad_alloc &ba)
//                    {
//                        spaceout_worklist_kleene++;
//                        delete thread_worklist_kleene;
//
//                        if (DEBUG_LEVEL > 1)
//                        {
//                            cout << "Worklist Kleene spaceout " << endl;
//                        }
//                    }
//
//                    cout << "Running Cachat" << endl;
//
//                    // benchmark saturation for this instance
//                    BenchmarkThread* thread_cachat;
//                    try
//                    {
//                        Event done_cachat;
//                        CachatRunnable* runnable_cachat = new CachatRunnable(A, G, {init});
//
//                        thread_cachat = new BenchmarkThread("saturation", 99, nullptr, -1,
//                                                            false,
//                                                            0, runnable_cachat,
//                                                            &done_cachat);
//                        thread_cachat->resume();
//                        auto status = done_cachat.wait(WAIT);
//
//                        if (status != 0)
//                        {
//                            thread_cachat->terminate();
//                            timeouts_cachat++;
//
//                            if (DEBUG_LEVEL > 1)
//                            {
//                                cout << "Cachat timeout" << endl;
//                            }
//                        }
//                        else
//                        {
//                            long long int time = thread_cachat->getTime();
//                            if (time <= TIMEOUT)
//                            {
//                                total_cachat += time;
//
//                                if (DEBUG_LEVEL > 1)
//                                {
//                                    cout << "Cachat completed in " << time << endl;
//                                }
//                            }
//                            else
//                            {
//                                timeouts_cachat++;
//
//                                if (DEBUG_LEVEL > 1)
//                                {
//                                    cout << "Cachat timeout" << endl;
//                                }
//                            }
//                        }
//                        delete thread_cachat;
//                    }
//                    catch (std::bad_alloc &ba)
//                    {
//                        spaceout_cachat++;
//                        delete thread_cachat;
//
//                        if (DEBUG_LEVEL > 1)
//                        {
//                            cout << "Cachat spaceout" << endl;
//                        }
//                    }
//
//                    Alphabet* Sigma = A->Sigma;
//                    delete G;
//                    delete A;
//                    delete Sigma;
//
//                }
//
//                unsigned long long int avg_naive_kleene = 0;
//                unsigned long long int avg_worklist_kleene = 0;
//                unsigned long long int avg_cachat = 0;
//
//                if (NR_TRIES != (timeouts_naive_kleene + spaceout_naive_kleene))
//                {
//                    avg_naive_kleene =
//                            total_naive_kleene / (NR_TRIES - timeouts_naive_kleene - spaceout_naive_kleene);
//                }
//
//                if (NR_TRIES != (timeouts_worklist_kleene + spaceout_worklist_kleene))
//                {
//                    avg_worklist_kleene =
//                            total_worklist_kleene /
//                            (NR_TRIES - timeouts_worklist_kleene - spaceout_worklist_kleene);
//                }
//
//                if (NR_TRIES != (timeouts_cachat + spaceout_cachat))
//                {
//                    avg_cachat = total_cachat / (NR_TRIES - timeouts_cachat - spaceout_cachat);
//                }
//
//                cout
//                        << nr_states << "/" << nr_terminals << "/" << nr_nonterminals << ":    "
//                        << avg_naive_kleene << " / " << timeouts_naive_kleene << " / " << spaceout_naive_kleene << "; "
//                        << avg_worklist_kleene << " / " << timeouts_worklist_kleene << " / " << spaceout_worklist_kleene
//                        <<
//                        "; "
//                        << avg_cachat << " / " << timeouts_cachat << " / " << spaceout_cachat << "; "
//                        << endl << endl;
//
//            }
//        }
//
//    }
//    return 0;
}
