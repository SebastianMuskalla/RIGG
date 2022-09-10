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

#include "Clause.h"

using namespace std;

vector<Clause*> Clause::composeWith (Formula* G)
{
    vector<Clause*> res;

    unsigned long long int numberBoxes = boxes.size();
    auto* mapping = new unsigned long long int[numberBoxes]();
    unsigned long long int numberClauses = G->clauses.size();
    unsigned long long int initialIndex = 0;

    while (true)
    {
        auto* clause = new Clause();

        for (unsigned long long int i = 0; i < numberBoxes; ++i)
        {
            Box* rho = boxes.at(i);
            Clause* H = G->clauses.at(mapping[i]);
            vector<Box*> tmp = rho->composeWith(H);

            clause->boxes.insert(clause->boxes.end(), tmp.begin(), tmp.end());
        }

        res.push_back(clause);

        initialIndex = increment(mapping, numberBoxes, numberClauses - 1, initialIndex);

        if (initialIndex == numberBoxes)
        {
            goto _postLoop;
        }

    }
    _postLoop:
    delete[] mapping;
    return res;
}

unsigned long long int
Clause::increment (unsigned long long int* func, unsigned long long int k, unsigned long long int g,
                   unsigned long long int start)
{
    bool reset = false;

    // assert(hint >= 0);
    for (unsigned long long int i = start; i < k; ++i)
    {
        if (func[i] < g)
        {
            ++func[i];

            if (reset)
            {
                for (int j = 0; j < i; ++j)
                {
                    func[j] = 0;
                }
                return 0;
            }
            else
            {
                return i;
            }
        }
        else
        {
            reset = true;
        }
    }
    return k;
}

string Clause::toString () const
{
    string s = "(";
    bool first = true;
    for (Box* box : boxes)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            s.append(" or ");
        }
        s.append(box->toString());
    }
    s.append(")");
    return s;
}

bool Clause::isRejecting ()
{
    if (rejecting == YES)
    {
        return true;
    }
    if (rejecting == NO)
    {
        return false;
    }

    for (Box* b : boxes)
    {
        if (b->isRejecting())
        {
            rejecting = YES;
            return true;
        }
    }
    rejecting = NO;
    return false;
}

bool Clause::contains (Clause* c)
{
    for (Box* b : c->boxes)
    {
        bool found = false;
        for (auto itr = boxes.begin(); !found && itr != boxes.end(); ++itr)
        {
            if ((**itr) == *b)
            {
                found = true;
            }
        }
        if (!found)
        {
            return false;
        }
    }
    return true;
}

Clause* Clause::wrap (Box* box)
{
    auto* c = new Clause();
    c->boxes.push_back(box);
    return c;
}












