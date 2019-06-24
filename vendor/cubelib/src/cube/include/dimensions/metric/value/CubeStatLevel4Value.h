/****************************************************************************
**  CUBE        http://www.scalasca.org/                                   **
*****************************************************************************
**  Copyright (c) 1998-2018                                                **
**  Forschungszentrum Juelich GmbH, Juelich Supercomputing Centre          **
**                                                                         **
**  Copyright (c) 2009-2015                                                **
**  German Research School for Simulation Sciences GmbH,                   **
**  Laboratory for Parallel Programming                                    **
**                                                                         **
**  This software may be modified and distributed under the terms of       **
**  a BSD-style license.  See the COPYING file in the package base         **
**  directory for details.                                                 **
****************************************************************************/


/**
 * \file CubeStatLevel4Value.h
 * \brief Provide a value, which carries an "average" value, variance, skewness and kurtosis. Sum of two "CubeStatLevel4Values" results again a valid average value, variance, skewness and kurtosis.
 *
 * It saves an int N and (\f$ \sum_i^N {x_i} \f$) for calculation of average.
 *
 *
 */

#ifndef __STAT_LEVEL4_VALUE_H
#define __STAT_LEVEL4_VALUE_H

#include <istream>

#include <cstring>
#include <string>
#include <iostream>
#include "CubeValues.h"
#include "CubeTrafos.h"


namespace cube
{
class Connection;
class StatLevel4Value;
extern Preallocator<StatLevel4Value> stat4_preallocator;

typedef   struct
{
    int pivot;
}  __attribute__ ( ( __packed__ ) ) StatLevel4ValueConfig;

/**
 *  The TAU value for atomic metrics. NOT TESTED
 */
class StatLevel4Value : public Value
{
private:

    static
    std::vector<StatLevel4ValueConfig> parameters;
    size_t                             index;


    double
    calcAvg() const;                                                           // /< Performs the calcualtion of Average, Variance and Skewness
    double
    calcVar() const;                                                           // /< Performs the calcualtion of Average, Variance and Skewness
    double
    calcSkew() const;                                                          // /< Performs the calcualtion of Average, Variance and Skewness
    double
    calcKur() const;                                                           // /< Performs the calcualtion of Average, Variance and Skewness

protected:
    UnsignedValue N;
    DoubleValue   Sum;
    DoubleValue   Sum2;
    DoubleValue   Sum3;
    DoubleValue   Sum4;

public:
    StatLevel4Value();
    StatLevel4Value( uint32_t,
                     double,
                     double,
                     double,
                     double );

    virtual
    ~StatLevel4Value()
    {
    };                                                                 // /< Vitual destructor allows, that every inherited class destroy itself properly.
    virtual unsigned
    getSize() const;                                                   // /< Value has to define size of it in bytes. Used to apply transformations and saving/loading in the file.
    virtual double
    getDouble() const;                                                 // /< Returns the double representation of itself, if possible.
    virtual uint16_t
    getUnsignedShort() const;                                          // /< Returns the unsigned 32bit representation of itself, if possible.
    virtual int16_t
    getSignedShort() const;                                            // /< Returns the signed 32bit representation of itself, if possible.
    virtual uint32_t
    getUnsignedInt() const;                                            // /< Returns the unsigned 32bit representation of itself, if possible.
    virtual int32_t
    getSignedInt() const;                                              // /< Returns the signed 32bit representation of itself, if possible.
    virtual uint64_t
    getUnsignedLong() const;                                           // /< Returns the unsigned 64bit representation of itself, if possible.
    virtual int64_t
    getSignedLong() const;                                             // /< Returns the signed 64bit representation of itself, if possible.
    virtual char
    getChar() const;                                                   // /< Returns the single byte representation of itself, if possible.
    virtual std::string
    getString() const;                                                 // /< Returns the textual representation of itself, if possible. Used to save itself in Cube3 format.

    virtual UnsignedValue
    getN() const
    {
        return N;
    };
    virtual DoubleValue
    getSum() const
    {
        return Sum;
    };
    virtual DoubleValue
    getSum2() const
    {
        return Sum2;
    };
    virtual DoubleValue
    getSum3() const
    {
        return Sum3;
    };
    virtual DoubleValue
    getSum4() const
    {
        return Sum4;
    };

    void*
    operator new( size_t size );
    void
    operator delete( void* p );

    virtual void
    Free()
    {
        delete ( StatLevel4Value* )this;
    }


    virtual const char*
    fromStream( const char* );                                                        // /< Constructs itself from the stream. The first stream byte used as starting point.
    virtual const double*
    fromStreamOfDoubles( const double* stream );
    virtual char*
    toStream( char* ) const;                                                          // /< Write to the stream the stream-representation of itself. The first stream byte used as starting point.

    /**
     * Assign value from a Cube connection.
     * @param connection Cube connection stream.
     */
    virtual void
    fromStream( Connection& connection );

    /**
     * Send value through a Cube connection.
     * @param connection Cube connection stream.
     */
    virtual void
    toStream( Connection& connection ) const;

    virtual char*
    transformStream( char*,
                     SingleValueTrafo* ) const;                                           // /< Applyes the transformation on the stream according to its layout. Used to deal with different endianess. Needed for comples types with non trivial layout.

    virtual Value*
    clone() const;                                                                     // /< Makes a copy of itself and sets the value of the copy to 0.
    virtual Value*
    copy() const;                                                                      // /< Makes a copy of itself and keeps the value.


    virtual void
    operator+=( Value* );                                                         // /< Defines + operation (for calculation of the excl/incl values).
    virtual void
    operator-=( Value* );                                                         // /< Defines - operation (for calculation of the excl values).
    virtual void
    operator*=( double );                                                         // /< Defines * operation (for calculation of the excl/incl values). Needed by algebra tools.
    virtual void
    operator/=( double );                                                         // /< Defines / operation (for calculation of the excl/incl values). Needed by algebra tools.

    virtual void
    operator=( double );                                       // /< Allows to assign its value from the build in double.

    virtual void
    operator=( Value* );                                       // /< Allows to assign its value from another object of same type. [WARNING, real type is not checked]


    virtual bool
    isZero() const
    {
        return Sum.isZero() && Sum2.isZero() && Sum3.isZero() && Sum4.isZero() && N.isZero();
    };

    virtual DataType
    myDataType() const
    {
        return CUBE_DATA_TYPE_NONE;
    };                                             // not supported yet
    virtual void
        normalizeWithClusterCount( uint64_t );

    virtual
    void
    init_new();

    virtual
    void
    clone_new( const Value* );
};
}



#endif
