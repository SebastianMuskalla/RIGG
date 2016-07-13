//
// Created by Sebastian on 13.07.2016.
//

#ifndef RIGG_ALPHABETGEN_H
#define RIGG_ALPHABETGEN_H


#include "../common/Alphabet.h"

class AlphabetGen
{
public:
    AlphabetGen (unsigned int nr_letters, string prefix) :
            nr_letters(nr_letters),
            prefix(prefix)
    { }

    Alphabet* generate ();

private:
    unsigned int nr_letters;

    string prefix;

};


#endif //RIGG_ALPHABETGEN_H
