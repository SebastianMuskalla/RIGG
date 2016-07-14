//
// Created by Sebastian on 13.07.2016.
//

#ifndef RIGG_POINTEECOMPARATOR_H
#define RIGG_POINTEECOMPARATOR_H

/**
 * goal: compares two pointers for equality of the objects they point to
 *
 * Initialized with some pointer t1, objects of this class provide a "functor" such that object(t2) returns true iff
 * *t1 == *t2
 */

template<typename T>
struct PointeeComparator
{
    const T* to_find;

    /**
     * initialize with the pointer pointing to the object that should be compared to
     */
    PointeeComparator (const T* to_find) :
            to_find(to_find)
    { }

    /**
     * compare the pointees
     */
    bool operator() (const T* other) const
    {
        return *to_find == *other;
    }
};

#endif //RIGG_POINTEECOMPARATOR_H
