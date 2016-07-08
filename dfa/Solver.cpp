#include "Solver.h"

using namespace std;

Formula* Solver::recomputeValue (Letter* l)
{
    bool and_mode = l->alphabet == Nprover;

    Formula* f = new Formula();

    auto itrpair = G->rules.equal_range(l);

    if (itrpair.first == itrpair.second)
    {
        return nullptr;
    }

    auto itr = itrpair.first;
    Formula* res = formulaFor(itr->second);
    ++itr;

    for (; itr != itrpair.second; ++itr)
    {
        Formula* tmp = formulaFor(itr->second);

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
    while (!todo.empty())
    {
        Letter* l = *todo.begin();
        todo.erase(todo.begin());

        Formula* old_value = solution[l];
        Formula* new_value = recomputeValue(l);


        // we always have  old_value implies new_value

        if (new_value->implies(old_value))
        {
            // value was stable, dont need to do anything
        }
        else
        {
            solution[l] = new_value;

            // value has changed, need to update dependencies
            auto itrpair = dependencies.equal_range(l);
            for (auto itr = itrpair.first; itr != itrpair.second; ++itr)
            {
                todo.insert(itr->second);
            }
        }

    }
}

Formula* Solver::formulaFor (vector<Letter*> word)
{
    if (word.empty())
    {
        return id_formula;
    }

    auto itr = word.begin();
    Formula* res = formulaFor(*itr);
    ++itr;
    for (; itr != word.end(); ++itr)
    {
        res = res->composeWith(formulaFor(*itr));
    }
    return res;
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
}
