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

//
// Created by Sebastian on 15.07.2016.
//

#include "Minimizer.h"

NFA* Minimizer::minimize (NFA* DFA)
{
    Alphabet* Sigma = DFA->Sigma;
    Alphabet* Q = DFA->Q;

    // partition of the DFA states into sets of states
    set<set<Letter*>> partition;

    // worklist of the sets of states in the current partition that have to be considered
    vector<set<Letter*>> worklist;

    // create the initial partition of final states F and non-final states
    set<Letter*> F;
    set<Letter*> nonF;

    for (Letter* q : Q->letters)
    {

        if (DFA->finalStates.find(q) != DFA->finalStates.end())
        {
            F.insert(q);
        }
        else
        {
            nonF.insert(q);
        }
    }

    // if there is no final state, we can return a trivial DFA that rejects every input
    if (F.empty())
    {
        auto* singleton = new Alphabet();
        Letter* q = singleton->addLetter("q");
        NFA* dfaWithEmptyLanguage = new NFA(Sigma, singleton, q, set<Letter*>());
        for (Letter* a : Sigma->letters)
        {
            dfaWithEmptyLanguage->addTransition(q, a, q);
        }
        return dfaWithEmptyLanguage;
    }

    // otherwise, we have to create the partition and initialize the worklist with F
    partition.insert(F);

    if (!nonF.empty())
    {
        partition.insert(nonF);
    }

    worklist.push_back(F);


    while (!worklist.empty())
    {
        // pick state set setA from worklist
        set<Letter*> setA = *worklist.begin();
        worklist.erase(worklist.begin());

        // for each a in Sigma do
        for (Letter* a : Sigma->letters)
        {
            // let setX be the set of states for which a transition on c leads to a state in setA
            set<Letter*> setX;
            for (Transition* t : DFA->transitions)
            {
                if (t->label == a && setA.find(t->target) != setA.end())
                {
                    setX.insert(t->source);
                }
            }

            // if setX is empty, the intersection XcapY will also be empty
            if (!setX.empty())
            {
                // for each set Y in the partition ...
                for (auto itr = partition.begin(); itr != partition.end();)
                {
                    set<Letter*> setY = *itr;
                    set<Letter*> intersection; // intersection of X and Y
                    set<Letter*> difference; // difference Y minus X

                    for (Letter* q : setY)
                    {
                        if (setX.find(q) != setX.end())
                        {
                            intersection.insert(q);
                        }
                        else
                        {
                            difference.insert(q);
                        }
                    }

                    // ... for which  intersection and difference are nonempty
                    if (!intersection.empty() && !difference.empty())
                    {
                        // replace setY in P by the two sets setX ∩ setY and setY \ setX

                        // we need to be careful to not invalidate itr
                        auto newItr = itr;
                        ++newItr;
                        partition.erase(itr);
                        itr = newItr;

                        partition.insert(intersection);
                        partition.insert(difference);

                        auto worklistItr = find(worklist.begin(), worklist.end(), setY);
                        if (worklistItr != worklist.end())
                        {
                            // if setY is in W
                            // replace setY in W by the same two sets
                            worklist.erase(worklistItr);
                            worklist.push_back(intersection);
                            worklist.push_back(difference);
                        }
                        else
                        {
                            // else add the smaller of the two sets
                            if (intersection.size() <= difference.size())
                            {
                                worklist.push_back(intersection);
                            }
                            else
                            {
                                worklist.push_back(difference);
                            }
                        }
                    }
                    else
                    {
                        ++itr;
                    }
                }
            }
        } // a in Sigma
    } // while worklist not empty

    // we have obtained the final partition, we can now construct the minimal DFA

    // convert the partition to a new set of states
    auto* newStates = new Alphabet();
    set<Letter*> newFinalStates;
    map<Letter*, Letter*> oldStateToNewState;

    for (const set<Letter*>& setQ : partition)
    {
        string name = "[";
        for (Letter* l : setQ)
        {
            name.append(l->toString());
        }

        name.append("]");

        Letter* state = newStates->addLetter(name);

        for (Letter* l : setQ)
        {
            oldStateToNewState[l] = state;
        }

        // if one state in setQ is final, all states in setQ are final
        if (DFA->finalStates.find(*setQ.begin()) != DFA->finalStates.end())
        {
            newFinalStates.insert(state);
        }
    }

    Letter* newInitialState = oldStateToNewState[DFA->initialState];

    NFA* minDFA = new NFA(Sigma, newStates, newInitialState, newFinalStates);

    // remap the transitions
    // the algorithm makes sure that for each set of states in the partition, we just need to consider one transition per letter
    for (const set<Letter*>& setQ : partition)
    {
        Letter* sourceState = oldStateToNewState[*setQ.begin()];

        for (Transition* t : DFA->transitions)
        {
            if (t->source == *setQ.begin())
            {
                Letter* target = oldStateToNewState[t->target];
                minDFA->addTransition(sourceState, t->label, target);
            }
        }

    }

    return minDFA;
}
