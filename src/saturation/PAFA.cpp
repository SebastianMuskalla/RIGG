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

#include "PAFA.h"

#include "../common/PointeeComparator.h"

using namespace std;

bool PAFA::acceptsFromControlState (Letter* controlState, vector<Letter*> word)
{
    set<Letter*> S = finalStates;

    // backwards search
    for (auto reverseItr = word.rbegin(); reverseItr != word.rend(); ++reverseItr)
    {
        if (S.empty())
        {
            return false;
        }

        set<Letter*> newS;
        for (AFATransition* t : transitions)
        {
            if (t->label == *reverseItr)
            {
                // check that all target states are contained in the old S
                for (Letter* x : t->targets)
                {
                    if (S.find(x) == S.end())
                    {
                        goto _continue_label;
                    }
                }

                // ... if yes, insert the source state into the new S
                newS.insert(t->source);
            }

            _continue_label:;
        }

        S = newS;
    }
    return (S.find(controlState) != S.end());
}

set<set<Letter*>> PAFA::reachableFromControlState (Letter* controlState, const vector<Letter*>& word)
{
    // disjunction of sets of states, each inner set representing a conjunction
    set<set<Letter*>> current;
    set<set<Letter*>> next;

    set<Letter*> init;
    init.insert(controlState);
    current.insert(init);

    for (Letter* a : word)
    {
        next.clear();

        for (const set<Letter*>& conjunction : current)
        {
            // we compute the successors for one conjunction at the time
            set<set<Letter*>> innerCurrent;
            set<set<Letter*>> innerNext;
            innerCurrent.insert(set<Letter*>());

            for (Letter* state : conjunction)
            {
                // iterate over all successors
                for (AFATransition* t : transitions)
                {
                    if (t->label == a && t->source == state)
                    {
                        for (const set<Letter*>& innerSet : innerCurrent)
                        {
                            set<Letter*> copy(innerSet);
                            copy.insert(t->targets.begin(), t->targets.end());
                            innerNext.insert(copy);
                        }
                    }
                }
                innerCurrent = innerNext;
                innerNext.clear();
            }

            for (const set<Letter*>& innerSet : innerCurrent)
            {
                if (!innerSet.empty())
                {
                    next.insert(innerSet);
                }
            }

        }

        current = next;
    }
    return current;
}

string PAFA::toString () const
{
    string res;
    res.append("Alphabet: ");
    res.append(Gamma->toString());
    res.append("\nStates: ");
    res.append(controlStates->toString());
    res.append("\nFinal states: ");

    bool first = true;
    for (Letter* f : finalStates)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            res.append(",");
        }
        res.append(f->toString());
    }

    res.append("\nTransition relation:");
    for (AFATransition* t : transitions)
    {
        res.append("\n").append(t->toString());
    }

    return res;
}

bool PAFA::addTransition (Letter* source, Letter* label, const set<Letter*>& targets)
{
    // adding transitions with empty target set is not very helpful
    if (targets.empty())
    {
        return false;
    }

    auto* t = new AFATransition(source, label, targets);

    PointeeComparator<AFATransition> eq = PointeeComparator(t);

    auto itr = find_if(transitions.begin(), transitions.end(), eq);

    if (itr == transitions.end())
    {
        transitions.insert(t);
        return true;
    }
    else
    {
        delete t;
        return false;
    }

}

PAFA::PAFA (GamePDS* P) :
        Gamma(P->Gamma),
        P(P),
        controlStates(),
        pdsStateToAFAState(),
        finalStates(),
        transitions()
{}

PAFA::~PAFA ()
{
    delete controlStates;
    for (AFATransition* t : transitions)
    {
        delete t;
    }
}
