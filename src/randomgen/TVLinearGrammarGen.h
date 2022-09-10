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

//
// Created by Sebastian on 13.07.2016.
//

#ifndef RIGG_TVLINEARGRAMMARGEN_H
#define RIGG_TVLINEARGRAMMARGEN_H

#include <random>
#include "../common/Alphabet.h"
#include "../common/Letter.h"
#include "../common/GameGrammar.h"

/**
 * Creates a random linear Game Grammar similar to the Tabakov-Vardi-model for automata
 *
 * The rules of the grammar are of the shape X -> a Y b
 */
class TVLinearGrammarGen
{
private:
    Alphabet* Sigma;

    /**
     * number of nonterminals owned by the Universal Player
     */
    const unsigned int numberNonterminalsUniversal;

    /**
     * number of nonterminals owned by the Existential Player
     */
    const unsigned int numberNonterminalsExistential;

    /**
     * density of the production rules, specifying how many rules are there per pair of nonterminal,
     * e.g. with a density of one, the grammar would have (number of nonterminals)² many rules
     */
    const double density;

    /**
     * rate of the rules X -> eta where eta does not contain a non-terminal
     *
     * (1 - rateOfFinals) of the rules have a random non-terminal on their right hand side
     */
    const double rateOfFinals;

    /**
     * rate of the rules that have a random terminal symbol to the left of the potential non-terminal,
     * i.e. rules of the shape X -> a Y b, X -> a Y, X -> a b, X -> a
     */
    const double rateOfLeftSymbol;

    /**
     * rate of the rules that have a random terminal symbol to the right of the potential non-terminal,
     * i.e. rules of the shape X -> a Y b, X -> Y b, X -> a b, X -> b
     */
    const double rateOfRightSymbol;

public:
    /**
     * Specify terminals (Sigma), number of nonterminals for Universal and Existential Player,
     * density (number of outgoing rules for each nonterminal),
     * rateOfFinals (rate of rules that do not have a (random) non-terminal on their right-hand side),
     * rateOfLeftSymbol (rate of rules that have exactly one (random) terminal on the left)
     * rateOfRightSymbol (rate of rules that have exactly one (random) terminal on the right)
     */
    TVLinearGrammarGen (Alphabet* Sigma, unsigned int numberNonterminalsUniversal,
                        unsigned int numberNonterminalsExistential,
                        double density, double rateOfFinals, double rateOfLeftSymbol, double rateOfRightSymbol);

    /**
     * @param sourceOfRandomness a random generated
     * @return a randomly generated game grammar with the specified parameters
     */
    GameGrammar* generate (mt19937& sourceOfRandomness);

    TVLinearGrammarGen (TVLinearGrammarGen const&) = delete;

    TVLinearGrammarGen& operator= (TVLinearGrammarGen const&) = delete;

};

#endif //RIGG_TVLINEARGRAMMARGEN_H
