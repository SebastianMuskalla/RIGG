//
// Created by Sebastian on 07.07.2016.
//

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
    friend ostream &operator<< (ostream &os, T t)
    {
        os << t.toString();
        os.flush();
        return os;
    }
};

#endif //RIGG_PRINTABLE_H
