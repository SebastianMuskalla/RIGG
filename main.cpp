#include <iostream>
#include <c++/4.8.3/chrono>
#include "common/Alphabet.h"
#include "common/NFA.h"
#include "dfa/WorklistKleene.h"
#include "cachat/Determinizer.h"
#include "cachat/GrammarDFAtoPDSAFA.h"
#include "cachat/Cachat.h"
#include "randomgen/TVGrammarGen.h"
#include "randomgen/TVAutomataGen.h"
#include "dfa/NaiveKleene.h"
#include "cachat/Minimizer.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
using namespace std;

uint max_time = 1000; // 1 second

/**
 * Given an NFA representing the regular goal language, a game grammar and an initial sentential form,
 * return true iff refuter can win the non-inclusion game from the given sentential form
 *
 * Internally, fixed point iteration over the domain of box formulas is used
 */
pair<bool, bool> solveWithWorklistDFA (NFA* A, GameGrammar* G, vector<Letter*> word, bool use_subsumption = false)
{
    WorklistKleene* s = new WorklistKleene(A, G, false, max_time);
    s->solve();

    if (s->timeout_flag)
    {
        delete s;
        return make_pair(true, true);
    }

    Formula* sol = s->formulaFor(word);
    bool res = sol->isRejecting();
    delete s;
    return make_pair(res, false);
}

/**
 * Given an NFA representing the regular goal language, a game grammar and an initial sentential form,
 * return true iff refuter can win the non-inclusion game from the given sentential form
 *
 * Internally, fixed point iteration over the domain of box formulas is used
 */
pair<bool, bool> solveWithNaiveDFA (NFA* A, GameGrammar* G, vector<Letter*> word)
{
    NaiveKleene* s = new NaiveKleene(A, G, max_time);
    s->solve();

    if (s->timeout_flag)
    {
        delete s;
        return make_pair(true, true);
    }

    Formula* sol = s->formulaFor(word);
    bool res = sol->isRejecting();
    delete s;
    return make_pair(res, false);
}


/**
 * Given an NFA representing the regular goal language, a game grammar and an initial sentential form,
 * return true iff refuter can win the non-inclusion game from the given sentential form
 *
 * Internally, the game is converted to a pushdown game a la Cachat and solved using his saturation procedure
 *
 * Provides time measuring for the 3 phases of the procedure
 */
pair<bool, bool> measureCachat (NFA* A, GameGrammar* G, vector<Letter*> word)
{
    // determinize the given automaton
    Determinizer* det = new Determinizer(A);
    NFA* D = det->determinize();

    // determinize the given automaton
    Minimizer* min = new Minimizer(D);
    NFA* M = min->minimize();

    // generate pushdown system and alternating automaton that define the equivalent game
    GrammarDFAtoPDSAFA* cachatifier = new GrammarDFAtoPDSAFA(M, G);
    tuple<GamePDS*, PAFA*, Letter*, Letter*> restuple = cachatifier->cachatify();
    GamePDS* P = get<0>(restuple);
    PAFA* AFA = get<1>(restuple);
    Letter* init_refuter = get<2>(restuple);
    Letter* init_prover = get<3>(restuple);


    // solve using cachats saturation procedure
    Cachat* cachat = new Cachat(P, AFA, max_time);
    cachat->saturate();

    if (cachat->timeout_flag)
    {
        delete det;
        delete min;
        delete cachatifier;
        delete cachat;
        delete D;
        delete M;
        delete AFA;
        delete P;

        return make_pair(true, true);
    }

    vector<Letter*> stack_word = cachatifier->wordToStackWord(word);
    bool res = AFA->acceptsFromControlState(AFA->pds_state_to_afa_state[init_refuter], stack_word);

    delete det;
    delete min;
    delete cachatifier;
    delete cachat;
    delete D;
    delete M;
    delete AFA;
    delete P;

    return make_pair(res, false);
}

/**
 * Takes a game instance (NFA, PDS, two initial sentential forms), solve it using both algorithms and measure the time it takes
 */
