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

/**
 * Given an NFA representing the regular goal language, a game grammar and an initial sentential form,
 * return true iff refuter can win the non-inclusion game from the given sentential form
 *
 * Internally, fixed point iteration over the domain of box formulas is used
 */
bool solveWithWorklistKleene (NFA* A, GameGrammar* G, vector<Letter*> word, bool use_subsumption = false)
{
    WorklistKleene* s = new WorklistKleene(A, G);
    s->solve();
    Formula* sol = s->formulaFor(word);
    bool res = sol->isRejecting();
    //delete s;
    return res;
}

/**
 * Given an NFA representing the regular goal language, a game grammar and an initial sentential form,
 * return true iff refuter can win the non-inclusion game from the given sentential form
 *
 * Internally, fixed point iteration over the domain of box formulas is used
 */
bool solveWithNaiveKleene (NFA* A, GameGrammar* G, vector<Letter*> word)
{
    NaiveKleene* s = new NaiveKleene(A, G);
    s->solve();

    Formula* sol = s->formulaFor(word);
    bool res = sol->isRejecting();
//    delete s;
    return res;
}


/**
 * Given an NFA representing the regular goal language, a game grammar and an initial sentential form,
 * return true iff refuter can win the non-inclusion game from the given sentential form
 *
 * Internally, the game is converted to a pushdown game a la Cachat and solved using his saturation procedure
 *
 * Provides time measuring for the 3 phases of the procedure
 */
bool solveWithCachat (NFA* A, GameGrammar* G, vector<Letter*> word)
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
    Cachat* cachat = new Cachat(P, AFA);
    cachat->saturate();

    vector<Letter*> stack_word = cachatifier->wordToStackWord(word);
    bool res = AFA->acceptsFromControlState(AFA->pds_state_to_afa_state[init_refuter], stack_word);

//    delete det;
//    delete min;
//    delete cachatifier;
//    delete cachat;
//    delete D;
//    delete M;
//    delete AFA;
//    delete P;

    return res;
}


int main (int argc, char* argv[]) // or char** argv )
{
    cout << argc << endl;
    if (argc != 5)
    {
        cout << "usage: naivekleene/worklistkleene/cachat NR_OF_STATES NR_OF_TERMINALS NR_OF_NONTERMINALS" << endl;
        return 0;
    }

    string algo(argv[1]);
    string nr_states_string(argv[2]);
    string nr_terminals_string(argv[3]);
    string nr_nonterminals_string(argv[4]);

    uint nr_states = atoi(nr_states_string.c_str());
    uint nr_terminals = atoi(nr_terminals_string.c_str());
    uint nr_nonterminals = atoi(nr_nonterminals_string.c_str());

    NFA* A = TVAutomataGen(3, 3, 0.8, 0.8).generate();
    GameGrammar* G = TVGrammarGen(A->Sigma, 3, 3, 0.75, 0.85, 0.85, 0.85).generate();

    if (algo == "naivekleene")
    {

    }

    return 0;
}

#pragma clang diagnostic pop
