#include "PAFA.h"

using namespace std;
//
//bool PAFA::acceptsFromPState (Letter* p_state, vector<Letter*> word)
//{
//    auto control_state_itr = pds_state_to_afa_state.find(p_state);
//    if (control_state_itr == pds_state_to_afa_state.end())
//    {
//        throw new string("invalid state!?");
//    }
//    Letter* control_state = control_state_itr->second;
//    return acceptsFromControlState(control_state, word);
//}

bool PAFA::acceptsFromControlState (Letter* control_state, vector<Letter*> word)
{
    set<Letter*> S = final_states;

    cout << "initial S: ";
    for (Letter* l : S)
    {
        cout << *l;
    }
    cout << endl;

    // backwards search
    for (auto ritr = word.rbegin(); ritr != word.rend(); ++ritr)
    {
        if (S.empty())
        {
            return false;
        }

        cout << " current S: ";
        for (Letter* l : S)
        {
            cout << *l;
        }
        cout << endl;

        cout << "Letter: " << **ritr << endl;

        set<Letter*> new_S;
        for (AFATransition* t : transitions)
        {
            if (t->label == *ritr)
            {
                cout << "applicable transition " << *t << endl;

                // check that all target states are contained in the old S
                for (Letter* x : t->targets)
                {
                    cout << "checking containment of " << *x << " in S" << endl;


                    if (S.find(x) == S.end())
                    {
                        cout << "not contained" << endl;
                        goto _continue_label;
                    }
                }

                cout << "was contained, insert!" << endl;

                // ... if yes, insert the source state into the new S
                new_S.insert(t->origin);
            }

            _continue_label:;
        }

        S = new_S;

        cout << "new S: ";
        for (Letter* l : S)
        {
            cout << *l;
        }
        cout << endl;
    }

    return (S.find(control_state) != S.end());
}

//set<set<Letter*>> PAFA::reachableFromPState (Letter* p_state, vector<Letter*> word)
//{
//    auto control_state_itr = pds_state_to_afa_state.find(p_state);
//    if (control_state_itr == pds_state_to_afa_state.end())
//    {
//        throw new string("invalid state!?");
//    }
//    Letter* control_state = control_state_itr->second;
//    return reachableFromControlState(control_state, word);
//}

set<set<Letter*>> PAFA::reachableFromControlState (Letter* control_state, vector<Letter*> word)
{

    bool important = false;

    if (!word.empty() && control_state->name == "{}_r" && word[0]->name == "X")
    {
        important = true;
        cout << "        important!" << endl;
    }

    // disjunction of sets of states, each inner set representing a conjunction
    set<set<Letter*>> current;
    set<set<Letter*>> next;

    set<Letter*> init;
    init.insert(control_state);
    current.insert(init);

    for (Letter* a : word)
    {
        if (important)
        {
            cout << "        Handling letter " << *a << endl;
        }

        next.clear();

        for (set<Letter*> conjunction : current)
        {
            // we compute the successors for one conjunction at the time
            set<set<Letter*>*> tmp;
            set<Letter*>* empty = new set<Letter*>();
            tmp.insert(empty);

            for (Letter* state : conjunction)
            {
                // iterate over all successors
                for (AFATransition* t : transitions)
                {
                    if (important)
                    {
                        cout << "        checking transition " << *t << endl;
                    }

                    if (t->label == a && t->origin == state)
                    {
                        if (important)
                        {
                            cout << "        transition " << *t << endl;
                        }

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
    // adding transitions with empty target set is not very helpful
    if (targets.empty())
        return false;

    AFATransition* t = new AFATransition(source, label, targets);

    pointer_values_equal<AFATransition> eq = {t};

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







