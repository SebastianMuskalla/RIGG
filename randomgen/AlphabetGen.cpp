//
// Created by Sebastian on 13.07.2016.
//

#include "AlphabetGen.h"

Alphabet* AlphabetGen::generate ()
{
    Alphabet* res = new Alphabet();
    for (int i = 0; i < nr_letters; i++)
    {
        string name = prefix;
        name.append(to_string(i));
        res->addLetter(name);
    }
    return res;
}

