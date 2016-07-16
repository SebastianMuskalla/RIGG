#include <c++/4.8.3/chrono>
#include "WorklistKleene.h"

using namespace std;

Formula* WorklistKleene::recomputeValue (Letter* l)
{
    bool and_mode = l->alphabet == Nprover;

    if (cout_debug)
    {
        cout << "    recomputing formula for " << l->toString() << endl;
        cout << "    owned by prover: " << and_mode << endl;
    }

    auto itrpair = G->rules.equal_range(l);

    if (itrpair.first == itrpair.second)
    {
        return nullptr;
    }

    auto itr = itrpair.first;
    Formula* res = formulaFor(itr->second);

    if (cout_debug)
    {
        cout << "    formula for first rule is: " << res->toString() << endl;
    }

    ++itr;

    for (; itr != itrpair.second; ++itr)
    {
        Formula* tmp = formulaFor(itr->second);

        if (cout_debug)
        {
            cout << "    formula for next rule is: " << tmp->toString() << endl;
        }

        if (and_mode)
        {
            res = res->formulaAnd(tmp);
        }
        else
        {
            res = res->formulaOr(tmp);
        }

        if (cout_debug)
        {
            cout << "    result of and/or is " << res->toString() << endl;
        }
    }

    return res;
}

void WorklistKleene::solve ()
{
    auto start = chrono::steady_clock::now();

    if (timeout &&
        chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() > timeout)
    {
        timeout_flag = true;
        return;
    }

    while (!todo.empty())
    {
        Letter* l = *todo.begin();
        todo.erase(todo.begin());

        Formula* old_value = solution[l];

        if (cout_debug)
        {
            cout << "picked up " << l->toString() << " from worklist" << endl;
            cout << "old value: " << old_value->toString() << endl;
        }

        Formula* new_value = recomputeValue(l);

        if (cout_debug)
        {
            cout << "new value: " << new_value->toString() << endl;
            cout << "implication check..." << endl;
        }

        // we always have  old_value implies new_value

        if (new_value->implies(old_value))
        {
            // value was stable, dont need to do anything
            if (cout_debug)
            {
                cout << "stable: new value implies old value" << endl;
            }
        }
        else
        {
            if (use_subsumption)
            {
                solution[l] = new_value->simplify();
            }
            else
            {
                solution[l] = new_value;
            }

            if (cout_debug)
            {
                cout << "unstable: insert dependencies" << endl;
            }

            // value has changed, need to update dependencies
            auto itrpair = dependencies.equal_range(l);
            for (auto itr = itrpair.first; itr != itrpair.second; ++itr)
            {
                todo.insert(itr->second);
            }
        }
    }
}

Formula* WorklistKleene::formulaFor (Letter* l)
{
    if (G->isNonterminal(l))
    {
        return solution[l];
    }
    else
    {
        return Formula::wrap(A->boxFor(l), this);
    }
}

WorklistKleene::WorklistKleene (NFA* A, GameGrammar* G, bool use_subsumption, uint timeout) :
        A(A),
        G(G),
        Q(A->Q),
        Nprover(G->Nprover),
        Nrefuter(G->Nrefuter),
        Sigma(G->Sigma),
        use_subsumption(use_subsumption),
        timeout(timeout)
{
    populate();

    identity_box = new Box(A, Q, "ID");
    for (Letter* q : Q->letters)
    {
        identity_box->content.emplace(q, q);
    }

    identity_formula = Formula::wrap(identity_box, this);

    if (cout_debug)
    {
        cout << "initial values" << endl;
        for (auto pair : solution)
        {
            cout << "key: " << pair.first->toString() << ", value: " << pair.second->toString() << endl;
        }
    }

}

void WorklistKleene::populate ()
{
    populateSolutionAndWorklist();
    populateDependencies();
}

void WorklistKleene::populateSolutionAndWorklist ()
{
    for (Letter* l : Nprover->letters)
    {
        solution[l] = Formula::falseFormula(this);
        todo.insert(l);
    }
    for (Letter* l : Nrefuter->letters)
    {
        solution[l] = Formula::falseFormula(this);
        todo.insert(l);
    }
}

void WorklistKleene::populateDependencies ()
{
    for (auto rule : G->rules)
    {
        Letter* lhs = rule.first;
        vector<Letter*> rhs = rule.second;
        for (Letter* l : rhs)
        {
            if (G->isNonterminal(l))
            {
                dependencies.emplace(l, lhs);
            }
        }
    }
}

Formula* WorklistKleene::formulaFor (vector<Letter*> word)
{
    if (cout_debug)
    {
        cout << "        computing forumula for ";
        for (Letter* l : word)
        {
            cout << l->toString();
        }
        cout << endl;
    }

    if (word.empty())
    {
        if (cout_debug)
        {
            cout << "        epsilon: " << identity_formula->toString() << endl;
        }
        return identity_formula;
    }

    auto itr = word.begin();
    Formula* res = formulaFor(*itr);

    if (cout_debug)
    {
        cout << "        formula for first letter " << (*itr)->toString() << " is: " << res->toString() << endl;
    }

    ++itr;
    for (; itr != word.end(); ++itr)
    {
        res = res->composeWith(formulaFor(*itr));

        if (cout_debug)
        {
            cout << "        formula for letter " << (*itr)->toString() << " is: " << formulaFor(*itr)->toString() <<
            endl;
            cout << "        composition is " << res->toString() << endl;
        }
    }
    return res;
}
