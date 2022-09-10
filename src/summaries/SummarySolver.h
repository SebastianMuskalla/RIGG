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

#ifndef RIGG_SUMMARYSOLVER_H
#define RIGG_SUMMARYSOLVER_H

#include "../common/NFA.h"
#include "../common/GameGrammar.h"
#include "../common/Alphabet.h"
#include "FormulaStorage.h"
#include "../logging/Logger.h"
#include "../logging/NullLogger.h"

using namespace std;

/**
 * SummarySolver for grammar games based on Kleene-iteration over the domain of box-formulas
 *
 * Uses a worklist to only updates single values that could have been become unstable due to other updates
 */
class SummarySolver : public FormulaStorage
{
public:
    NFA* A;
    GameGrammar* G;
    Alphabet* NUniversal;
    Alphabet* NExistential;
    Alphabet* Sigma;

    /**
     * Generate solver for given game instance
     */
    SummarySolver (NFA* A, GameGrammar* G, const Logger& logger = NullLogger());

    /**
     * Computes the formula for a given sentential form (by composing the formulas for the single letters) according to the current solution
     */
    Formula* formulaFor (vector<Letter*> word);

    /**
     * Computes the formula for a given letter (terminal or non-terminal of any of the two players)  according to the current solution
     */
    Formula* formulaFor (Letter* l);

    /**
     * Start the iteration to the solution
     */
    void solve ();

    void registerFormula (Formula* f) override
    {
        allFormulas.insert(f);
    }

    virtual ~SummarySolver ()
    {
        for (Formula* f : allFormulas)
        {
            delete f;
        }
        A->resetBoxes();
    }

    SummarySolver (SummarySolver const&) = delete;

    SummarySolver& operator= (SummarySolver const&) = delete;

private:
    Box* identityBox;
    Formula* identityFormula;

    multimap<Letter*, Letter*> dependencies;
    map<Letter*, Formula*> solution;

    set<Letter*> worklist;

    set<Formula*> allFormulas;

    const Logger& logger;

    /**
     * Initially populate solution, worklist and dependencies
     */
    void populate ();

    /**
     * Initially populate solution and worklist
     */
    void populateSolutionAndWorklist ();

    /**
     * Initially populate dependencies
     */
    void populateDependencies ();

    /**
     * Recompute the solution for a given non-terminal
     */
    Formula* recomputeValue (Letter* l);

};

#endif //RIGG_SUMMARYSOLVER_H
