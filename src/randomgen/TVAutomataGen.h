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

#ifndef RIGG_TVAUTOMATAGEN_H
#define RIGG_TVAUTOMATAGEN_H

#include <random>
#include "../common/Alphabet.h"
#include "../common/Letter.h"
#include "../common/NFA.h"

/**
 * Creates a random Automaton according to Tabakov-Vardi-model
 */
class TVAutomataGen
{
    /**
     * the number of letters
     *
     * Note: the original paper by Tabakov and Vardi considers the case of a binary alphabet
     */
    unsigned int numberLetters;

    /**
     * the number of states of the automaton
     */
    unsigned int numberStates;

    /**
     * the density of the transition relation, i.e.
     * if we have numberStates many states, then we expect there for each letter a to be
     * numberStates * density many transitions in total.
     *
     * The total number of transitions will be numberStates * density * numberLetters
     */
    double density;

    /**
     * the rate of states that are final
     *
     * Note: in the original paper by Tabakov and Vardi, the initial state is always final.
     * this is not true here
     */
    double rateOfFinals;

public:
    TVAutomataGen (unsigned int numberLetters, unsigned int numberStates, double density,
                   double rateOfFinals);

    /**
     * @param sourceOfRandomness a source of randomness
     * @return  a randomly generated NFA with the specified parameters
     */
    NFA* generate (mt19937& sourceOfRandomness);

    TVAutomataGen (TVAutomataGen const&) = delete;

    TVAutomataGen& operator= (TVAutomataGen const&) = delete;

};

#endif //RIGG_TVAUTOMATAGEN_H
