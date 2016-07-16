#ifndef RIGG_GRAMMARNFATOPDSAFA_H
#define RIGG_GRAMMARNFATOPDSAFA_H

#include "../common/NFA.h"
#include "../common/GameGrammar.h"
#include "GamePDS.h"
#include "PAFA.h"

using namespace std;

/**
 * converts a given grammar game (i.e. game grammar G and DFA A specifying the goal language) to an instance for Cachat's pushdown game
 *
 * note that we require A to be deterministic, because otherwise the players are allowed to resolve the non-determinism in the automaton for the goal language, which changes the semantics of the game
 *
 * we create a pushdown system that
 * - has a state q_p (owned by player 0 / refuter) and a state q_r (owned by player 1 / prover) for each state q of A
 * - has the union of terminals and non-terminals as stack_symbols
 * - has for each state q, each player pl, and each terminal x a rule that pops x in q_pl and goes to q'_pl, where q' is the (unique) state such that q -- x --> q' in A
 * - has for each state q, each player pl, each nonterminal X owned by pl, and each rule X -> RHS a rule that pops X, pushes RHS (while the state stays in Qp)
 * - has for each state q, each player pl, and each nonterminal X not owned by pl a rule that goes from q_pl to q_notpl without modifying the stack
 *
 * the goal automaton accepts the empty stack in all states that are not accepting in A, i.e. it accepts if we have successfully processed a terminal word that is not in the language of A
 */
class GrammarDFAtoPDSAFA
{
    NFA* DFA;
    GameGrammar* G;
    Alphabet* Sigma;
    Alphabet* Nrefuter;
    Alphabet* Nprover;
    Alphabet* player0_states;
    Alphabet* player1_states;
    Alphabet* Gamma;
    map<Letter*, Letter*> refuter_states;
    map<Letter*, Letter*> prover_states;
    map<Letter*, Letter*> stack_terminals;
    map<Letter*, Letter*> stack_refuter_nonterminals;
    map<Letter*, Letter*> stack_prover_nonterminals;

public:
    GrammarDFAtoPDSAFA (NFA* DFA, GameGrammar* G);

    /**
     * cachatifies the grammar game
     *
     * the letters that are returned are the initial states of refuter and prover
     *
     * (actually, it does not matter with which state one starts)
     */
    tuple<GamePDS*, PAFA*, Letter*, Letter*> cachatify ();

    /**
     * convers a sentential from of the grammar to the word containing the corresponding stack symbols
     */
    vector<Letter*> wordToStackWord (vector<Letter*> word);

    GrammarDFAtoPDSAFA (GrammarDFAtoPDSAFA const &) = delete;

    GrammarDFAtoPDSAFA &operator= (GrammarDFAtoPDSAFA const &) = delete;
};

#endif //RIGG_GRAMMARNFATOPDSAFA_H
