//
// Created by Sebastian on 13.07.2016.
//

#ifndef RIGG_ALPHABETGEN_H
#define RIGG_ALPHABETGEN_H

#include "../common/Alphabet.h"

/**
 * Generates an alphabet with the desired number of letters
 *
 * Only use each instance once!
 */
class AlphabetGen
{
public:
    AlphabetGen (unsigned int nr_letters, string prefix);

    /**
     * Start generation and return generated alphabet
     *
     * Use only once per instance!
     */
    Alphabet* generate ();

private:
    unsigned int nr_letters;

    string prefix;
};

#endif //RIGG_ALPHABETGEN_H
