#include "Box.h"

using namespace std;

Box* Box::composeWith (Box* r)
{
    Box* res = new Box(A, Q, this->name + r->name);
    for (pair<Letter*, Letter*> cnt : content)
    {
        auto range = r->content.equal_range(cnt.second);
        for (auto itr = range.first; itr != range.second; ++itr)
        {
            res->content.emplace(cnt.first, itr->second);
        }
    }
    return res;
}

vector<Box*> Box::composeWith (Clause* r)
{
    vector<Box*> res;
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

    Letter* init = A->initial_state;
    auto bounds = content.equal_range(init);

    for (auto itr = bounds.first; itr != bounds.second; ++itr)
    {
        if (A->final_states.find(itr->second) != A->final_states.end())
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
        name(name)
{ }
