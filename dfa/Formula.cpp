#include "Formula.h"

using namespace std;

Formula* Formula::composeWith (Formula* G)
{
    if (this->isFalse() || G->isFalse())
    {
        return falseFormula(storage);
    }

    Formula* res = new Formula(storage);
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

    string res = "";
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
        bool find_contained = false;
        for (auto itr = clauses.begin(); !find_contained && itr != clauses.end(); ++itr)
        {
            if (c->contains(*itr))
            {
                find_contained = true;
            }
        }
        if (!find_contained)
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
    Formula* res = new Formula(storage);
    res->clauses.push_back(c);
    return res;
}

Formula* Formula::formulaAnd (Formula* other)
{
    if (isFalse() || other->isFalse())
    {
        return Formula::falseFormula(storage);
    }

    Formula* res = new Formula(storage);

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

    Formula* res = new Formula(storage);
    for (Clause* c1 : clauses)
    {
        for (Clause* c2 : other->clauses)
        {
            Clause* c = new Clause();
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
        Formula* res = new Formula(storage);
        Clause* c = new Clause();
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
    if (is_false == YES)
    {
        return true;
    }
    if (is_false == NO)
    {
        return false;
    }

    if (clauses.empty())
    {
        throw new string("Formula TRUE - Should not occur!");
    }

    for (Clause* c : clauses)
    {
        if (c->boxes.empty())
        {
            is_false = YES;
            return true;
        }
    }
    is_false = NO;
    return false;
}

Formula* Formula::simplify ()
{

    vector<Clause*> minimal_clauses;

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
            minimal_clauses.push_back(c);
        }
    }

    clauses = minimal_clauses;

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
