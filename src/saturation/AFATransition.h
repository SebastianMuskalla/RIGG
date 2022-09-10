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

#ifndef RIGG_AFATRANSITION_H
#define RIGG_AFATRANSITION_H

#include <set>
#include "../common/Letter.h"

using namespace std;

/**
 * transition of an alternating finite automaton
 */
class AFATransition : public Printable<AFATransition>
{
public:
    Letter* label;
    Letter* source;
    /**
     * Each set {q_1, ..., q_n} represents the conjunction q_1 and ... and q_n.
     * Disjunctions are captured by nondeterminism.
     */
    set<Letter*> targets;

    AFATransition (Letter* source, Letter* label, const set<Letter*>& targets) :
            label(label),
            source(source),
            targets(targets)
    {}

    string toString () const override;

    bool operator== (const AFATransition& other) const
    {
        return label == other.label && source == other.source && targets == other.targets;
    }

    AFATransition (AFATransition const&) = delete;

    AFATransition& operator= (AFATransition const&) = delete;
};

#endif //RIGG_AFATRANSITION_H
