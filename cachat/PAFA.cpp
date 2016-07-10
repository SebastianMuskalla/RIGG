#include "PAFA.h"

using namespace std;

bool PAFA::acceptsFromPState (Letter* p_state, vector<Letter*> word)
{
    auto control_state_itr = pds_state_to_afa_state.find(p_state);
    if (control_state_itr == pds_state_to_afa_state.end())
    {
        throw new string("invalid state!?");
    }
    Letter* control_state = control_state_itr->second;
    return acceptsFromControlState(control_state, word);
}

bool PAFA::acceptsFromControlState (Letter* control_state, vector<Letter*> word)
{

    auto reach = reachableFromControlState(control_state, word);

    // check that there is a tree such that all leaves are final
    for (set<Letter*> conjunction : reach)
    {
        bool all_final = true;
        for (auto itr = conjunction.begin(); itr != conjunction.end() && all_final; ++itr)
        {
            Letter* state = *itr;
            if (final_states.find(state) == final_states.end())
            {
                all_final = false;
            }
        }
        if (all_final)
        {
            return true;
        }
    }
    return false;


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
            set<set<Letter*>*> tmp;
            set<Letter*>* empty = new set<Letter*>;
            tmp.insert(empty);

            for (Letter* state : conjunction)
            {
                // iterate over all successors
                for (AFATransition* t : transitions)
                {
                    if (t->label == a && t->origin == state)
                    {
                        for (set<Letter*>* set_pointer : tmp)
                        {
                            set_pointer->insert(t->targets.begin(), t->targets.end());
                        }
                    }

                }
            }

            for (set<Letter*>* set_pointer : tmp)
            {
                next.insert(*set_pointer);
                delete set_pointer;
            }

        }
    }
    return next;
}


string PAFA::toString () const
{
    string res;
    res.append("Input alphabet: ");
    res.append(Gamma->toString());
    res.append("\nStates: ");
    res.append(control_states->toString());
//    res.append("\nInitial state: ");
//    res.append(initial_state->toString());
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
    AFATransition* t = new AFATransition(source, label, targets);
    auto itr = transitions.find_if(transitions.begin(), transitions.end(), [&t] (const AFATransition*&arg)
    { return *arg == *t; });
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







