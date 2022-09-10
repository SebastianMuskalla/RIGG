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

#ifndef RIGG_BENCHMARKING_H
#define RIGG_BENCHMARKING_H

#include <climits>
#include <chrono>
#include <random>
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
#include "logging/QuietLogger.h"

#define EXIT_CODE_OK 0
#define EXIT_CODE_ERROR 1

/**
 * barrier for optimization so that time measuring does not get messed up
 */
#define OPTIMIZATION_BARRIER() asm volatile("" ::: "memory")

enum class Algorithm
{
    /**
     * our algorithm
     */
    SUMMARIES,

    /**
     * saturation algorithm based on the work by Cachat
     */
    SATURATION,

    /**
     * use both algorithms and compare times and solutions
     */
    BOTH
};

/**
 * a struct for storing the parameters for benchmarking
 */
struct BenchmarkingParameters
{
    LogLevel logLevel;
    bool logLevelIsSet = false;
    static constexpr const LogLevel LOGLEVEL_DEFAULT = NORMAL;

    Algorithm algorithm;
    bool algorithmIsSet = false;
    static constexpr const Algorithm ALGORITHM_DEFAULT = Algorithm::BOTH;

    unsigned int numberOfLetters;
    bool numberOfLettersIsSet = false;
    static constexpr const unsigned int NUMBEROFLETTERS_DEFAULT = 5;

    unsigned int numberOfStates;
    bool numberOfStatesIsSet = false;
    static constexpr const unsigned int NUMBEROFSTATES_DEFAULT = 5;

    double automatonDensity;
    bool automatonDensityIsSet = false;
    static constexpr const double AUTOMATONDENSITY_DEFAULT = 0.8;

    double automatonRateOfFinals;
    bool automatonRateOfFinalsIsSet = false;
    static constexpr const double AUTOMATONRATEOFFINALS_DEFAULT = 0.2;

    unsigned int numberOfNonterminalsUniversal;
    bool numberOfNonterminalsUniversalIsSet = false;
    static constexpr const unsigned int NUMBEROFNONTERMINALSUNIVERSAL_DEFAULT = 5;

    unsigned int numberOfNonterminalsExistential;
    bool numberOfNonterminalsExistentialIsSet = false;
    static constexpr const unsigned int NUMBEROFNONTERMINALSEXISTENTIAL_DEFAULT = 5;

    double grammarDensity;
    bool grammarDensityIsSet = false;
    static constexpr const double GRAMMARDENSITY_DEFAULT = 0.75;

    double grammarRateOfFinals;
    bool grammarRateOfFinalsIsSet = false;
    static constexpr const double GRAMMARRATEOFFINALS_DEFAULT = 0.85;

    double grammarRateOfLeft;
    bool grammarRateOfLeftIsSet = false;
    static constexpr const double GRAMMARRATEOFLEFT_DEFAULT = 0.85;

    double grammarRateOfRight;
    bool grammarRateOfRightIsSet = false;
    static constexpr const double GRAMMARRATEOFRIGHT_DEFAULT = 0.85;

    unsigned int seed;
    bool seedIsSet = false;
};

class Benchmarking
{
public:

    /**
     * evaluate the command-line arguments to create a BenchmarkingParameters object storing the values
     *
     * unspecified options will be assigned their default values as specified by the static members of BenchmarkingParameters
     */
    static BenchmarkingParameters* parseArguments (int argumentCount, char* arguments[]);

    /**
     * randomly generate a game instance with the specified parameters and benchmark it
     */
    static int benchmark (BenchmarkingParameters* parameters);

private:

    static BenchmarkingParameters* processArguments (int argumentCount, char* arguments[]);

    static void assignDefaultValues (BenchmarkingParameters* parameters);


    /**
     * case-insensitive string comparison
     */
    static bool equalsUpToCase (string first, string second);


    /**
     * check if string is in a given array of string views
     */
    template<size_t S>
    static bool isOneOf (const string& argument, const string_view (& strings)[S]);

    /**
     * print the values of the parameters
     * (and whether they have been specified by the user or whether the default values have been used)
     */
    static void logParameters (const BenchmarkingParameters& parameters, const Logger& logger);

    static constexpr const string_view OPTION_LOGLEVEL[] = {"--log", "--loglevel"};
    static constexpr const string_view OPTION_LOGLEVEL_ERROR[] = {"-q", "--quiet"};
    static constexpr const string_view OPTION_LOGLEVEL_INFO[] = {"-v", "--verbose"};
    static constexpr const string_view OPTION_LOGLEVEL_DEBUG[] = {"-vv", "--vv", "--veryverbose"};

    static constexpr const string_view OPTION_LOGLEVEL_OPTION_ERROR[] = {"ERROR"};
    static constexpr const string_view OPTION_LOGLEVEL_OPTION_NORMAL[] = {"NORMAL"};
    static constexpr const string_view OPTION_LOGLEVEL_OPTION_INFO[] = {"INFO"};
    static constexpr const string_view OPTION_LOGLEVEL_OPTION_DEBUG[] = {"DEBUG"};

    static constexpr const string_view OPTION_ALGORITHM[] = {"-a", "--algorithm"};
    static constexpr const string_view OPTION_ALGORITHM_SUMMARIES[] = {"Summaries", "Summary", "Kleene"};
    static constexpr const string_view OPTION_ALGORITHM_SATURATION[] = {"Saturation", "Cachat"};
    static constexpr const string_view OPTION_ALGORITHM_BOTH[] = {"Both"};

    static constexpr const string_view OPTION_LETTERS[] = {"-l", "--letters"};

    static constexpr const string_view OPTION_STATES[] = {"-s", "--states"};

    static constexpr const string_view OPTION_NONTERMINALS[] = {"-n", "--nonterminals"};
    static constexpr const string_view OPTION_NONTERMINALS_UNIVERSAL[] = {"--nonterminalsuniversal"};
    static constexpr const string_view OPTION_NONTERMINALS_EXISTENTIAL[] = {"--nonterminalsexistential"};

    static constexpr const string_view OPTION_AUTOMATONDENSITY[] = {"--automatondensity"};
    static constexpr const string_view OPTION_AUTOMATONFINALS[] = {"--automatonfinals"};

    static constexpr const string_view OPTION_GRAMMARDENSITY[] = {"--grammardensity"};
    static constexpr const string_view OPTION_GRAMMARFINALS[] = {"--grammarfinals"};
    static constexpr const string_view OPTION_GRAMMARLEFT[] = {"--grammarleft"};
    static constexpr const string_view OPTION_GRAMMARRIGHT[] = {"--grammarright"};

    static constexpr const string_view OPTION_SEED[] = {"-r", "--seed"};

};


#endif //RIGG_BENCHMARKING_H
