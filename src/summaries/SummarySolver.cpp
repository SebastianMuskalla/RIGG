/*
 * Copyright 2016-2022 Sebastian Muskalla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SummarySolver.h"

using namespace std;

Formula* SummarySolver::recomputeValue (Letter* l)
{
    bool conjunction = l->alphabet == NUniversal;

    logger.debug("recomputing formula for nonterminal " + l->toString() + ", owned by the " +
                 (conjunction ? "Universal" : "Existential") + " player", 3);

    auto itrPair = G->rules.equal_range(l);

    if (itrPair.first == itrPair.second)
    {
        return nullptr;
    }

    auto itr = itrPair.first;

    logger.debug("computing formula for rule " + l->toString() + " -> " + Alphabet::wordToString(itr->second), 4);

    Formula* res = formulaFor(itr->second);

    logger.debug("formula for first rule is: " + res->toString(), 4);

    ++itr;

    for (; itr != itrPair.second; ++itr)
    {
        logger.debug("computing formula for rule " + l->toString() + " -> " + Alphabet::wordToString(itr->second), 4);

        Formula* temp = formulaFor(itr->second);

        logger.debug("formula for next rule is: " + temp->toString(), 4);

        if (conjunction)
        {
            res = res->formulaAnd(temp);
        }
        else
        {
            res = res->formulaOr(temp);
        }

        logger.debug("formula for their " + string() + (conjunction ? "conjunction" : "disjunction") + " is: " +
                     temp->toString(), 4);

    }

    return res;
}

void SummarySolver::solve ()
{
    unsigned int iterationCount = 0;

    logger.debug("Initialized summary solver");

    while (!worklist.empty())
    {
        iterationCount++;

        if (iterationCount < 10)
        {
            logger.debug("Step number " + to_string(iterationCount), 1);
        }
        else if (iterationCount % 10 == 0)
        {
            logger.debug("... Step number " + to_string(iterationCount), 1);
        }

        Letter* l = *worklist.begin();
        worklist.erase(worklist.begin());

        Formula* oldValue = solution[l];

        logger.debug("picked up " + l->toString() + " from worklist", 1);
        logger.debug("old value: " + oldValue->toString(), 2);

        Formula* newValue = recomputeValue(l);

        logger.debug("new value: " + newValue->toString(), 2);
        logger.debug("conducting implication check");

        // we always have  oldValue implies newValue
        if (newValue->implies(oldValue))
        {
            // value was stable, don't need to do anything
            logger.debug("solution is stable: new value implies old value", 1);
        }
        else
        {
            solution[l] = newValue;

            logger.debug("solution is unstable, inserting dependencies into worklist", 1);

            // value has changed, need to update dependencies
            auto itrPair = dependencies.equal_range(l);
            for (auto itr = itrPair.first; itr != itrPair.second; ++itr)
            {

                logger.debug("Inserting " + itr->second->toString() + " into worklist", 2);

                worklist.insert(itr->second);
            }
        }
    }
    logger.debug("Worklist is empty, solution has been computed");

    logger.info("Finished after " + to_string(iterationCount) + " steps");

    for (pair<Letter*, Formula*> entry : solution)
    {
        logger.debug("Solution for " + entry.first->toString() + ": " + entry.second->toString(), 1);
    }
}

Formula* SummarySolver::formulaFor (Letter* l)
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

SummarySolver::SummarySolver (NFA* A, GameGrammar* G, const Logger& logger) :
        A(A),
        G(G),
        NUniversal(G->NUniversal),
        NExistential(G->NExistential),
        Sigma(G->Sigma),
        logger(logger)
{
    populate();

    identityBox = A->identityBox();

    identityFormula = Formula::wrap(identityBox, this);
}

void SummarySolver::populate ()
{
    populateSolutionAndWorklist();
    populateDependencies();
}

void SummarySolver::populateSolutionAndWorklist ()
{
    for (Letter* l : NUniversal->letters)
    {
        solution[l] = Formula::falseFormula(this);
        worklist.insert(l);
    }
    for (Letter* l : NExistential->letters)
    {
        solution[l] = Formula::falseFormula(this);
        worklist.insert(l);
    }
}

void SummarySolver::populateDependencies ()
{
    for (const auto& rule : G->rules)
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

Formula* SummarySolver::formulaFor (vector<Letter*> word)
{
    if (word.empty())
    {
        logger.debug("computing formula for epsilon: " + identityFormula->toString(), 5);

        return identityFormula;
    }
    else
    {
        logger.debug("computing formula for " + Alphabet::wordToString(word), 5);

        auto itr = word.begin();
        Formula* res = formulaFor(*itr);

        logger.debug("formula for first letter " + (*itr)->toString() + " is: " + res->toString(), 6);

        ++itr;
        for (; itr != word.end(); ++itr)
        {
            res = res->composeWith(formulaFor(*itr));

            logger.debug("formula for letter " + (*itr)->toString() + " is: " + formulaFor(*itr)->toString(), 6);
            logger.debug("composition is " + res->toString(), 5);
        }
        return res;
    }
}
