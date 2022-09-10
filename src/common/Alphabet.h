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


#ifndef RIGG_ALPHABET_H
#define RIGG_ALPHABET_H

#include <vector>
#include <string>
#include "Letter.h"


using namespace std;

/**
 * alphabet, i.e. list of letters
 */
class Alphabet : public Printable<Alphabet>
{
public:
    /**
     * the letters
     */
    vector<Letter*> letters;

    Alphabet ()
    = default;

    virtual ~Alphabet ();

    /**
     * number of letters currently in the alphabet
     */
    unsigned long long int size () const
    {
        return letters.size();
    }


    /**
     * get the letter with the specified index
     */
    Letter* get (unsigned int index) const
    {
        return letters[index];
    }

    /**
     * add a letter with the specified name
     */
    Letter* addLetter (string name);

    string toString () const override;

    Alphabet (Alphabet const&) = delete;

    Alphabet& operator= (Alphabet const&) = delete;

    static string wordToString (const vector<Letter*>& word);
};

#endif //RIGG_ALPHABET_H
