#include "Formula.h"

using namespace std;

Formula* Formula::composeWith (Formula* G)
{
    if (this->isFalse() || G->isFalse())
    {
        return falseFormula();
    }

    Formula* res = new Formula();
    for (Clause* c : clauses)
    {
        vector<Clause*> tmp = c->composeWith(G);
        res->clauses.insert(res->clauses.end(), tmp.begin(), tmp.end());
    }
    return res;
}

string Formula::toString () const
{
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
        return true;
    if (rejecting == NO)
        return false;

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
        for (auto itr = clauses.begin(); itr != clauses.end() && !find_contained; ++itr)
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

Formula* Formula::wrap (Box* box)
{
    Clause* cl = Clause::wrap(box);
    return wrap(cl);

}

Formula* Formula::wrap (Clause* c)
{
    Formula* res = new Formula();
    res->clauses.push_back(c);
}

Formula* Formula::formulaAnd (Formula* other)
{
    Formula* res = new Formula();
    res->clauses.insert(res->clauses.end(), this->clauses.begin(), this->clauses.end());
    res->clauses.insert(res->clauses.end(), other->clauses.begin(), other->clauses.end());
    return res;
}

Formula* Formula::formulaOr (Formula* other)
{
    Formula* res = new Formula();
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

Formula* Formula::FALSE_FORMULA = nullptr;

Formula* Formula::falseFormula ()
{
    if (FALSE_FORMULA == nullptr)
    {
        Formula* res = new Formula();
        Clause* c = new Clause();
        res->clauses.push_back(c);
        FALSE_FORMULA = res;
        return res;
    }
    else
    {
        return FALSE_FORMULA;
    }
}

bool Formula::isFalse ()
{
    if (clauses.empty())
    {
        throw new string("HODENKOBOLD!");
    }

    for (Clause* c : clauses)
    {
        if (c->boxes.empty())
        {
            return true;
        }
    }
    return false;
}














