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

#ifndef RIGG_CACHATIFIER_H
#define RIGG_CACHATIFIER_H

#include "../common/NFA.h"
#include "../common/GameGrammar.h"
#include "GamePDS.h"
#include "PAFA.h"

using namespace std;

/**
 * converts a given grammar game (i.e. game grammar G and DFA A specifying the goal language) to an instance for SaturationSolver's pushdown game
 *
 * note that we require A to be deterministic, because otherwise the players are allowed to resolve the non-determinism in the automaton for the goal language, which changes the semantics of the game
 *
 * we create a pushdown system that
 * - has a state q_ex (owned by the Existential Player) and a state q_all (owned by the Universal Player) for each state q of A
 * - has the union of terminals and non-terminals as stack_symbols
 * - has for each state q, each player pl, and each terminal x a rule that pops x in q_pl and goes to q'_pl, where q' is the (unique) state such that q -- x --> q' in A
 * - has for each state q, each player pl, each nonterminal X owned by pl, and each rule X -> RHS a rule that pops X, pushes RHS (while the state stays in Qp)
 * - has for each state q, each player pl, and each nonterminal X not owned by pl a rule that goes from q_pl to q_notpl without modifying the stack
 *
 * the goal automaton accepts the empty stack in all states that are not accepting in A, i.e. it accepts if we have successfully processed a terminal word that is not in the language of A
 */
class Cachatifier
{
private:
    NFA* DFA;
    GameGrammar* G;
    Alphabet* Sigma;
    map<Letter*, Letter*> stackTerminals;
    map<Letter*, Letter*> stackNonterminalsExistential;
    map<Letter*, Letter*> stackNonterminalsUniversal;
    Alphabet* NExistential;
    Alphabet* NUniversal;

public:
    /**
     * Cachatifies the grammar game
     *
     * first component of tuple: Game pushdown system (GamePDS)
     * second component: PAFA describing the target set of the Existential player
     * third component: initial state of the Universal player in the GamePDS
     * fourth component: initial state of the Existential player in the GamePDS
     *
     * (actually, it does not matter with which of these two states one starts)
     */
    tuple<GamePDS*, PAFA*, Letter*, Letter*> cachatify ();

    /**
     * converts a sentential from of the grammar to the word containing the corresponding stack symbols
     */
    vector<Letter*> wordToStackWord (const vector<Letter*>& word);

    Cachatifier (NFA* DFA, GameGrammar* G);

    Cachatifier (Cachatifier const&) = delete;

    Cachatifier& operator= (Cachatifier const&) = delete;
};

#endif //RIGG_CACHATIFIER_H
