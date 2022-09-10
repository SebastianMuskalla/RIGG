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

#include "Benchmarking.h"


bool Benchmarking::equalsUpToCase (string first, string second)
{
    transform(first.begin(), first.end(), first.begin(), ::tolower);
    transform(second.begin(), second.end(), second.begin(), ::tolower);
    return (first == second);
}


template<size_t S>
bool Benchmarking::isOneOf (const string& argument, const string_view (& strings)[S])
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


BenchmarkingParameters* Benchmarking::parseArguments (int argumentCount, char** arguments)
{
    BenchmarkingParameters* benchmarkingParameters;

    try
    {
        benchmarkingParameters = processArguments(argumentCount, arguments);
    }
    catch (runtime_error const& e)
    {
        cout << "ERROR: " << e.what() << endl;
        cout << "Use -h (or --help) to show a list of the available arguments" << endl;
        throw exception();
    }

    assignDefaultValues(benchmarkingParameters);

    return benchmarkingParameters;
}


int Benchmarking::benchmark (BenchmarkingParameters* parameters)
{
    Logger* logger;

    // logger for logging the internals of the algorithm with additional indentation
    // we will only need this if the log level is set to INFO or higher
    Logger* sublogger;

    switch (parameters->logLevel)
    {
        case ERROR:
            logger = new QuietLogger();
            sublogger = new NullLogger();
            break;
        case NORMAL:
            logger = new FilteredCoutLogger(NORMAL);
            sublogger = new NullLogger();
            break;
        case INFO:
            logger = new FilteredCoutLogger(INFO);
            sublogger = new IndentedLogger(*logger, 1);
            break;
        case DEBUG:
            logger = new FilteredCoutLogger(DEBUG);
            sublogger = new IndentedLogger(*logger, 1);
            break;
    }

    logParameters(*parameters, *logger);

    logger->debug("Initializing Mersenne Twister 19937 generator with the seed");
    mt19937 sourceOfRandomness(parameters->seed);

    logger->msg("Generating instances");

    NFA* A = TVAutomataGen(parameters->numberOfLetters, parameters->numberOfStates, parameters->automatonDensity,
                           parameters->automatonRateOfFinals).generate(sourceOfRandomness);

    Alphabet* Sigma = A->Sigma;

    if (logger->accepts(INFO))
    {
        logger->info("Generated automaton:", 0);
        logger->info(A->toString(), 1);
    }

    GameGrammar* G = TVLinearGrammarGen(Sigma, parameters->numberOfNonterminalsUniversal,
                                        parameters->numberOfNonterminalsUniversal, parameters->grammarDensity,
                                        parameters->grammarRateOfFinals, parameters->grammarRateOfLeft,
                                        parameters->grammarRateOfRight).generate(sourceOfRandomness);

    if (logger->accepts(INFO))
    {
        logger->info("Generated grammar:", 0);
        logger->info(G->toString(), 1);
    }

    SummarySolver* summarySolver = nullptr;
    SaturationSolver* saturationSolver = nullptr;

    if (parameters->algorithm != Algorithm::SATURATION)
    {
        logger->msg("Solving the instance using Summaries");

        OPTIMIZATION_BARRIER();
        auto start = chrono::steady_clock::now();
        OPTIMIZATION_BARRIER();

        summarySolver = new SummarySolver(A, G, *sublogger);
        summarySolver->solve();

        OPTIMIZATION_BARRIER();
        auto end = chrono::steady_clock::now();
        OPTIMIZATION_BARRIER();

        auto time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        logger->important("Time taken for the Summary algorithm: " + to_string(time) + "ms");
    }

    if (parameters->algorithm != Algorithm::SUMMARIES)
    {
        logger->msg("Solving the instance using Saturation");

        OPTIMIZATION_BARRIER();
        auto start = chrono::steady_clock::now();
        OPTIMIZATION_BARRIER();

        logger->info("Determinizing the automaton", 1);
        NFA* D = Determinizer::determinize(A);

        OPTIMIZATION_BARRIER();
        auto afterDeterminization = chrono::steady_clock::now();
        OPTIMIZATION_BARRIER();

        if (logger->accepts(DEBUG))
        {
            logger->debug("Deterministic automaton:", 2);
            logger->debug(D->toString(), 2);
        }

        logger->info("Minimizing the automaton", 2);

        NFA* M = Minimizer::minimize(D);

        OPTIMIZATION_BARRIER();
        auto afterMinimization = chrono::steady_clock::now();
        OPTIMIZATION_BARRIER();

        if (logger->accepts(DEBUG))
        {
            logger->debug("Minimal deterministic automaton:", 2);
            logger->debug(M->toString(), 2);
        }

        logger->info("Converting game into a pushdown game", 2);

        auto* cachatifier = new Cachatifier(M, G);
        tuple<GamePDS*, PAFA*, Letter*, Letter*> tuple = cachatifier->cachatify();
        GamePDS* P = get<0>(tuple);
        PAFA* AFA = get<1>(tuple);
        Letter* initialStateUniversal = get<2>(tuple);
        Letter* initialStateExistential = get<3>(tuple);

        OPTIMIZATION_BARRIER();
        auto afterCachatification = chrono::steady_clock::now();
        OPTIMIZATION_BARRIER();

        if (logger->accepts(DEBUG))
        {
            logger->debug("Pushdown system:", 2);
            logger->debug(P->toString(), 2);
            logger->debug("AFA for the target set:", 2);
            logger->debug(AFA->toString(), 2);
        }

        logger->info("Running the saturation solver");

        saturationSolver = new SaturationSolver(P, AFA, *sublogger);
        saturationSolver->saturate();

        OPTIMIZATION_BARRIER();
        auto end = chrono::steady_clock::now();
        OPTIMIZATION_BARRIER();

        auto timeTotal = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        logger->important("Time taken for the Saturation algorithm: " + to_string(timeTotal) + "ms");

        auto timeDeterminization = chrono::duration_cast<chrono::milliseconds>(afterDeterminization - start).count();
        auto timeMinimization = chrono::duration_cast<chrono::milliseconds>(
                afterMinimization - afterDeterminization).count();
        auto timeCachatification = chrono::duration_cast<chrono::milliseconds>(
                afterCachatification - afterMinimization).count();
        auto timeSaturation = chrono::duration_cast<chrono::milliseconds>(end - afterCachatification).count();

        logger->msg("Time taken for the determinization step: " + to_string(timeDeterminization) + "ms", 1);
        logger->msg("Time taken for the minimization step: " + to_string(timeMinimization) + "ms", 1);
        logger->msg("Time taken for the conversion step: " + to_string(timeCachatification) + "ms", 1);
        logger->msg("Time taken for the saturation step: " + to_string(timeSaturation) + "ms", 1);


        if (parameters->algorithm == Algorithm::BOTH)
        {
            logger->msg("Comparing the output of the algorithms to verify correctness");

            for (Letter* Y : G->NUniversal->letters)
            {
                Formula* formula = summarySolver->formulaFor(Y);
                bool resultSummaries = formula->isRejecting();

                vector<Letter*> stackWord = cachatifier->wordToStackWord({Y});
                Letter* state = saturationSolver->convertToAFAState(initialStateUniversal);

                bool resultSaturation = AFA->acceptsFromControlState(state, stackWord);

                if (resultSummaries == resultSaturation)
                {
                    logger->info("For nonterminal " + Y->toString() + ", the " +
                                 (resultSummaries ? "Existential" : "Universal") + " Player wins", 1);

                    if (logger->accepts(DEBUG))
                    {
                        logger->debug("Formula: " + formula->toString(), 2);
                    }
                }
                else
                {
                    logger->important("ERROR: Discrepancy for nonterminal " + Y->toString());
                    logger->important(
                            "Summary algorithm: " + string() + (resultSummaries ? "Existential" : "Universal") +
                            " Player wins", 1);
                    logger->important(
                            "Saturation algorithm: " + string() + (resultSaturation ? "Existential" : "Universal") +
                            " Player wins", 1);
                    logger->important("Enable debug output (--vv) and run with the same seed for more information");
                    return EXIT_CODE_ERROR;
                }
            }

            for (Letter* X : G->NExistential->letters)
            {
                Formula* formula = summarySolver->formulaFor(X);
                bool resultSummaries = formula->isRejecting();

                vector<Letter*> stackWord = cachatifier->wordToStackWord({X});
                Letter* state = saturationSolver->convertToAFAState(initialStateExistential);
                bool resultSaturation = AFA->acceptsFromControlState(state, stackWord);

                if (resultSummaries == resultSaturation)
                {
                    logger->info("For nonterminal " + X->toString() + ", the " +
                                 (resultSummaries ? "Existential" : "Universal") + " Player wins", 1);
                    logger->debug("Formula: " + formula->toString(), 2);
                }
                else
                {
                    logger->important("ERROR: Discrepancy for nonterminal " + X->toString());
                    logger->important(
                            "Summary algorithm: " + string() + (resultSummaries ? "Existential" : "Universal") +
                            " Player wins", 1);
                    logger->important(
                            "Saturation algorithm: " + string() + (resultSaturation ? "Existential" : "Universal") +
                            " Player wins", 1);
                    logger->important("Enable debug output (--vv) and run with the same seed for more information");
                    return EXIT_CODE_ERROR;
                }
            }

            logger->msg("OK");
        }
    }

    if (parameters->logLevel > LogLevel::NORMAL)
    {
        logger->important("WARNING: if the log level is set to INFO (-v) or DEBUG (-vv),");
        logger->important("         time measurement may be influenced by console output. ");
        logger->important("Use -q for more accurate time measurement.");
    }

    return EXIT_CODE_OK;
}

