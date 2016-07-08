//
// Created by Sebastian on 07.07.2016.
//

#ifndef RIGG_PRINTABLE_H
#define RIGG_PRINTABLE_H


#include <c++/4.8.3/string>
//#include <ostream>
#include <c++/4.8.3/iostream>

using namespace std;

/**
 * Interface for printable classes.
 * Provides interface for toString() method.
 * Objects can be put into ostream.
 */
template<class T>
class Printable
{
public:
    /**
     * Creates string representation.
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
