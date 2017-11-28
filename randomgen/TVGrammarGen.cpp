//
// Created by Sebastian on 13.07.2016.
//

#include "TVGrammarGen.h"
#include "AlphabetGen.h"

TVGrammarGen::TVGrammarGen (Alphabet* Sigma, unsigned int nr_refuter_nonterminals,
                            unsigned int nr_prover_nonterminals, double density, double final_rate, double left_rate,
                            double right_rate) :
        Sigma(Sigma),
        nr_refuter_nonterminals(nr_refuter_nonterminals),
        nr_prover_nonterminals(nr_prover_nonterminals),
        density(density),
        final_rate(final_rate),
        left_rate(left_rate),
        right_rate(right_rate)
{ }

GameGrammar* TVGrammarGen::generate ()
{
    Nrefuter = AlphabetGen(nr_refuter_nonterminals, "X").generate();
    Nprover = AlphabetGen(nr_prover_nonterminals, "Y").generate();

    GameGrammar* G = new GameGrammar(Sigma, Nrefuter, Nprover);

    unsigned int nr_rules_per_nonterminal = (unsigned int) (density *
                                                            (nr_refuter_nonterminals + nr_prover_nonterminals));

    for (Letter* X : Nrefuter->letters)
    {
        for (int i = 0; i < nr_rules_per_nonterminal; ++i)
        {
            vector<Letter*> rhs;
            if (((double) rand() / RAND_MAX) < left_rate)
            {
                unsigned int left_id = (unsigned int) (rand() % Sigma->size());
                Letter* left = Sigma->get(left_id);
                rhs.push_back(left);
            }

            if (((double) rand() / RAND_MAX) > final_rate)
            {
                Letter* Y;
                unsigned int to_id = rand() % (nr_refuter_nonterminals + nr_prover_nonterminals);
                if (to_id < nr_refuter_nonterminals)
                {
                    Y = Nrefuter->get(to_id);
                }
                else
                {
                    to_id -= nr_refuter_nonterminals;
                    Y = Nprover->get(to_id);
                }
                rhs.push_back(Y);

            }

            if (((double) rand() / RAND_MAX) < right_rate)
            {
                unsigned int right_id = (unsigned int) (rand() % Sigma->size());
                Letter* right = Sigma->get(right_id);
                rhs.push_back(right);
            }

            G->addRule(X, rhs);
        }
    }
    for (Letter* X : Nprover->letters)
    {
        for (int i = 0; i < nr_rules_per_nonterminal; ++i)
        {
            vector<Letter*> rhs;
            if (((double) rand() / RAND_MAX) < left_rate)
            {
                unsigned int left_id = (unsigned int) (rand() % Sigma->size());
                Letter* left = Sigma->get(left_id);
                rhs.push_back(left);
            }

            if (((double) rand() / RAND_MAX) > final_rate)
            {
                Letter* Y;
                unsigned int to_id = rand() % nr_refuter_nonterminals + nr_prover_nonterminals;
                if (to_id < nr_refuter_nonterminals)
                {
                    Y = Nrefuter->get(to_id);
                }
                else
                {
                    to_id -= nr_refuter_nonterminals;
                    Y = Nprover->get(to_id);
                }
                rhs.push_back(Y);

            }

            if (((double) rand() / RAND_MAX) < right_rate)
            {
                unsigned int right_id = (unsigned int) (rand() % Sigma->size());
                Letter* right = Sigma->get(right_id);
                rhs.push_back(right);
            }

            G->addRule(X, rhs);
        }
    }
    return G;
}


