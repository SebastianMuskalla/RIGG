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

#include "Examples.h"


bool Examples::equalsUpToCase (string first, string second)
{
    transform(first.begin(), first.end(), first.begin(), ::tolower);
    transform(second.begin(), second.end(), second.begin(), ::tolower);
    return (first == second);
}


template<size_t S>
bool Examples::isOneOf (const string& argument, const string_view (& strings)[S])
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


bool Examples::shouldRunExamples (int argumentCount, char** arguments)
{
    for (int i = 1; i < argumentCount; i++)
    {
        string argument(arguments[i]);

        if (isOneOf(argument, OPTION_EXAMPLE))
        {
            return true;
        }
    }
    return false;
}

int Examples::runExamples ()
{
    Logger* logger = new CoutLogger();

    runExample1(*logger);

    logger->newline();
    logger->newline();

    runExample2(*logger);

    return EXIT_CODE_OK;
}

/**
 * example from the paper
 *
 * grammar
 * X -> aY | epsilon
 * Y -> bX
 *
 * automaton accepts (ab)*
 */
void Examples::runExample1 (const Logger& logger)
{
    auto* Sigma = new Alphabet();
    Letter* a = Sigma->addLetter("a");
    Letter* b = Sigma->addLetter("b");

    auto* NUniversal = new Alphabet();
    Letter* Y = NUniversal->addLetter("Y");

    auto* NExistential = new Alphabet();
    Letter* X = NExistential->addLetter("X");

    auto* G = new GameGrammar(Sigma, NUniversal, NExistential);
    G->addRule(X, {a, Y});
    G->addRule(X, {});
    G->addRule(Y, {b, X});

    auto* Q = new Alphabet();
    Letter* q0 = Q->addLetter("q0");
    Letter* q1 = Q->addLetter("q1");
    set<Letter*> finals = {q0};

    NFA* A = new NFA(Sigma, Q, q0, finals);
    A->addTransition(q0, a, q1);
    A->addTransition(q1, b, q0);

    logger.msg("Basic example from the paper");
    logger.msg("============================");

    logger.msg("Grammar:");
    logger.msg(G->toString(), 1);

    logger.msg("Automaton:");
    logger.msg(A->toString(), 1);

    logger.newline();
    logger.msg("Solving it using summaries");
    logger.msg("--------------------------");

    logger.msg("running the summary solver...");

    auto* indentedLogger = new IndentedLogger(logger, 1);

    auto* summarySolver = new SummarySolver(A, G, *indentedLogger);
    summarySolver->solve();

    logger.msg("solution:");

    Formula* solX = summarySolver->formulaFor(X);
    logger.msg("Formula for X: " + solX->toString() + (solX->isRejecting() ? " IS REJECTING" : " IS NOT REJECTING"),
               1);
    logger.msg("(expected result: Universal Player wins -> formula is not rejecting)", 1);

    Formula* solY = summarySolver->formulaFor(Y);
    logger.msg("Formula for Y: " + solY->toString() + (solY->isRejecting() ? " IS REJECTING" : " IS NOT REJECTING"),
               1);
    logger.msg("(expected result: Existential Player wins -> formula is rejecting)", 1);

    logger.newline();
    logger.msg("Solving it using saturation");
    logger.msg("---------------------------");

    NFA* D = Determinizer::determinize(A);

    logger.msg("determinizing the automaton...");
    logger.msg("Deterministic automaton:");
    logger.msg(D->toString(), 1);

    NFA* M = Minimizer::minimize(D);

    logger.msg("minimizing the automaton...");
    logger.msg("Minimal deterministic automaton:");
    logger.msg(M->toString(), 1);

    auto* cachatifier = new Cachatifier(M, G);
    tuple<GamePDS*, PAFA*, Letter*, Letter*> tuple = cachatifier->cachatify();
    GamePDS* P = get<0>(tuple);
    PAFA* AFA = get<1>(tuple);
    Letter* initialStateUniversal = get<2>(tuple);
    Letter* initialStateExistential = get<3>(tuple);

    logger.msg("transforming it into a pushdown game...");
    logger.msg("Pushdown system:");
    logger.msg(P->toString(), 1);
    logger.msg("AFA for the target set:");
    logger.msg(AFA->toString(), 1);

    logger.msg("running the saturation solver...");

    auto* saturationSolver = new SaturationSolver(P, AFA, *indentedLogger);
    saturationSolver->saturate();

    logger.msg("AFA for the winning region:");
    logger.msg(AFA->toString(), 1);

    vector<Letter*> stackX = cachatifier->wordToStackWord({X});
    Letter* initialStateExistentialAFA = saturationSolver->convertToAFAState(initialStateExistential);
    bool existentialPlayerWinsFromX = AFA->acceptsFromControlState(initialStateExistentialAFA, stackX);

    logger.msg("For X: " + string() +
               (existentialPlayerWinsFromX ? "Existential Player wins " : "Universal Player wins"));

    vector<Letter*> stackY = cachatifier->wordToStackWord({Y});
    Letter* initialStateUniversalAFA = saturationSolver->convertToAFAState(initialStateUniversal);
    bool existentialPlayerWinsFromY = AFA->acceptsFromControlState(initialStateUniversalAFA, stackY);

    logger.msg("For Y: " + string() +
               (existentialPlayerWinsFromY ? "Existential Player wins " : "Universal Player wins"));
}

