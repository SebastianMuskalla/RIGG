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

#include "Help.h"
#include "Examples.h"
#include "Benchmarking.h"

#define EXIT_CODE_OK 0
#define EXIT_CODE_ERROR 1

int main (int argc, char* argv[])
{
    // check if we should show the help
    if (Help::shouldShowHelp(argc, argv))
    {
        return Help::showHelp();
    }

    // check if we should run the handcrafted examples
    if (Examples::shouldRunExamples(argc, argv))
    {
        return Examples::runExamples();
    }

    // otherwise, generate a random instance with the specified parameters and benchmark it
    try
    {
        BenchmarkingParameters* benchmarkingParameters = Benchmarking::parseArguments(argc, argv);
        return Benchmarking::benchmark(benchmarkingParameters);
    }
    catch (exception const& e)
    {
        return EXIT_CODE_ERROR;
    }
}



