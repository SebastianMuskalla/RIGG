/*
 * Copyright 2016-2022 Sebastian Muskalla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <random>
#include "TVLinearGrammarGen.h"
#include "AlphabetGen.h"

TVLinearGrammarGen::TVLinearGrammarGen (Alphabet* Sigma, unsigned int numberNonterminalsUniversal,
                                        unsigned int numberNonterminalsExistential,
                                        double density, double rateOfFinals, double rateOfLeftSymbol,
                                        double rateOfRightSymbol) :
        Sigma(Sigma),
        numberNonterminalsExistential(numberNonterminalsExistential),
        numberNonterminalsUniversal(numberNonterminalsUniversal),
        density(density),
        rateOfFinals(rateOfFinals),
        rateOfLeftSymbol(rateOfLeftSymbol),
        rateOfRightSymbol(rateOfRightSymbol)
{}

GameGrammar* TVLinearGrammarGen::generate (mt19937& sourceOfRandomness)
{
    Alphabet* NExistential = AlphabetGen(numberNonterminalsExistential, "X").generate();
    Alphabet* NUniversal = AlphabetGen(numberNonterminalsUniversal, "Y").generate();

    unsigned int totalNumberOfNonterminals = numberNonterminalsExistential + numberNonterminalsUniversal;

    auto* G = new GameGrammar(Sigma, NUniversal, NExistential);

    auto numberOfRulesPerNonterminal = (unsigned int) (lround(density * totalNumberOfNonterminals));

    uniform_real_distribution<double> rateDistribution(0.0, 1.0);
    uniform_int_distribution<unsigned int> terminalIndexDistribution(0, Sigma->size() - 1);
    uniform_int_distribution<unsigned int> nonterminalIndexDistribution(0, totalNumberOfNonterminals - 1);

    for (Letter* X : NExistential->letters)
    {
        for (int i = 0; i < numberOfRulesPerNonterminal; ++i)
        {
            vector<Letter*> rhs;
            if (rateDistribution(sourceOfRandomness) < rateOfLeftSymbol)
            {
                unsigned int id = terminalIndexDistribution(sourceOfRandomness);
                Letter* left = Sigma->get(id);
                rhs.push_back(left);
            }

            // the rate of finals is the rate of rules that do NOT have a non-terminal on their right
            if (rateDistribution(sourceOfRandomness) > rateOfFinals)
            {
                Letter* Y;
                unsigned int id = nonterminalIndexDistribution(sourceOfRandomness);

                if (id < numberNonterminalsExistential)
                {
                    Y = NExistential->get(id);
                }
                else
                {
                    id -= numberNonterminalsExistential;
                    Y = NUniversal->get(id);
                }
                rhs.push_back(Y);
            }

            if (rateDistribution(sourceOfRandomness) < rateOfRightSymbol)
            {
                unsigned int id = terminalIndexDistribution(sourceOfRandomness);
                Letter* right = Sigma->get(id);
                rhs.push_back(right);
            }

            G->addRule(X, rhs);
        }
    }
    for (Letter* X : NUniversal->letters)
    {
        for (int i = 0; i < numberOfRulesPerNonterminal; ++i)
        {
            vector<Letter*> rhs;
            if (rateDistribution(sourceOfRandomness) < rateOfLeftSymbol)
            {
                unsigned int id = terminalIndexDistribution(sourceOfRandomness);
                Letter* left = Sigma->get(id);
                rhs.push_back(left);
            }

            // the rate of finals is the rate of rules that do NOT have a non-terminal on their right
            if (rateDistribution(sourceOfRandomness) > rateOfFinals)
            {
                Letter* Y;
                unsigned int id = nonterminalIndexDistribution(sourceOfRandomness);
                if (id < numberNonterminalsExistential)
                {
                    Y = NExistential->get(id);
                }
                else
                {
                    id -= numberNonterminalsExistential;
                    Y = NUniversal->get(id);
                }
                rhs.push_back(Y);

            }

            if (rateDistribution(sourceOfRandomness) < rateOfRightSymbol)
            {
                unsigned int id = terminalIndexDistribution(sourceOfRandomness);
                Letter* right = Sigma->get(id);
                rhs.push_back(right);
            }

            G->addRule(X, rhs);
        }
    }
    return G;
}


