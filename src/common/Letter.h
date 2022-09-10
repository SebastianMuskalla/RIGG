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

#ifndef RIGG_LETTER_H
#define RIGG_LETTER_H

#include <string>

#include "Printable.h"

class Alphabet;

using namespace std;

/**
 * a single letter in a alphabet
 *
 * will be used for terminals, non-terminals, stack symbols and control states
 */
class Letter : public Printable<Letter>
{
    friend class Alphabet;

public:
    Alphabet* alphabet;
    unsigned long long int index;
    string name;

    /**
     * string representation
     */
    string toString () const override;

    Letter (Letter const&) = delete;

    Letter& operator= (Letter const&) = delete;

private:

    /**
     * constructor is private, will only be called by the method "addLetter" of the friend class Alphabet
     */
    Letter (Alphabet* alphabet, unsigned long long int index, string& name) :
            alphabet(alphabet),
            index(index),
            name(name)
    {}

};

#endif //RIGG_LETTER_H
