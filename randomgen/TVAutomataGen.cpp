//
// Created by Sebastian on 13.07.2016.
//

#include "TVAutomataGen.h"
#include "../common/NFA.h"
#include "AlphabetGen.h"

NFA* TVAutomataGen::generate ()
{

    Sigma = AlphabetGen(nr_letters, "a").generate();
    Q = AlphabetGen(nr_states, "q").generate();

    int final_bound = (int) (nr_states * final_rate);

    for (unsigned int i = final_bound; i < nr_states; i++)
    {
        final_states.insert(Q->get(i));
    }

    unsigned int initial_state_index = rand() % nr_states;

    Letter* initial_state = Q->get(initial_state_index);

    NFA* A = new NFA(Sigma, Q, initial_state, final_states);

    int transitions_per_letter = (int) (nr_states * density);
    for (Letter* a : Sigma->letters)
    {
        int todo = transitions_per_letter;
        while (todo != 0)
        {
            unsigned int source_index = rand() % nr_states;
            unsigned int target_index = rand() % nr_states;

            if (A->tryAddTransition(Q->get(source_index), a, Q->get(target_index)))
            {
                todo--;
            }
        }
    }
    return A;

}

TVAutomataGen::TVAutomataGen (unsigned int nr_letters, unsigned int nr_states, double density,
                              double final_rate) :
        nr_letters(nr_letters),
        nr_states(nr_states),
        density(density),
        final_rate(final_rate)
{ }
