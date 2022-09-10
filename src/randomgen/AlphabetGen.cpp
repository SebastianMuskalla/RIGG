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

#include "AlphabetGen.h"

#include <utility>

Alphabet* AlphabetGen::generate ()
{
    auto* res = new Alphabet();
    for (int i = 0; i < numberLetters; i++)
    {
        string name = prefix;
        name.append(to_string(i));
        res->addLetter(name);
    }
    return res;
}

AlphabetGen::AlphabetGen (unsigned int numberLetters, string prefix) :
        numberLetters(numberLetters),
        prefix(std::move(prefix))
{}
