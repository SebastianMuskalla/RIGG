//
// Created by Sebastian on 15.07.2016.
//

#include "NaiveKleene.h"

NaiveKleene::NaiveKleene (NFA* A, GameGrammar* G) :
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
            cout << "key: " << *pair.first << ", value: " << *pair.second << endl;
        }
    }
}

Formula* NaiveKleene::formulaFor (vector<Letter*> word)
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
        {
            cout << "        epsilon: " << *id_formula << endl;
        }
        return id_formula;
    }

    auto itr = word.begin();
    Formula* res = formulaFor(*itr);

    if (cout_debug)
    {
        cout << "        formula for first letter " << **itr << " is: " << *res << endl;
    }

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

Formula* NaiveKleene::formulaFor (Letter* l)
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

void NaiveKleene::solve ()
{
    bool stable = false;
    while (!stable)
    {
        stable = true;
        for (Letter* l : N)
        {
            new_solution[l] = recomputeValue(l);
        }

        for (auto itr = N.begin(); stable && itr != N.end(); ++itr)
        {
            if (!new_solution[*itr]->implies(solution[*itr]))
            {
                stable = false;
            }
        }

        if (!stable)
        {
            solution = new_solution;
            new_solution.clear();
        }
    }
}

void NaiveKleene::populate ()
{
    for (Letter* l : Nprover->letters)
    {
        solution[l] = Formula::falseFormula();
        N.insert(l);
    }
    for (Letter* l : Nrefuter->letters)
    {
        solution[l] = Formula::falseFormula();
        N.insert(l);
    }
}

Formula* NaiveKleene::recomputeValue (Letter* l)
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
    {
        cout << "    formula for first rule is: " << *res << endl;
    }

    ++itr;

    for (; itr != itrpair.second; ++itr)
    {
        Formula* tmp = formulaFor(itr->second);

        if (cout_debug)
        {
            cout << "    formula for next rule is: " << *tmp << endl;
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
            cout << "    result of and/or is " << *res << endl;
        }
    }

    return res;
}










