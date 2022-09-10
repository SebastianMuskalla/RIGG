
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

#ifndef RIGG_PRINTABLE_H
#define RIGG_PRINTABLE_H

#include <string>
#include <iostream>

using namespace std;

/**
 * interface for printable classes.
 *
 * provides virtual toString() method
 *
 * printable objects can be put into ostream
 */
template<class T>
class Printable
{
public:
    /**
     * creates string representation
     */
    virtual string toString () const = 0;

    /**
     * puts string representation of object into stream
     */
    friend ostream& operator<< (ostream& os, T t)
    {
        os << t.toString();
        os.flush();
        return os;
    }
};

#endif //RIGG_PRINTABLE_H
