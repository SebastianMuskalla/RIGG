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

#ifndef RIGG_EXAMPLES_H
#define RIGG_EXAMPLES_H

#include "common/Alphabet.h"
#include "common/NFA.h"
#include "summaries/SummarySolver.h"
#include "randomgen/TVLinearGrammarGen.h"
#include "randomgen/TVAutomataGen.h"
#include "saturation/Cachatifier.h"
#include "saturation/SaturationSolver.h"
#include "saturation/Minimizer.h"
#include "saturation/Determinizer.h"
#include "logging/Logger.h"
#include "logging/FilteredCoutLogger.h"
#include "logging/CoutLogger.h"
#include "logging/IndentedLogger.h"
#include "Help.h"

#define EXIT_CODE_OK 0
#define EXIT_CODE_ERROR 1

using namespace std;

class Examples
{
    /**
     * command-line option to indicate that the examples should be executed
     */
    static constexpr const string_view OPTION_EXAMPLE[] = {"-e", "--e", "--example", "--examples"};

    static void runExample1 (const Logger& logger);

    static void runExample2 (const Logger& logger);


    /**
     * case-insensitive string comparison
     */
    static bool equalsUpToCase (string first, string second);

    /**
     * check if string is in a given array of string views
     */
    template<size_t S>
    static bool isOneOf (const string& argument, const string_view (& strings)[S]);


public:

    /**
     * check whether the examples should be executed
     */
    static bool shouldRunExamples (int argumentCount, char* arguments[]);

    /**
     * run the examples
     */
    static int runExamples ();
};

#endif //RIGG_EXAMPLES_H
