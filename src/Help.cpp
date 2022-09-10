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

#include <algorithm>
#include "Help.h"
#include "logging/Logger.h"
#include "logging/CoutLogger.h"


bool Help::equalsUpToCase (string first, string second)
{
    transform(first.begin(), first.end(), first.begin(), ::tolower);
    transform(second.begin(), second.end(), second.begin(), ::tolower);
    return (first == second);
}


template<size_t S>
bool Help::isOneOf (const string& argument, const string_view (& strings)[S])
{
    for (auto param : strings)
    {
        if (equalsUpToCase(string(param), argument))
        {
            return true;
        }
    }
    return false;
}


bool Help::shouldShowHelp (int argumentCount, char** arguments)
{
    for (int i = 1; i < argumentCount; i++)
    {
        string argument(arguments[i]);

        if (isOneOf(argument, OPTION_HELP))
        {
            return true;
        }
    }
    return false;
}

int Help::showHelp ()
{
    Logger* logger = new CoutLogger();
    logger->msg("RIGG - Solver for Regular Inclusion Grammar Games");
    logger->msg("Copyright 2016-2022 Sebastian Muskalla, licensed under the GPLv3");
    logger->msg("Documentation and code: https://github.com/SebastianMuskalla/RIGG");
    logger->newline();
    logger->msg("-h (or --help): show this help");
    logger->msg("-e (or --examples): execute some examples");
    logger->msg("otherwise: Benchmarking mode with the following options");
    logger->newline();
    logger->msg("-q (or --quiet or --log ERROR)     show reduced output");
    logger->msg("-v (or --verbose or --log INFO)    show verbose output");
    logger->msg("-vv (or --log DEBUG)               show debug output");
    logger->msg("-a (or --algorithm) <BOTH/SUMMARIES/SATURATION>    which algorithm to run");
    logger->msg("-l (or --letters) <INT>            number of letters (terminals");
    logger->msg("-s (or --states) <INT>             number of states of the automaton");
    logger->msg("-n (or --nonterminals) <INT>       nonterminals per player");
    logger->msg("-r (or --seed) <INT>               seed for the random generation");

    logger->msg("--automatondensity <DOUBLE>        density of the automaton");
    logger->msg("--automatonfinals <DOUBLE>         rate of final states in the automaton");
    logger->msg("--nonterminalsuniversal <INT>      nonterminals of the Universal Player");
    logger->msg("--nonterminalsexistential <INT>    nonterminals of the Existential Player");
    logger->msg("--grammardensity <DOUBLE>          density of the grammar");
    logger->msg("--grammarfinals <DOUBLE>           rate of final transitions in the grammar");
    logger->msg("--grammarleft <DOUBLE>             rate of left transitions in the grammar");
    logger->msg("--grammarright <DOUBLE>            rate of right transitions in the grammar");

    return EXIT_CODE_OK;
}
