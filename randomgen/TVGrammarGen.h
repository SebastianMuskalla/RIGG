//
// Created by Sebastian on 13.07.2016.
//

#ifndef RIGG_TVGRAMMARGEN_H
#define RIGG_TVGRAMMARGEN_H

#include "../common/Alphabet.h"
#include "../common/Letter.h"
#include "../common/GameGrammar.h"

/**
 * Creates a random Game Grammar similar to the Tabakov-Vardi-model for automata
 *
 * The rules of the grammar are of the shape X -> a Y b
 *
 * Use each instance only once!
 */
class TVGrammarGen
{
private:
    Alphabet* Sigma;
    const unsigned int nr_refuter_nonterminals;
    const unsigned int nr_prover_nonterminals;

    const double density;
    const double final_rate;
    const double left_rate;
    const double right_rate;

    Alphabet* Nprover;
    Alphabet* Nrefuter;

public:
    /**
     * Specify terminals, number of nonterminals for prover and refuter, density (number of outgoing rules for each nonterminal),
     * final_rate (rate of rules that do not have a non-terminal on their right-hand side),
     * left_rate (rate of rules that have exactly one (random) terminal on the left)
     * right_rate (rate of rules that have exactly one (random) terminal on the right)
     */
    TVGrammarGen (Alphabet* Sigma, unsigned int nr_refuter_nonterminals,
                  unsigned int nr_prover_nonterminals, double density, double final_rate, double left_rate,
                  double right_rate);

    /**
     * Start generation and return generated grammar
     *
     * Only use once per instance!
     */
    GameGrammar* generate ();

    TVGrammarGen (TVGrammarGen const &) = delete;

    TVGrammarGen &operator= (TVGrammarGen const &) = delete;
};

#endif //RIGG_TVGRAMMARGEN_H
