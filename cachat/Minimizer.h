//
// Created by Sebastian on 15.07.2016.
//

#ifndef RIGG_MINIMIZER_H
#define RIGG_MINIMIZER_H

#include "../common/Alphabet.h"
#include "../common/NFA.h"

class Minimizer
{
//P := {F, Q \ F};
//W := {F};
//while (W is not empty) do
//     choose and remove a set A from W
//     for each c in Σ do
//          let X be the set of states for which a transition on c leads to a state in A
//          for each set Y in P for which X ∩ Y is nonempty and Y \ X is nonempty do
//               replace Y in P by the two sets X ∩ Y and Y \ X
//               if Y is in W
//                    replace Y in W by the same two sets
//               else
//                    if |X ∩ Y| <= |Y \ X|
//                         add X ∩ Y to W
//                    else
//                         add Y \ X to W
//          end;
//     end;
//end;

    NFA* DFA;
    Alphabet* Sigma;
    Alphabet* Q;
    set<set<Letter*>> partition;
    vector<set<Letter*>> worklist;
    bool trivial = false;

public:
    Minimizer (NFA* DFA);

    NFA* minimize ();
};

#endif //RIGG_MINIMIZER_H
