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

#ifndef RIGG_POINTEECOMPARATOR_H
#define RIGG_POINTEECOMPARATOR_H


/**
 * compares two pointers for equality of the objects they point to
 *
 * Initialized with some pointer t1, objects of this class provide a functor such that object(t2) returns true iff
 * *t1 == *t2
 */

template<typename T>
struct PointeeComparator
{
    const T* object;

    /**
     * initialize with the pointer pointing to the object that should be compared to
     */
    explicit PointeeComparator (const T* object) :
            object(object)
    {}

    /**
     * compare the pointees
     */
    bool operator() (const T* other) const
    {
        return *object == *other;
    }
};

#endif //RIGG_POINTEECOMPARATOR_H
