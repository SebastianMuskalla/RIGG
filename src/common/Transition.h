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

#ifndef RIGG_TRANSITION_H
#define RIGG_TRANSITION_H

#include "Letter.h"

using namespace std;

/**
 * represents a transition of an NFA
 */
class Transition : public Printable<Transition>
{

public:
    Transition (Letter* origin, Letter* label, Letter* target) :
            source(origin),
            label(label),
            target(target)
    {}

    Letter* label;
    Letter* source;
    Letter* target;

    string toString () const override;

    bool operator== (const Transition& other) const
    {
        return label == other.label && source == other.source && target == other.target;
    }

    Transition (Transition const&) = delete;

    Transition& operator= (Transition const&) = delete;
};

#endif //RIGG_TRANSITION_H
