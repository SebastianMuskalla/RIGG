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

#include "NFA.h"

#include <utility>
#include "PointeeComparator.h"

using namespace std;

NFA::NFA (Alphabet* Sigma, Alphabet* Q, Letter* initialState, set<Letter*> finalStates) :
        Sigma(Sigma),
        Q(Q),
        initialState(initialState),
        finalStates(std::move(finalStates))
{}

Transition* NFA::addTransition (Letter* source, Letter* label, Letter* target)
{
    auto* t = new Transition(source, label, target);
    transitions.emplace(t);
    return t;
}

Box* NFA::boxFor (Letter* a)
{
    auto itr = boxForLetter.find(a);
    if (itr != boxForLetter.end())
    {
        return itr->second;
    }

    // Box does not exist yet, create it
    Box* res = new Box(this, Q, a->name);
    for (Transition* t : transitions)
    {
        if (t->label == a)
        {
            res->content.emplace(t->source, t->target);
        }
    }
    boxForLetter.emplace(a, res);
    return res;
}

void NFA::addTransition (Transition* t)
{
    transitions.insert(t);
}

string NFA::toString () const
{
    string res;
    res.append("Input alphabet: ");
    res.append(Sigma->toString());
    res.append("\nStates: ");
    res.append(Q->toString());
    res.append("\nInitial state: ");
    res.append(initialState->toString());
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
    for (Transition* t : transitions)
    {
        res.append("\n").append(t->toString());
    }
    return res;
}

bool NFA::tryAddTransition (Letter* source, Letter* label, Letter* target)
{
    auto* t = new Transition(source, label, target);

    PointeeComparator<Transition> eq = PointeeComparator(t);

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

NFA::~NFA ()
{
    delete Q;
    for (Transition* t : transitions)
    {
        delete t;
    }
    for (Box* b : allBoxes)
    {
        delete b;
    }
}

void NFA::resetBoxes ()
{
    boxForLetter.clear();
    for (Box* b : allBoxes)
    {
        delete b;
    }
    allBoxes.clear();
}

Box* NFA::identityBox ()
{
    Box* res = new Box(this, Q, "id");
    for (Letter* q : Q->letters)
    {
        res->content.emplace(q, q);
    }
    return res;
}


