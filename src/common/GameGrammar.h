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

#ifndef RIGG_GAMEGRAMMAR_H
#define RIGG_GAMEGRAMMAR_H

#include <unordered_map>
#include <map>
#include "Alphabet.h"

using namespace std;

/**
 * grammar with a ownership partitioning on the non-terminals
 */
class GameGrammar : public Printable<GameGrammar>
{
public:
    /**
     * terminals
     */
    Alphabet* Sigma;

    /**
     * non-terminals owned by the Universal Player
     */
    Alphabet* NUniversal;

    /**
     * non-terminals owned by the Existential Player
     */
    Alphabet* NExistential;


    /**
     * production rules of the shape
     * X from -> {word over Sigma union N},
     * where N = NExistential union NUniversal
     */
    multimap<Letter*, vector<Letter*>> rules;

    GameGrammar (Alphabet* Sigma, Alphabet* NUniversal, Alphabet* NExistential) :
            Sigma(Sigma),
            NExistential(NExistential),
            NUniversal(NUniversal)
    {}

    /**
     * we do not delete the terminal alphabet
     */
    virtual ~GameGrammar ();

    /**
     * add rule with specified non-terminals als left-hand side and specified sentential form as right-hand-side to the grammar
     */
    void addRule (Letter* lhs, const vector<Letter*>& rhs)
    {
        rules.insert(make_pair(lhs, rhs));
    }

    /**
     * check whether a letter is a non-terminal owned by either player
     */
    bool isNonterminal (Letter* l) const
    {
        return (l->alphabet == NUniversal || l->alphabet == NExistential);
    }

    string toString () const override;

    GameGrammar (GameGrammar const&) = delete;

    GameGrammar& operator= (GameGrammar const&) = delete;
};

#endif //RIGG_GAMEGRAMMAR_H
