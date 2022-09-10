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

#include "Box.h"

#include <utility>

using namespace std;

Box* Box::composeWith (Box* other)
{
    string newName = name;
    newName.append(other->name);

    if (name == "id")
    {
        newName = other->name;
    }
    if (other->name == "id")
    {
        newName = name;
    }

    Box* res = new Box(A, Q, newName);
    for (pair<Letter*, Letter*> qp : content)
    {
        auto range = other->content.equal_range(qp.second);
        for (auto pr = range.first; pr != range.second; ++pr)
        {
            res->content.emplace(qp.first, pr->second);
        }
    }
    return res;
}

vector<Box*> Box::composeWith (Clause* r)
{
    vector<Box*> res;
    res.reserve(r->boxes.size());
    for (Box* b : r->boxes)
    {
        res.push_back(composeWith(b));
    }
    return res;
}

string Box::toString () const
{
    string s = "[";
    s.append(name);
    s.append(": ");
    bool first = true;
    for (auto pair : content)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            s.append(",");
        }

        s.append(pair.first->toString()).append("->").append(pair.second->toString());
    }
    s.append("]");
    return s;
}

bool Box::isRejecting ()
{
    if (rejecting == YES)
    {
        return true;
    }
    if (rejecting == NO)
    {
        return false;
    }

    Letter* init = A->initialState;
    auto bounds = content.equal_range(init);

    for (auto itr = bounds.first; itr != bounds.second; ++itr)
    {
        if (A->finalStates.find(itr->second) != A->finalStates.end())
        {
            rejecting = NO;
            return false;
        }
    }
    rejecting = YES;
    return true;
}

Box::Box (NFA* A, Alphabet* Q, string name) :
        A(A),
        Q(Q),
        name(std::move(name))
{
    A->allBoxes.insert(this);
}