BenchmarkingParameters* Benchmarking::processArguments (int argumentCount, char** arguments)
{
    auto benchmarkingParameters = new BenchmarkingParameters();

    // parse arguments
    for (int i = 1; i < argumentCount; i++)
    {
        string argument(arguments[i]);

        if (isOneOf(argument, OPTION_LOGLEVEL))
        {
            if (benchmarkingParameters->logLevelIsSet)
            {
                throw runtime_error("option " + argument + " - log level is already set");
            }

            if (i + 1 >= argumentCount)
            {
                throw runtime_error(R"(expected "ERROR", "INFO" or "DEBUG")");
            }

            i++;
            string nextArgument(arguments[i]);

            if (isOneOf(argument, OPTION_LOGLEVEL_OPTION_ERROR))
            {
                benchmarkingParameters->logLevel = ERROR;
                benchmarkingParameters->logLevelIsSet = true;
            }
            else if (isOneOf(argument, OPTION_LOGLEVEL_OPTION_NORMAL))
            {
                benchmarkingParameters->logLevel = NORMAL;
                benchmarkingParameters->logLevelIsSet = true;
            }
            else if (isOneOf(argument, OPTION_LOGLEVEL_OPTION_INFO))
            {
                benchmarkingParameters->logLevel = INFO;
                benchmarkingParameters->logLevelIsSet = true;
            }
            else if (isOneOf(argument, OPTION_LOGLEVEL_OPTION_DEBUG))
            {
                benchmarkingParameters->logLevel = DEBUG;
                benchmarkingParameters->logLevelIsSet = true;
            }
            else
            {
                throw runtime_error("unexpected parameter " + nextArgument + R"(, expected "ERROR", "INFO" or "DEBUG")");
            }
        }
        else if (isOneOf(argument, OPTION_LOGLEVEL_ERROR))
        {
            if (benchmarkingParameters->logLevelIsSet)
            {
                throw runtime_error("option " + argument + " - log level is already set");
            }

            benchmarkingParameters->logLevel = ERROR;
            benchmarkingParameters->logLevelIsSet = true;
        }
        else if (isOneOf(argument, OPTION_LOGLEVEL_INFO))
        {
            if (benchmarkingParameters->logLevelIsSet)
            {
                throw runtime_error("option " + argument + " - log level is already set");
            }

            benchmarkingParameters->logLevel = INFO;
            benchmarkingParameters->logLevelIsSet = true;
        }
        else if (isOneOf(argument, OPTION_LOGLEVEL_DEBUG))
        {
            if (benchmarkingParameters->logLevelIsSet)
            {
                throw runtime_error("option " + argument + " - log level is already set");
            }

            benchmarkingParameters->logLevel = DEBUG;
            benchmarkingParameters->logLevelIsSet = true;
        }
        else if (isOneOf(argument, OPTION_ALGORITHM))
        {
            if (benchmarkingParameters->algorithmIsSet)
            {
                throw runtime_error("option " + argument + " - algorithm is already set");
            }

            if (i + 1 >= argumentCount)
            {
                throw runtime_error(R"(ERROR: expected "SUMMARIES", "SATURATION" or "BOTH")");
            }

            i++;
            string nextArgument(arguments[i]);

            if (isOneOf(nextArgument, OPTION_ALGORITHM_SUMMARIES))
            {
                benchmarkingParameters->algorithm = Algorithm::SUMMARIES;
                benchmarkingParameters->algorithmIsSet = true;
            }
            else if (isOneOf(nextArgument, OPTION_ALGORITHM_SATURATION))
            {
                benchmarkingParameters->algorithm = Algorithm::SATURATION;
                benchmarkingParameters->algorithmIsSet = true;
            }
            else if (isOneOf(nextArgument, OPTION_ALGORITHM_BOTH))
            {
                benchmarkingParameters->algorithm = Algorithm::BOTH;
                benchmarkingParameters->algorithmIsSet = true;
            }
            else
            {
                throw runtime_error("ERROR: unexpected parameter " + nextArgument + R"(, expected "SUMMARIES", "SATURATION" or "BOTH"))");
            }
        }
        else if (isOneOf(argument, OPTION_LETTERS))
        {
            if (benchmarkingParameters->numberOfLettersIsSet)
            {
                throw runtime_error("option " + argument + " - number of letters is already set");
            }

            if (i + 1 >= argumentCount)
            {
                throw runtime_error("expected an integer");
            }

            i++;
            string nextArgument(arguments[i]);
            unsigned int integer;
            try
            {
                integer = stoi(nextArgument);
            }
            catch (exception const& e)
            {
                throw runtime_error("unexpected parameter " + nextArgument + ", expected an integer");
            }

            benchmarkingParameters->numberOfLetters = integer;
            benchmarkingParameters->numberOfLettersIsSet = true;
        }
        else if (isOneOf(argument, OPTION_STATES))
        {
            if (benchmarkingParameters->numberOfStatesIsSet)
            {
                throw runtime_error("option " + argument + " - number of states is already set");
            }

            if (i + 1 >= argumentCount)
            {
                throw runtime_error("expected an integer");
            }

            i++;
            string nextArgument(arguments[i]);
            unsigned int integer;
            try
            {
                integer = stoi(nextArgument);
            }
            catch (exception const& e)
            {
                throw runtime_error("unexpected parameter " + nextArgument + ", expected an integer");
            }

            benchmarkingParameters->numberOfStates = integer;
            benchmarkingParameters->numberOfStatesIsSet = true;
        }
        else if (isOneOf(argument, OPTION_SEED))
        {
            if (benchmarkingParameters->seedIsSet)
            {
                throw runtime_error("option " + argument + " - seed is already set");
            }

            if (i + 1 >= argumentCount)
            {
                throw runtime_error("expected an integer");
            }

            i++;
            string nextArgument(arguments[i]);
            unsigned long longInteger;
            try
            {
                // we parse it as long because it may exceed the limit of (signed) int
                longInteger = stol(nextArgument);
            }
            catch (exception const& e)
            {
                throw runtime_error("unexpected parameter " + nextArgument + ", expected an integer");
            }

            if (longInteger > UINT_MAX)
            {
                throw runtime_error("parameter " + nextArgument + " is too long");
            }

            auto integer = (unsigned int) longInteger;

            benchmarkingParameters->seed = integer;
            benchmarkingParameters->seedIsSet = true;

        }
        else if (isOneOf(argument, OPTION_NONTERMINALS))
        {
            if (benchmarkingParameters->numberOfNonterminalsExistentialIsSet)
            {
                throw runtime_error("option " + argument + " - number of nonterminals for the Existential Player is already set");
            }

            if (benchmarkingParameters->numberOfNonterminalsUniversalIsSet)
            {
                throw runtime_error("option " + argument + " - number of nonterminals for the Universal Player is already set");
            }

            if (i + 1 >= argumentCount)
            {
                throw runtime_error("expected an integer");
            }

            i++;
            string nextArgument(arguments[i]);
            unsigned int integer;
            try
            {
                integer = stoi(nextArgument);
            }
            catch (exception const& e)
            {
                throw runtime_error("unexpected parameter " + nextArgument + ", expected an integer");
            }

            benchmarkingParameters->numberOfNonterminalsExistential = integer;
            benchmarkingParameters->numberOfNonterminalsUniversal = integer;
            benchmarkingParameters->numberOfNonterminalsExistentialIsSet = true;
            benchmarkingParameters->numberOfNonterminalsUniversalIsSet = true;
        }
        else if (isOneOf(argument, OPTION_NONTERMINALS_UNIVERSAL))
        {
            if (benchmarkingParameters->numberOfNonterminalsUniversalIsSet)
            {
                throw runtime_error("option " + argument + " - number of nonterminals for the Universal Player is already set");
            }

            if (i + 1 >= argumentCount)
            {
                throw runtime_error("expected an integer");
            }

            i++;
            string nextArgument(arguments[i]);
            unsigned int integer;
            try
            {
                integer = stoi(nextArgument);
            }
            catch (exception const& e)
            {
                throw runtime_error("unexpected parameter " + nextArgument + ", expected an integer");
            }

            benchmarkingParameters->numberOfNonterminalsUniversal = integer;
            benchmarkingParameters->numberOfNonterminalsUniversalIsSet = true;
        }
        else if (isOneOf(argument, OPTION_NONTERMINALS_EXISTENTIAL))
        {
            if (benchmarkingParameters->numberOfNonterminalsExistentialIsSet)
            {
                throw runtime_error("option " + argument + " - number of nonterminals for the Existential Player is already set");
            }

            if (i + 1 >= argumentCount)
            {
                throw runtime_error("expected an integer");
            }

            i++;
            string nextArgument(arguments[i]);
            unsigned int integer;
            try
            {
                integer = stoi(nextArgument);
            }
            catch (exception const& e)
            {
                throw runtime_error("unexpected parameter " + nextArgument + ", expected an integer");
            }

            benchmarkingParameters->numberOfNonterminalsExistential = integer;
            benchmarkingParameters->numberOfNonterminalsExistentialIsSet = true;
        }
        else if (isOneOf(argument, OPTION_AUTOMATONDENSITY))
        {
            if (benchmarkingParameters->automatonDensityIsSet)
            {
                throw runtime_error("option " + argument + " - automaton density is already set");
            }

            if (i + 1 >= argumentCount)
            {
                throw runtime_error("expected a double");
            }

            i++;
            string nextArgument(arguments[i]);
            double rate;
            try
            {
                rate = stod(nextArgument);
            }
            catch (exception const& e)
            {
                throw runtime_error("unexpected parameter " + nextArgument + ", expected a double");
            }

            benchmarkingParameters->automatonDensity = rate;
            benchmarkingParameters->automatonDensityIsSet = true;
        }
        else if (isOneOf(argument, OPTION_AUTOMATONFINALS))
        {
            if (benchmarkingParameters->automatonRateOfFinalsIsSet)
            {
                throw runtime_error("option " + argument + " - final rate is already set");
            }

            if (i + 1 >= argumentCount)
            {
                throw runtime_error("expected a double");
            }

            i++;
            string nextArgument(arguments[i]);
            double rate;
            try
            {
                rate = stod(nextArgument);
            }
            catch (exception const& e)
            {
                throw runtime_error("unexpected parameter " + nextArgument + ", expected a double");
            }

            benchmarkingParameters->automatonRateOfFinals = rate;
            benchmarkingParameters->automatonRateOfFinalsIsSet = true;
        }
        else if (isOneOf(argument, OPTION_GRAMMARDENSITY))
        {
            if (benchmarkingParameters->grammarDensityIsSet)
            {
                throw runtime_error("option " + argument + " - grammar density is already set");
            }

            if (i + 1 >= argumentCount)
            {
                throw runtime_error("expected a double");
            }

            i++;
            string nextArgument(arguments[i]);
            double rate;
            try
            {
                rate = stod(nextArgument);
            }
            catch (exception const& e)
            {
                throw runtime_error("unexpected parameter " + nextArgument + ", expected a double");
            }

            benchmarkingParameters->grammarDensity = rate;
            benchmarkingParameters->grammarDensityIsSet = true;
        }
        else if (isOneOf(argument, OPTION_GRAMMARFINALS))
        {
            if (benchmarkingParameters->grammarRateOfFinalsIsSet)
            {
                throw runtime_error("option " + argument + " - grammar final rate  is already set");
            }

            if (i + 1 >= argumentCount)
            {
                throw runtime_error("expected a double");
            }

            i++;
            string nextArgument(arguments[i]);
            double rate;
            try
            {
                rate = stod(nextArgument);
            }
            catch (exception const& e)
            {
                throw runtime_error("unexpected parameter " + nextArgument + ", expected a double");
            }

            benchmarkingParameters->grammarRateOfFinals = rate;
            benchmarkingParameters->grammarRateOfFinalsIsSet = true;
        }
        else if (isOneOf(argument, OPTION_GRAMMARLEFT))
        {
            if (benchmarkingParameters->grammarRateOfLeftIsSet)
            {
                throw runtime_error("option " + argument + " - grammar left rate  is already set");
            }

            if (i + 1 >= argumentCount)
            {
                throw runtime_error("expected a double");
            }

            i++;
            string nextArgument(arguments[i]);
            double rate;
            try
            {
                rate = stod(nextArgument);
            }
            catch (exception const& e)
            {
                throw runtime_error("unexpected parameter " + nextArgument + ", expected a double");
            }

            benchmarkingParameters->grammarRateOfLeft = rate;
            benchmarkingParameters->grammarRateOfLeftIsSet = true;
        }
        else if (isOneOf(argument, OPTION_GRAMMARRIGHT))
        {
            if (benchmarkingParameters->grammarRateOfRightIsSet)
            {
                throw runtime_error("option " + argument + " - grammar right rate  is already set");
            }

            if (i + 1 >= argumentCount)
            {
                throw runtime_error("expected a double");
            }

            i++;
            string nextArgument(arguments[i]);
            double rate;
            try
            {
                rate = stod(nextArgument);
            }
            catch (exception const& e)
            {
                throw runtime_error("unexpected parameter " + nextArgument + ", expected a double");
            }

            benchmarkingParameters->grammarRateOfRight = rate;
            benchmarkingParameters->grammarRateOfRightIsSet = true;
        }
        else
        {
            throw runtime_error("invalid argument " + argument);
        }


    } // for

    return benchmarkingParameters;
}

