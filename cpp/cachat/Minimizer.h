//
// Created by Sebastian on 15.07.2016.
//

#ifndef RIGG_MINIMIZER_H
#define RIGG_MINIMIZER_H

#include "../common/Alphabet.h"
#include "../common/NFA.h"

/**
 * given an DFA with no unreachable states (e.g. one produced by "Determinizer"), compute the (up to isomorphy) unique minimal DFA for the language of the given DFA, using Hopcrofts algorithm as specified by the following pseudo-code (taken from wikipedia)
 *
 * P := {F, Q \ F};
 * W := {F};
 * while (W is not empty) do
 *      choose and remove a set A from W
 *      for each c in Σ do
 *           let X be the set of states for which a transition on c leads to a state in A
 *           for each set Y in P for which X ∩ Y is nonempty and Y \ X is nonempty do
 *                replace Y in P by the two sets X ∩ Y and Y \ X
 *                if Y is in W
 *                     replace Y in W by the same two sets
 *                else
 *                     if |X ∩ Y| <= |Y \ X|
 *                          add X ∩ Y to W
 *                     else
 *                         add Y \ X to W
 *            end;
 *      end;
 * end;
 */
class Minimizer
{
    NFA* DFA;
    Alphabet* Sigma;
    Alphabet* Q;
    set<set<Letter*>> partition;
    vector<set<Letter*>> worklist;
    bool trivial = false;

public:
    /**
     * DFA to minimize (it is the obligation of the caller to guarantee that the NFA is deterministic)
     */
    Minimizer (NFA* DFA);

    /**
     * Minimize it and return the minimal DFA
     */
    NFA* minimize ();

    Minimizer (Minimizer const &) = delete;

    Minimizer &operator= (Minimizer const &) = delete;
};

#endif //RIGG_MINIMIZER_H
