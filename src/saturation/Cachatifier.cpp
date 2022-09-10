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

#include "Cachatifier.h"

using namespace std;

tuple<GamePDS*, PAFA*, Letter*, Letter*> Cachatifier::cachatify ()
{
    Alphabet* statesExistential;
    Alphabet* statesUniversal;
    Alphabet* Gamma;
    map<Letter*, Letter*> statesExistentialMap;
    map<Letter*, Letter*> statesUniversalMap;

    auto* resPDS = new GamePDS();

    Gamma = new Alphabet();
    statesExistential = new Alphabet();
    statesUniversal = new Alphabet();

    resPDS->statesUniversal = statesUniversal;
    resPDS->statesExistential = statesExistential;
    resPDS->Gamma = Gamma;

    for (Letter* a : Sigma->letters)
    {
        Letter* x = Gamma->addLetter(a->toString());
        stackTerminals.emplace(a, x);
    }
    for (Letter* X : NExistential->letters)
    {
        Letter* x = Gamma->addLetter(X->toString());
        stackNonterminalsExistential.emplace(X, x);
    }
    for (Letter* Y : NUniversal->letters)
    {
        Letter* x = Gamma->addLetter(Y->toString());
        stackNonterminalsUniversal.emplace(Y, x);
    }

    for (Letter* q : DFA->Q->letters)
    {
        Letter* qExistential = statesExistential->addLetter(q->toString().append("_ex"));
        Letter* qUniversal = statesUniversal->addLetter(q->toString().append("_all"));
        statesExistentialMap.emplace(q, qExistential);
        statesUniversalMap.emplace(q, qUniversal);

    }

    PAFA* resAFA = new PAFA(resPDS);
    resAFA->controlStates = new Alphabet();

    for (Letter* q : statesExistential->letters)
    {
        Letter* qAFA = resAFA->controlStates->addLetter(q->toString());
        resAFA->pdsStateToAFAState.emplace(q, qAFA);
    }
    for (Letter* q : statesUniversal->letters)
    {
        Letter* qAFA = resAFA->controlStates->addLetter(q->toString());
        resAFA->pdsStateToAFAState.emplace(q, qAFA);
    }

    for (Letter* q : DFA->Q->letters)
    {
        if (DFA->finalStates.find(q) == DFA->finalStates.end())
        {
            Letter* qUniversal = statesUniversalMap[q];
            Letter* qExistential = statesExistentialMap[q];
            Letter* qUniversalAFA = resAFA->pdsStateToAFAState[qUniversal];
            Letter* qExistentialAFA = resAFA->pdsStateToAFAState[qExistential];

            resAFA->finalStates.insert(qUniversalAFA);
            resAFA->finalStates.insert(qExistentialAFA);
        }
    }

    // transitions that change between players
    for (Letter* q : DFA->Q->letters)
    {
        Letter* qExistential = statesExistentialMap[q];
        Letter* qUniversal = statesUniversalMap[q];

        for (Letter* x : NExistential->letters)
        {
            Letter* X = stackNonterminalsExistential[x];
            auto* p = new PDSTransition(qUniversal, X, {X}, qExistential);
            resPDS->transitions.insert(p);
        }
        for (Letter* y : NUniversal->letters)
        {
            Letter* Y = stackNonterminalsUniversal[y];
            auto* p = new PDSTransition(qExistential, Y, {Y}, qUniversal);
            resPDS->transitions.insert(p);
        }
    }


    // transitions that process terminals
    for (Transition* t : DFA->transitions)
    {
        Letter* sourceExistential = statesExistentialMap[t->source];
        Letter* sourceUniversal = statesUniversalMap[t->source];
        Letter* targetExistential = statesExistentialMap[t->target];
        Letter* targetUniversal = statesUniversalMap[t->target];
        Letter* x = stackTerminals[t->label];

        auto* p1 = new PDSTransition(sourceExistential, x, {}, targetExistential);
        auto* p2 = new PDSTransition(sourceUniversal, x, {}, targetUniversal);
        resPDS->transitions.insert(p1);
        resPDS->transitions.insert(p2);
    }

    // transitions that process nonterminals owned by the Existential Player
    for (Letter* X : NExistential->letters)
    {

        Letter* stackX = stackNonterminalsExistential.find(X)->second;

        auto itrPair = G->rules.equal_range(X);

        for (auto itr = itrPair.first; itr != itrPair.second; ++itr)
        {

            vector<Letter*> stackWord;
            for (Letter* l : itr->second)
            {
                if (l->alphabet == Sigma)
                {
                    Letter* x = stackTerminals[l];
                    stackWord.push_back(x);
                }
                if (l->alphabet == NUniversal)
                {
                    Letter* x = stackNonterminalsUniversal[l];
                    stackWord.push_back(x);
                }
                if (l->alphabet == NExistential)
                {
                    Letter* x = stackNonterminalsExistential[l];
                    stackWord.push_back(x);
                }
            }

            for (Letter* q : statesExistential->letters)
            {
                auto* p = new PDSTransition(q, stackX, stackWord, q);
                resPDS->transitions.insert(p);
            }
        }
    }

    // transitions that process nonterminals owned by the Universal Player
    for (Letter* Y : NUniversal->letters)
    {
        Letter* stackY = stackNonterminalsUniversal[Y];

        auto itrPair = G->rules.equal_range(Y);

        for (auto itr = itrPair.first; itr != itrPair.second; ++itr)
        {
            vector<Letter*> stackWord;
            for (Letter* l : itr->second)
            {
                if (l->alphabet == Sigma)
                {
                    Letter* x = stackTerminals[l];
                    stackWord.push_back(x);
                }
                if (l->alphabet == NUniversal)
                {
                    Letter* x = stackNonterminalsUniversal[l];
                    stackWord.push_back(x);
                }
                if (l->alphabet == NExistential)
                {
                    Letter* x = stackNonterminalsExistential[l];
                    stackWord.push_back(x);
                }
            }

            for (Letter* q : statesUniversal->letters)
            {
                auto* p = new PDSTransition(q, stackY, stackWord, q);
                resPDS->transitions.insert(p);
            }
        }
    }

    Letter* initialExistential = statesExistentialMap[DFA->initialState];
    Letter* initialUniversal = statesUniversalMap[DFA->initialState];

    return make_tuple(resPDS, resAFA, initialUniversal, initialExistential);

}


vector<Letter*> Cachatifier::wordToStackWord (const vector<Letter*>& word)
{
    vector<Letter*> res;
    for (Letter* l : word)
    {
        if (l->alphabet == Sigma)
        {
            res.push_back(stackTerminals[l]);
        }
        if (l->alphabet == NUniversal)
        {
            res.push_back(stackNonterminalsUniversal[l]);
        }
        if (l->alphabet == NExistential)
        {
            res.push_back(stackNonterminalsExistential[l]);
        }
    }
    return res;
}


Cachatifier::Cachatifier (NFA* DFA, GameGrammar* G)
        :
        DFA(DFA),
        G(G),
        Sigma(G->Sigma),
        NExistential(G->NExistential),
        NUniversal(G->NUniversal)
{}