void Benchmarking::assignDefaultValues (BenchmarkingParameters* benchmarkingParameters)
{
    if (!benchmarkingParameters->logLevelIsSet)
    {
        benchmarkingParameters->logLevel = BenchmarkingParameters::LOGLEVEL_DEFAULT;
    }

    if (!benchmarkingParameters->algorithmIsSet)
    {
        benchmarkingParameters->algorithm = BenchmarkingParameters::ALGORITHM_DEFAULT;
    }

    if (!benchmarkingParameters->numberOfLettersIsSet)
    {
        benchmarkingParameters->numberOfLetters = BenchmarkingParameters::NUMBEROFLETTERS_DEFAULT;
    }

    if (!benchmarkingParameters->numberOfStatesIsSet)
    {
        benchmarkingParameters->numberOfStates = BenchmarkingParameters::NUMBEROFSTATES_DEFAULT;
    }

    if (!benchmarkingParameters->automatonDensityIsSet)
    {
        benchmarkingParameters->automatonDensity = BenchmarkingParameters::AUTOMATONDENSITY_DEFAULT;
    }

    if (!benchmarkingParameters->automatonRateOfFinalsIsSet)
    {
        benchmarkingParameters->automatonRateOfFinals = BenchmarkingParameters::AUTOMATONRATEOFFINALS_DEFAULT;
    }

    if (!benchmarkingParameters->numberOfNonterminalsUniversalIsSet)
    {
        benchmarkingParameters->numberOfNonterminalsUniversal = BenchmarkingParameters::NUMBEROFNONTERMINALSUNIVERSAL_DEFAULT;
    }

    if (!benchmarkingParameters->numberOfNonterminalsExistentialIsSet)
    {
        benchmarkingParameters->numberOfNonterminalsExistential = BenchmarkingParameters::NUMBEROFNONTERMINALSEXISTENTIAL_DEFAULT;
    }

    if (!benchmarkingParameters->grammarDensityIsSet)
    {
        benchmarkingParameters->grammarDensity = BenchmarkingParameters::GRAMMARDENSITY_DEFAULT;
    }

    if (!benchmarkingParameters->grammarRateOfFinalsIsSet)
    {
        benchmarkingParameters->grammarRateOfFinals = BenchmarkingParameters::GRAMMARRATEOFFINALS_DEFAULT;
    }

    if (!benchmarkingParameters->grammarRateOfLeftIsSet)
    {
        benchmarkingParameters->grammarRateOfLeft = BenchmarkingParameters::GRAMMARRATEOFLEFT_DEFAULT;
    }

    if (!benchmarkingParameters->grammarRateOfRightIsSet)
    {
        benchmarkingParameters->grammarRateOfRight = BenchmarkingParameters::GRAMMARRATEOFRIGHT_DEFAULT;
    }

    if (!benchmarkingParameters->seedIsSet)
    {
        random_device randomDevice;
        benchmarkingParameters->seed = randomDevice();
    }
}

