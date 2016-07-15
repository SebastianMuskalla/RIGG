//
// Created by Sebastian on 13.07.2016.
//

#ifndef RIGG_TVAUTOMATAGEN_H
#define RIGG_TVAUTOMATAGEN_H

#include "../common/Alphabet.h"
#include "../common/Letter.h"
#include "../common/NFA.h"

/**
 * Creates a random Automaton according to Tabakov-Vardi-model
 *
 * Use each instance only once!
 */
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
                   double final_rate);

    /**
     * Start generation and return generated NFA
     *
     * Only use once per instance!
     */
    NFA* generate ();

    TVAutomataGen (TVAutomataGen const &) = delete;

    TVAutomataGen &operator= (TVAutomataGen const &) = delete;
};

#endif //RIGG_TVAUTOMATAGEN_H
