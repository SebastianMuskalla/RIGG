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

#include "TVAutomataGen.h"
#include "AlphabetGen.h"

NFA* TVAutomataGen::generate (mt19937& sourceOfRandomness)
{

    Alphabet* Sigma = AlphabetGen(numberLetters, "a").generate();
    Alphabet* Q = AlphabetGen(numberStates, "q").generate();

    set<Letter*> finalStates;

    // compute the index i such that states [a0 ... a(i-1)] are final and [ai ... an] are non-final
    auto finalBound = (unsigned int) (lround(numberStates * rateOfFinals));

    for (unsigned int i = 0; i < min(finalBound, numberStates); i++)
    {
        finalStates.insert(Q->get(i));
    }

    uniform_int_distribution<unsigned int> stateDistribution(0, numberStates - 1);
    unsigned int indexInitialState = stateDistribution(sourceOfRandomness);

    Letter* initialState = Q->get(indexInitialState);

    NFA* A = new NFA(Sigma, Q, initialState, finalStates);

    auto transitionsPerLetter = (unsigned int) (lround(numberStates * density));

    for (Letter* a : Sigma->letters)
    {
        unsigned int todo = min(transitionsPerLetter, numberStates * numberStates);

        while (todo != 0)
        {
            unsigned int indexSource = stateDistribution(sourceOfRandomness);
            unsigned int indexTarget = stateDistribution(sourceOfRandomness);

            // try to add a transition, this only succeeds if it doesn't already exist
            // this will be inefficient if the density is very high
            if (A->tryAddTransition(Q->get(indexSource), a, Q->get(indexTarget)))
            {
                todo--;
            }
        }
    }
    return A;

}

TVAutomataGen::TVAutomataGen (unsigned int numberLetters, unsigned int numberStates, double density,
                              double rateOfFinals) :
        numberLetters(numberLetters),
        numberStates(numberStates),
        density(density),
        rateOfFinals(rateOfFinals)
{}