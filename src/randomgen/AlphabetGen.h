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

#ifndef RIGG_ALPHABETGEN_H
#define RIGG_ALPHABETGEN_H

#include "../common/Alphabet.h"

/**
 * Generates an alphabet with the desired number of letters
 */
class AlphabetGen
{
public:
    AlphabetGen (unsigned int numberLetters, string prefix);

    /**
     * Start generation and return generated alphabet
     */
    Alphabet* generate ();

    AlphabetGen (AlphabetGen const&) = delete;

    AlphabetGen& operator= (AlphabetGen const&) = delete;

private:
    const unsigned int numberLetters;

    const string prefix;
};

#endif //RIGG_ALPHABETGEN_H
