#include "PAFA.h"
#include "../common/PointeeComparator.h"

using namespace std;

bool PAFA::acceptsFromControlState (Letter* control_state, vector<Letter*> word)
{
    set<Letter*> S = final_states;

    // backwards search
    for (auto ritr = word.rbegin(); ritr != word.rend(); ++ritr)
    {
        if (S.empty())
        {
            return false;
        }

        set<Letter*> new_S;
        for (AFATransition* t : transitions)
        {
            if (t->label == *ritr)
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
                new_S.insert(t->source);
            }

            _continue_label:;
        }

        S = new_S;

    }

    return (S.find(control_state) != S.end());
}

set<set<Letter*>> PAFA::reachableFromControlState (Letter* control_state, vector<Letter*> word)
{
    // disjunction of sets of states, each inner set representing a conjunction
    set<set<Letter*>> current;
    set<set<Letter*>> next;

    set<Letter*> init;
    init.insert(control_state);
    current.insert(init);

    for (Letter* a : word)
    {
        next.clear();

        for (set<Letter*> conjunction : current)
        {
            // we compute the successors for one conjunction at the time
            set<set<Letter*>> inner_current;
            set<set<Letter*>> inner_next;
            inner_current.insert(set<Letter*>());

            for (Letter* state : conjunction)
            {
                // iterate over all successors
                for (AFATransition* t : transitions)
                {
                    if (t->label == a && t->source == state)
                    {
                        for (set<Letter*> inner_set : inner_current)
                        {
                            set<Letter*> copy(inner_set);
                            copy.insert(t->targets.begin(), t->targets.end());
                            inner_next.insert(copy);
                        }
                    }
                }
                inner_current = inner_next;
                inner_next.clear();
            }

            for (set<Letter*> inner_set : inner_current)
            {
                if (!inner_set.empty())
                {
                    next.insert(inner_set);
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
    res.append(control_states->toString());
    res.append("\nFinal states: ");

    bool first = true;
    for (Letter* f : final_states)
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

bool PAFA::addTransition (Letter* source, Letter* label, set<Letter*> targets)
{
    // adding transitions with empty target set is not very helpful
    if (targets.empty())
    {
        return false;
    }

    AFATransition* t = new AFATransition(source, label, targets);

    PointeeComparator<AFATransition> eq = {t};

    auto itr = find_if(transitions.begin(), transitions.end(), eq);

//    auto itr = find_if(transitions.begin(), transitions.end(), [&compare] (const AFATransition*&arg)
//    { return *arg == *compare; });

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
        control_states(),
        pds_state_to_afa_state(),
        final_states(),
        transitions()
{ }

PAFA::~PAFA ()
{
    delete control_states;
    for (AFATransition* t : transitions)
    {
        delete t;
    }
}
