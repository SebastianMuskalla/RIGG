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

#include "GamePDS.h"

using namespace std;

string GamePDS::toString () const
{
    string res;
    res.append("Stack alphabet: ");
    res.append(Gamma->toString());
    res.append("\nStates of the Universal Player: ");
    res.append(statesUniversal->toString());
    res.append("\nStates of the Existential Player: ");
    res.append(statesExistential->toString());
    res.append("\nTransition relation:");
    for (PDSTransition* t : transitions)
    {
        res.append("\n").append(t->toString());
    }
    return res;
}