void Benchmarking::logParameters (const BenchmarkingParameters& parameters, const Logger& logger)
{
    logger.msg("Benchmarking a randomly generated instance");

    logger.important("Seed: " + to_string(parameters.seed) + " (" +
                     (parameters.seedIsSet ? "provided by user" : "randomly generated") + ")", 0);

    if (logger.accepts(NORMAL))
    {

        logger.msg("Evaluation of the parameters");

        switch (parameters.algorithm)
        {
            case Algorithm::BOTH:
                logger.msg("Running both algorithms, Summaries and Saturation (default)");
                break;
            case Algorithm::SUMMARIES:
                logger.msg("Running the Summary algorithm");
                break;
            case Algorithm::SATURATION:
                logger.msg("Running the Saturation algorithm");
                break;
        }

        string logLevelString;
        switch (parameters.logLevel)
        {
            case ERROR:
                logLevelString = "ERROR (reduces output)";
                break;
            case NORMAL:
                logLevelString = "NORMAL (default)";
                break;
            case INFO:
                logLevelString = "INFO (verbose output)";
                break;
            case DEBUG:
                logLevelString = "DEBUG (very verbose output)";
                break;
        }

        logger.msg("LogLevel: " + logLevelString);
        logger.msg("Number of states: " + to_string(parameters.numberOfStates) + " (" + (parameters.numberOfStatesIsSet ? "provided by user" : "default value") + ")", 1);
        logger.msg("Number of letters: " + to_string(parameters.numberOfLetters) + " (" + (parameters.numberOfLettersIsSet ? "provided by user" : "default value") + ")", 1);
        logger.msg("Number of nonterminals of the Universal Player: " + to_string(parameters.numberOfNonterminalsUniversal) + " (" + (parameters.numberOfNonterminalsUniversalIsSet ? "provided by user" : "default value") + ")", 1);
        logger.msg("Number of nonterminals of the Existential Player: " + to_string(parameters.numberOfNonterminalsExistential) + " (" + (parameters.numberOfNonterminalsExistentialIsSet ? "provided by user" : "default value") + ")", 1);
        logger.msg("Density of the automaton: " + to_string(parameters.automatonDensity) + " (" + (parameters.automatonDensityIsSet ? "provided by user" : "default value") + ")", 1);
        logger.msg("Final rate of the automaton: " + to_string(parameters.automatonRateOfFinals) + " (" + (parameters.automatonRateOfFinalsIsSet ? "provided by user" : "default value") + ")", 1);
        logger.msg("Density of the grammar: " + to_string(parameters.grammarDensity) + " (" + (parameters.grammarDensityIsSet ? "provided by user" : "default value") + ")", 1);
        logger.msg("Final rate of the grammar: " + to_string(parameters.grammarRateOfFinals) + " (" + (parameters.grammarRateOfFinalsIsSet ? "provided by user" : "default value") + ")", 1);
        logger.msg("Left rate of the grammar: " + to_string(parameters.grammarRateOfLeft) + " (" + (parameters.grammarRateOfLeftIsSet ? "provided by user" : "default value") + ")", 1);
        logger.msg("Right rate of the grammar: " + to_string(parameters.grammarRateOfRight) + " (" + (parameters.grammarRateOfRightIsSet ? "provided by user" : "default value") + ")", 1);
    }
}


