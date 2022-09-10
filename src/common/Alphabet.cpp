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

#include "Alphabet.h"

using namespace std;

string Alphabet::toString () const
{
    bool first = true;
    string res;

    for (Letter* l : letters)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            res.append(",");
        }
        res.append(l->toString());
    }
    return res;
}


Letter* Alphabet::addLetter (string name)
{
    auto* l = new Letter(this, size(), name);
    letters.push_back(l);
    return l;
}

Alphabet::~Alphabet ()
{
    for (Letter* q : letters)
    {
        delete q;
    }
}

string Alphabet::wordToString (const vector<Letter*>& word)
{
    if (word.empty())
    {
        return "epsilon";
    }

    string res;
    for (Letter* letter : word)
    {
        res.append(letter->toString());
    }
    return res;
}
