#ifndef RIGG_GRAMMARNFATOPDSAFA_H
#define RIGG_GRAMMARNFATOPDSAFA_H


#include "../common/NFA.h"
#include "../common/GameGrammar.h"
#include "GamePDS.h"
#include "PAFA.h"

using namespace std;

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

    tuple<GamePDS*, PAFA*, Letter*, Letter*> cachatify ();

    vector<Letter*> wordToStackWord (vector<Letter*> word);
};


#endif //RIGG_GRAMMARNFATOPDSAFA_H
