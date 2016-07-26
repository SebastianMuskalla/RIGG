#include "Clause.h"

using namespace std;

vector<Clause*> Clause::composeWith (Formula* G)
{
    vector<Clause*> res;

    uint64 nr_boxes = boxes.size();
    uint64* mapping = new uint64[nr_boxes]();
    uint64 nr_clauses = G->clauses.size();
    uint64 start_index = 0;

    while (true)
    {
        Clause* clause = new Clause();

        for (uint64 i = 0; i < nr_boxes; ++i)
        {
            Box* rho = boxes.at(i);
            Clause* H = G->clauses.at(mapping[i]);
            vector<Box*> tmp = rho->composeWith(H);

            clause->boxes.insert(clause->boxes.end(), tmp.begin(), tmp.end());
        }

        res.push_back(clause);

        start_index = increment(mapping, nr_boxes, nr_clauses - 1, start_index);

        if (start_index == nr_boxes)
        {
            goto _postloop;
        }

    }
    _postloop:
    delete[] mapping;
    return res;
}

uint64 Clause::increment (uint64* func, uint64 k, uint64 g,
                          uint64 start)
{
    bool reset = false;

    // assert(hint >= 0);
    for (uint64 i = start; i < k; ++i)
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
    Clause* c = new Clause();
    c->boxes.push_back(box);
    return c;
}












