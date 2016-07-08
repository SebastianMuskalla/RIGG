#include "Solver.h"

using namespace std;

Formula* Solver::recomputeValue (Letter* l)
{
    bool and_mode = l->alphabet == Nprover;

    if (cout_debug)
    {
        cout << "    recomputing formula for " << *l << endl;
        cout << "    owned by prover: " << and_mode << endl;
    }

    Formula* f = new Formula();

    auto itrpair = G->rules.equal_range(l);

    if (itrpair.first == itrpair.second)
    {
        return nullptr;
    }

    auto itr = itrpair.first;
    Formula* res = formulaFor(itr->second);

    if (cout_debug)
        cout << "    formula for first rule is: " << *res << endl;

    ++itr;


    for (; itr != itrpair.second; ++itr)
    {
        Formula* tmp = formulaFor(itr->second);

        if (cout_debug)
            cout << "    formula for next rule is: " << *tmp << endl;

        if (and_mode)
        {
            res = res->formulaAnd(tmp);
        }
        else
        {
            res = res->formulaOr(tmp);
        }
    }


    return res;


}

void Solver::solve ()
{
//    for (auto pair : solution)
//    {
//        cout << "key:" << *pair.first << ", value " << *pair.second << endl;
//    }

    while (!todo.empty())
    {

        Letter* l = *todo.begin();
        todo.erase(todo.begin());

        Formula* old_value = solution[l];
        Formula* new_value = recomputeValue(l);

        if (cout_debug)
        {
            cout << "picked up " << *l << " from todo" << endl;
            cout << "old value: " << *old_value << endl;
            cout << "new value:" << *new_value << endl;
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
            solution[l] = new_value;

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
            cout << endl;
        }

    }
}


Formula* Solver::formulaFor (Letter* l)
{
    if (G->isNonterminal(l))
    {
        return solution[l];
    }
    else
    {
        return Formula::wrap(A->boxFor(l));
    }
}

Solver::Solver (NFA* A, GameGrammar* G) :
        A(A),
        G(G),
        Q(A->states),
        Nprover(G->Nprover),
        Nrefuter(G->Nrefuter),
        Sigma(G->Sigma)
{

    populate();

    id_box = new Box(A, Q, "ID");
    for (Letter* q : Q->letters)
    {
        id_box->content.emplace(q, q);
    }

    id_formula = Formula::wrap(id_box);

    if (cout_debug)
    {
        cout << "initial values" << endl;
        for (auto pair : solution)
        {
            cout << "key:" << *pair.first << ", value " << *pair.second << endl;
        }
    }

}

void Solver::populate ()
{
    populateSolutionAndWorklist();
    populateDependencies();
}

void Solver::populateSolutionAndWorklist ()
{
    for (Letter* l : Nprover->letters)
    {
        solution[l] = Formula::falseFormula();
        todo.insert(l);
    }
    for (Letter* l : Nrefuter->letters)
    {
        solution[l] = Formula::falseFormula();
        todo.insert(l);
    }

}

void Solver::populateDependencies ()
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

Formula* Solver::formulaFor (vector<Letter*> word)
{
    if (cout_debug)
    {
        cout << "        computing forumula for ";
        for (Letter* l : word)
        {
            cout << *l;
        }
        cout << endl;
    }

    if (word.empty())
    {
        if (cout_debug)
            cout << "        epsilon: " << *id_formula << endl;
        return id_formula;
    }

    auto itr = word.begin();
    Formula* res = formulaFor(*itr);

    if (cout_debug)
        cout << "        formula for first letter " << **itr << " is: " << *res << endl;

    ++itr;
    for (; itr != word.end(); ++itr)
    {

        res = res->composeWith(formulaFor(*itr));

        if (cout_debug)
        {
            cout << "        formula for letter " << **itr << " is: " << *formulaFor(*itr) << endl;
            cout << "        composition is " << *res << endl;
        }
    }
    return res;
}
