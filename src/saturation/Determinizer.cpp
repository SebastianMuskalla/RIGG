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

#include "Determinizer.h"

using namespace std;

string Determinizer::setToString (const set<Letter*>& set)
{
    string res = "{";
    bool first = true;
    for (Letter* l : set)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            res.append(",");
        }
        res.append(l->toString());
    }
    res.append("}");
    return res;
}

NFA* Determinizer::determinize (NFA* A)
{
    Alphabet* Sigma = A->Sigma;
    set<Transition*> nfaTransitions = A->transitions;

    // set of states of the DFA
    auto* powerset = new Alphabet();

    // initial state of the DFA is the set {q_init}
    set<Letter*> initialStateAsSet;
    initialStateAsSet.insert(A->initialState);

    // maps sets of NFA states to DFA states
    map<set<Letter*>, Letter*> setToState;

    Letter* initialStateAsLetter = powerset->addLetter(setToString(initialStateAsSet));
    setToState.emplace(initialStateAsSet, initialStateAsLetter);

    // worklist of the DFA states that have to be processed
    set<set<Letter*>> worklist;
    worklist.insert(initialStateAsSet);

    // states that have been processed
    set<set<Letter*>> done;

    // transition relations of the DFA
    set<Transition*> dfaTransitions;

    // final states of the DFA
    set<Letter*> dfaFinalStates;
    // make the initial state a final state
    if (A->finalStates.find(A->initialState) != A->finalStates.end())
    {
        dfaFinalStates.insert(initialStateAsLetter);
    }

    while (!worklist.empty())
    {
        // pick state set from worklist for which transitions have not yet been processed
        auto sourceSet = *worklist.begin();
        worklist.erase(worklist.begin());
        done.insert(sourceSet);


        Letter* sourceLetter = setToState[sourceSet];

        for (Letter* a : Sigma->letters)
        {

            set<Letter*> targetSet;
            bool isFinal = false;

            // construct target set for given source set and letter
            for (Transition* t : nfaTransitions)
            {
                if (t->label == a && sourceSet.find(t->source) != sourceSet.end())
                {
                    targetSet.insert(t->target);

                    if (A->finalStates.find(t->target) != A->finalStates.end())
                    {
                        isFinal = true;
                    }
                }
            }

            // check if target set has already an associated letter
            // if not, create it, and add target set to worklist
            auto targetLetterItr = setToState.find(targetSet);
            Letter* targetLetter;
            if (targetLetterItr == setToState.end())
            {
                targetLetter = powerset->addLetter(setToString(targetSet));
                setToState.emplace(targetSet, targetLetter);
                worklist.insert(targetSet);

                if (isFinal)
                {
                    dfaFinalStates.insert(targetLetter);
                }
            }
            else
            {
                targetLetter = targetLetterItr->second;
            }


            dfaTransitions.insert(new Transition(sourceLetter, a, targetLetter));
        }

    }

    NFA* dfa = new NFA(Sigma, powerset, initialStateAsLetter, dfaFinalStates);
    for (Transition* t : dfaTransitions)
    {
        dfa->addTransition(t);
    }
    return dfa;
}
