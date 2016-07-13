//
// Created by Sebastian on 13.07.2016.
//

#ifndef RIGG_TVGRAMMARGEN_H
#define RIGG_TVGRAMMARGEN_H


#include "../common/Alphabet.h"
#include "../common/Letter.h"
#include "../common/GameGrammar.h"

class TVGrammarGen
{
    Alphabet* Sigma;
    unsigned int nr_refuter_nonterminals;
    unsigned int nr_prover_nonterminals;

    double density;
    double final_rate;
    double left_rate;
    double right_rate;


public:
    TVGrammarGen (Alphabet* Sigma, unsigned int nr_refuter_nonterminals,
                  unsigned int nr_prover_nonterminals, double density, double final_rate, double left_rate,
                  double right_rate);

    GameGrammar* generate ();


private:
    Alphabet* Nprover;
    Alphabet* Nrefuter;


};


#endif //RIGG_TVGRAMMARGEN_H
