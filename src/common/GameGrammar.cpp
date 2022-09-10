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

#include "GameGrammar.h"

using namespace std;

string GameGrammar::toString () const
{
    string res;
    res.append("Terminals: ");
    res.append(Sigma->toString());
    res.append("\nNonterminals of the Universal Player: ");
    res.append(NUniversal->toString());
    res.append("\nNonterminals of the Existential Player: ");
    res.append(NExistential->toString());
    res.append("\nProduction rules:");
    for (pair<Letter*, vector<Letter*>> rule : rules)
    {
        res.append("\n");
        res.append(rule.first->toString());
        res.append(" -> ");
        res.append(Alphabet::wordToString(rule.second));
    }
    return res;
}

GameGrammar::~GameGrammar ()
{
    delete NExistential;
    delete NUniversal;
}
