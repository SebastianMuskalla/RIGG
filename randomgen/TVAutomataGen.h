//
// Created by Sebastian on 13.07.2016.
//

#ifndef RIGG_TVAUTOMATAGEN_H
#define RIGG_TVAUTOMATAGEN_H


#include "../common/Alphabet.h"
#include "../common/Letter.h"
#include "../common/NFA.h"

class TVAutomataGen
{
    Alphabet* Sigma;
    Alphabet* Q;
    unsigned int nr_letters;
    unsigned int nr_states;
    double density;
    double final_rate;

    set<Letter*> final_states;

public:
    TVAutomataGen (unsigned int nr_letters, unsigned int nr_states, double density,
                   double final_rate) :
            nr_letters(nr_letters),
            nr_states(nr_states),
            density(density),
            final_rate(final_rate)
    { }

    NFA* generate ();
};


#endif //RIGG_TVAUTOMATAGEN_H