tuple<uint, uint, uint, uint, uint, uint> timeMeasuringWithTimeouts (
        tuple<NFA*, GameGrammar*, vector<Letter*>, vector<Letter*>> t)
{
    NFA* A = get<0>(t);
    GameGrammar* G = get<1>(t);
    vector<Letter*> word1 = get<2>(t);
    vector<Letter*> word2 = get<3>(t);

    uint naive_dfa_timeouts = 0;
    uint worklist_dfa_timeouts = 0;
    uint cachat_timeouts = 0;

    auto t1 = chrono::steady_clock::now();
    auto res_naive_dfa_1 = solveWithNaiveDFA(A, G, word1);
    auto t2 = chrono::steady_clock::now();
    auto res_naive_dfa_2 = solveWithNaiveDFA(A, G, word2);
    auto t3 = chrono::steady_clock::now();
    auto res_worklist_dfa_1 = solveWithWorklistDFA(A, G, word1);
    auto t4 = chrono::steady_clock::now();
    auto res_worklist_dfa_2 = solveWithWorklistDFA(A, G, word2);
    auto t5 = chrono::steady_clock::now();
    auto cachat_min_1 = measureCachat(A, G, word1);
    auto t6 = chrono::steady_clock::now();
    auto cachat_min_2 = measureCachat(A, G, word2);
    auto t7 = chrono::steady_clock::now();

    uint naive_dfa_time = 0;
    uint worklist_time = 0;
    uint cachat_time = 0;

    if (res_naive_dfa_1.second)
    {
        naive_dfa_timeouts++;
    }
    else
    {
        naive_dfa_time += chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
    }

    if (res_naive_dfa_2.second)
    {
        naive_dfa_timeouts++;
    }
    else
    {
        naive_dfa_time += chrono::duration_cast<chrono::milliseconds>(t3 - t2).count();
    }

    if (res_worklist_dfa_1.second)
    {
        worklist_dfa_timeouts++;
    }
    else
    {
        worklist_time += chrono::duration_cast<chrono::milliseconds>(t4 - t3).count();
    }

    if (res_worklist_dfa_2.second)
    {
        worklist_dfa_timeouts++;
    }
    else
    {
        worklist_time += chrono::duration_cast<chrono::milliseconds>(t5 - t4).count();
    }

    if (cachat_min_1.second)
    {
        cachat_timeouts++;
    }
    else
    {
        cachat_time += chrono::duration_cast<chrono::milliseconds>(t6 - t5).count();
    }

    if (cachat_min_2.second)
    {
        cachat_timeouts++;
    }
    else
    {
        cachat_time += chrono::duration_cast<chrono::milliseconds>(t7 - t6).count();
    }


    return tuple<uint, uint, uint, uint, uint, uint>(naive_dfa_time, naive_dfa_timeouts, worklist_time,
                                                     worklist_dfa_timeouts, cachat_time, cachat_timeouts);
}


void benchmark ()
{
    vector<uint> all_nr_terminals = {10};
    vector<uint> all_nr_nonterminals = {15};
    uint nr_tries = 10;
    for (int nr_states = 5; true; nr_states += 5)
    {
        for (uint nr_terminals : all_nr_terminals)
        {
            for (uint nr_nonterminals : all_nr_nonterminals)
            {
                uint naive_dfa_total = 0;
                uint worklist_dfa_total = 0;
                uint min_cachat_total = 0;
                uint naive_dfa_timeouts = 0;
                uint worklist_dfa_timeouts = 0;
                uint min_cachat_timeouts = 0;

                for (uint i = 0; i < nr_tries; i++)
                {
                    NFA* A = TVAutomataGen(nr_terminals, nr_states, 1, 0.6).generate();
                    GameGrammar* G = TVGrammarGen(A->Sigma, nr_nonterminals, nr_nonterminals, 1, 0.9, 0.9,
                                                  0.9).generate();

                    auto t = timeMeasuringWithTimeouts(
                            tuple<NFA*, GameGrammar*, vector<Letter*>, vector<Letter*>>(A, G, {G->Nrefuter->get(0)},
                                                                                        {G->Nprover->get(0)}));

                    naive_dfa_total += get<0>(t);
                    worklist_dfa_total += get<2>(t);
                    min_cachat_total += get<4>(t);

                    naive_dfa_timeouts += get<1>(t);
                    worklist_dfa_timeouts += get<3>(t);
                    min_cachat_timeouts += get<5>(t);

                    delete A->Sigma;
                    delete A;
                    delete G;
                }

                uint naive_dfa_avg = naive_dfa_total / (2 * nr_tries);
                uint worklist_dfa_avg = worklist_dfa_total / (2 * nr_tries);
                uint min_cachat_avg = min_cachat_total / (2 * nr_tries);

                cout
                << nr_states << "/" << nr_terminals << "/" << nr_nonterminals << ":    "
                << naive_dfa_avg << " / " << naive_dfa_timeouts << "; "
                << worklist_dfa_avg << " / " << worklist_dfa_timeouts << "; "
                << min_cachat_avg << " / " << min_cachat_timeouts << "; "
                << endl;

            }
        }
    }
}

int main ()
{
    srand(time(NULL) * getpid());

    benchmark();

    return 0;
}

#pragma clang diagnostic pop
