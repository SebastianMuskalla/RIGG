#include "Clause.h"

using namespace std;


vector<Clause*> Clause::composeWith (Formula* G)
{
    vector<Clause*> res;

    uint k = this->boxes.size();
    uint* func = new uint[k]();
    uint G_size = G->clauses.size() - 1;
    uint start = 0;

    while (true)
    {
        Clause* clause = new Clause();

//        for (int i = 0; i < k; i++)
//        {
//            cout << func[i];
//        }
//        cout << endl;


        for (uint i = 0; i < k; ++i)
        {
            Box* rho = boxes.at(i);
            Clause* H = G->clauses.at(func[i]);
            vector<Box*> tmp = rho->composeWith(H);

//            for (Box* b : tmp)
//            {
//                cout << *b;
//            }
//            cout << endl;

            clause->boxes.insert(clause->boxes.end(), tmp.begin(), tmp.end());
        }

        res.push_back(clause);

        if (start == k)
        {
            goto _postloop;
        }
        else
        {
            start = increment(func, k, G_size, start);
        }


    }
    _postloop:
    return res;


}


uint Clause::increment (uint* func, uint k, uint g,
                        uint start)
{
    bool reset = false;

    // assert(hint >= 0);
    for (uint i = start; i < k; ++i)
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
        return true;
    if (rejecting == NO)
        return false;

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
        for (auto itr = boxes.begin(); itr != boxes.end() && !found; ++itr)
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