void Examples::runExample2 (const Logger& logger)
{
    auto* Sigma = new Alphabet();
    Letter* a = Sigma->addLetter("a");
    Letter* b = Sigma->addLetter("b");

    auto* NUniversal = new Alphabet();
    Letter* Y = NUniversal->addLetter("Y");

    auto* NExistential = new Alphabet();
    Letter* X = NExistential->addLetter("X");

    auto* G = new GameGrammar(Sigma, NUniversal, NExistential);
    G->addRule(X, {a, Y});
    G->addRule(X, {b, Y});
    G->addRule(Y, {a, X});
    G->addRule(Y, {b, X});
    // termination rule is missing intentionally

    auto* Q = new Alphabet();
    Letter* q0 = Q->addLetter("q0");
    Letter* qf = Q->addLetter("qf");
    Letter* qa1 = Q->addLetter("qa1");
    Letter* qa2 = Q->addLetter("qa2");
    Letter* qb1 = Q->addLetter("qb1");
    Letter* qb2 = Q->addLetter("qb2");
    set<Letter*> finals = {qf};

    NFA* A = new NFA(Sigma, Q, q0, finals);
    A->addTransition(q0, a, qa1);
    A->addTransition(qa1, a, qa2);
    A->addTransition(qa2, a, qf);
    A->addTransition(q0, b, qb1);
    A->addTransition(qb1, b, qb2);
    A->addTransition(qb2, b, qf);

    logger.msg("Another example where the derivations don't terminate");
    logger.msg("=====================================================");

    logger.msg("Grammar:");
    logger.msg(G->toString(), 1);

    logger.msg("Automaton:");
    logger.msg(A->toString(), 1);

    logger.newline();
    logger.msg("Solving it using summaries");
    logger.msg("--------------------------");

    logger.msg("running the summary solver...");

    auto* indentedLogger = new IndentedLogger(logger, 1);

    auto* summarySolver = new SummarySolver(A, G, *indentedLogger);
    summarySolver->solve();

    logger.msg("solution:");

    Formula* solX = summarySolver->formulaFor(X);
    logger.msg("Formula for X: " + solX->toString() + (solX->isRejecting() ? " IS REJECTING" : " IS NOT REJECTING"),
               1);
    logger.msg("(expected result: Universal Player wins -> formula is not rejecting)", 1);

    Formula* solY = summarySolver->formulaFor(Y);
    logger.msg("Formula for Y: " + solY->toString() + (solY->isRejecting() ? " IS REJECTING" : " IS NOT REJECTING"),
               1);
    logger.msg("(expected result: Universal Player wins -> formula is not rejecting)", 1);

    logger.newline();
    logger.msg("Solving it using saturation");
    logger.msg("--------------------------");

    NFA* D = Determinizer::determinize(A);

    logger.msg("determinizing the automaton...");
    logger.msg("Deterministic automaton:");
    logger.msg(D->toString(), 1);

    NFA* M = Minimizer::minimize(D);

    logger.msg("minimizing the automaton...");
    logger.msg("Minimal deterministic automaton:");
    logger.msg(M->toString(), 1);

    auto* cachatifier = new Cachatifier(M, G);
    tuple<GamePDS*, PAFA*, Letter*, Letter*> tuple = cachatifier->cachatify();
    GamePDS* P = get<0>(tuple);
    PAFA* AFA = get<1>(tuple);
    Letter* initialStateUniversal = get<2>(tuple);
    Letter* initialStateExistential = get<3>(tuple);

    logger.msg("transforming it into a pushdown game...");
    logger.msg("Pushdown system:");
    logger.msg(P->toString(), 1);
    logger.msg("AFA for the target set:");
    logger.msg(AFA->toString(), 1);

    logger.msg("running the saturation solver...");

    auto* saturationSolver = new SaturationSolver(P, AFA, *indentedLogger);
    saturationSolver->saturate();

    logger.msg("AFA for the winning region:");
    logger.msg(AFA->toString(), 1);

    vector<Letter*> stackX = cachatifier->wordToStackWord({X});
    Letter* initialStateExistentialAFA = saturationSolver->convertToAFAState(initialStateExistential);
    bool existentialPlayerWinsFromX = AFA->acceptsFromControlState(initialStateExistentialAFA, stackX);

    logger.msg("For X: " + string() +
               (existentialPlayerWinsFromX ? "Existential Player wins " : "Universal Player wins"));

    vector<Letter*> stackY = cachatifier->wordToStackWord({Y});
    Letter* initialStateUniversalAFA = saturationSolver->convertToAFAState(initialStateUniversal);
    bool existentialPlayerWinsFromY = AFA->acceptsFromControlState(initialStateUniversalAFA, stackY);

    logger.msg("For Y: " + string() +
               (existentialPlayerWinsFromY ? "Existential Player wins " : "Universal Player wins"));
}
