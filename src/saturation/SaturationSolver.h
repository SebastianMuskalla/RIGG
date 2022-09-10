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

#ifndef RIGG_SATURATIONSOLVER_H
#define RIGG_SATURATIONSOLVER_H

#include "GamePDS.h"
#include "PAFA.h"
#include "../logging/Logger.h"
#include "../logging/NullLogger.h"

using namespace std;

/**
 * implements Cachat's saturation algorithm
 *
 * given a pushdown system with an ownership partitioning of the control states
 * and an alternating finite automaton (AFA) accepting the goal configurations,
 * it saturates the AFA, resulting in an AFA accepting the whole winning region,
 * i.e. the set of configurations from which player0 can enforce reaching a goal configuration after finitely many steps
 *
 * the provided AFA is modified!
 *
 * has exponential worst-case time complexity (in the number of states of the AFA)
 */
class SaturationSolver
{
    GamePDS* P;
    PAFA* AFA;

public:
    SaturationSolver (GamePDS* P, PAFA* AFA, const Logger& logger = NullLogger()) :
            P(P),
            AFA(AFA),
            logger(logger)
    {}

    void saturate ();

    Letter* convertToAFAState (Letter* p)
    {
        return AFA->pdsStateToAFAState[p];
    }

    SaturationSolver (SaturationSolver const&) = delete;

    SaturationSolver& operator= (SaturationSolver const&) = delete;

private:
    const Logger& logger;
};

#endif //RIGG_SATURATIONSOLVER_H
