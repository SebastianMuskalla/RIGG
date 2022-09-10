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

#include "Formula.h"

using namespace std;

Formula* Formula::composeWith (Formula* G)
{
    if (this->isFalse() || G->isFalse())
    {
        return falseFormula(storage);
    }

    auto* res = new Formula(storage);
    for (Clause* c : clauses)
    {
        vector<Clause*> tmp = c->composeWith(G);
        res->clauses.insert(res->clauses.end(), tmp.begin(), tmp.end());
    }
    return res;
}

string Formula::toString () const
{
    if (isFalse())
    {
        return "FALSE";
    }

    string res;
    bool first = true;
    for (Clause* c : clauses)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            res.append(" and ");
        }
        res.append(c->toString());
    }
    return res;
}

bool Formula::isRejecting ()
{
    if (rejecting == YES)
    {
        return true;
    }
    if (rejecting == NO)
    {
        return false;
    }

    for (Clause* c : clauses)
    {
        if (!c->isRejecting())
        {
            rejecting = NO;
            return false;
        }
    }
    rejecting = YES;
    return true;
}

bool Formula::implies (Formula* G)
{
    for (Clause* c : G->clauses)
    {
        bool foundContained = false;
        for (auto itr = clauses.begin(); !foundContained && itr != clauses.end(); ++itr)
        {
            if (c->contains(*itr))
            {
                foundContained = true;
            }
        }
        if (!foundContained)
        {
            return false;
        }
    }
    return true;
}

Formula* Formula::wrap (Box* box, FormulaStorage* storage)
{
    Clause* cl = Clause::wrap(box);
    return wrap(cl, storage);
}

Formula* Formula::wrap (Clause* c, FormulaStorage* storage)
{
    auto* res = new Formula(storage);
    res->clauses.push_back(c);
    return res;
}

Formula* Formula::formulaAnd (Formula* other)
{
    if (isFalse() || other->isFalse())
    {
        return Formula::falseFormula(storage);
    }

    auto* res = new Formula(storage);
    res->clauses.reserve(clauses.size() + other->clauses.size());

    for (Clause* c : clauses)
    {
        res->clauses.emplace_back(new Clause(*c));
    }
    for (Clause* c : other->clauses)
    {
        res->clauses.emplace_back(new Clause(*c));
    }
    return res;
}

Formula* Formula::formulaOr (Formula* other)
{
    if (isFalse())
    {
        return other;
    }
    if (other->isFalse())
    {
        return this;
    }

    auto* res = new Formula(storage);
    res->clauses.reserve(clauses.size() * other->clauses.size());

    for (Clause* c1 : clauses)
    {
        for (Clause* c2 : other->clauses)
        {
            auto* c = new Clause();
            c->boxes.insert(c->boxes.end(), c1->boxes.begin(), c1->boxes.end());
            c->boxes.insert(c->boxes.end(), c2->boxes.begin(), c2->boxes.end());
            res->clauses.push_back(c);
        }
    }
    return res;
}

unordered_map<FormulaStorage*, Formula*> Formula::FALSE_FORMULA;

Formula* Formula::falseFormula (FormulaStorage* storage)
{
    if (FALSE_FORMULA.find(storage) == FALSE_FORMULA.end())
    {
        auto* res = new Formula(storage);
        auto* c = new Clause();
        res->clauses.push_back(c);
        FALSE_FORMULA[storage] = res;
        return res;
    }
    else
    {
        return FALSE_FORMULA[storage];
    }
}

bool Formula::isFalse () const
{
    if (unsatisfiable == YES)
    {
        return true;
    }
    if (unsatisfiable == NO)
    {
        return false;
    }

    if (clauses.empty())
    {
        // this should never happen because TRUE will never occur as formula
        throw exception();
    }

    for (Clause* c : clauses)
    {
        if (c->boxes.empty())
        {
            unsatisfiable = YES;
            return true;
        }
    }
    unsatisfiable = NO;
    return false;
}

Formula* Formula::simplify ()
{

    vector<Clause*> minimalClauses;

    for (Clause* c : clauses)
    {
        bool minimal = true;
        for (auto itr = clauses.begin(); minimal && itr != clauses.end(); ++itr)
        {
            if (c != *itr && c->contains(*itr))
            {
                minimal = false;
            }
        }
        if (minimal)
        {
            minimalClauses.push_back(c);
        }
    }

    clauses = minimalClauses;

    // we don't need to do this, because subsumption retains the properties
    // resetMemoization();

    return this;
}

Formula::~Formula ()
{
    for (Clause* c : clauses)
    {
        delete c;
    }
}
