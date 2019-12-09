#pragma once

#ifndef HELPMATH_H
#define HELPMATH_H

#include "Config.h"

#define _MAX(a,b) (a > b)? a : b
#define _MIN(a,b) (a < b)? a : b

namespace GJK
{
    // Summatory => sum an array of values
    template<class _InIt, class _ValueType> 
    INLINE void Sum(_InIt ArrayBase, unsigned int Count, _ValueType& Result)
    {	 
        // sum( First+i + Second+i ) where 0<=i<Count to Result

        Result = _ValueType();
        for(int i=0; i < Count; (++ArrayBase,++i))
            Result += *ArrayBase;
        return;
    }

    // Productory => multiply an array of values
    template<class _InIt, class _ValueType> 
    INLINE void Mult(_InIt ArrayBase, unsigned int Count, _ValueType& Result)
    {	 
        // mult( First+i * Second+i ) where 0<=i<Count to Result

        Result = _ValueType();
        for(int i=0; i < Count; ++ArrayBase)
            Result *= *ArrayBase;
        return;
    }

    // Dot Product between 2 arrays
    template<class _InIt, class _ValueType> 
    INLINE void Dot(_InIt First, _InIt Second, unsigned int Count, _ValueType& Result)
    {	 
        // Dot( First+i , Second+i ) where 0<=i<Count to Result

        Result = _ValueType();
        for(int i=0; i < Count; (++First,++Second,++i))
            Result += (*First) * (*Second);
        return;
    }

    // Norm 2 of the two vectors
    template<class _InIt, class _ValueType> 
    INLINE void Norm(_InIt First, _InIt Second, unsigned int Count, _ValueType& Result)
    {	 
        // Norm( First+i , Second+i ) where 0<=i<Count to Result

        Result = _ValueType();
        for(int i=0; i < Count; (++First,++Second,++i))
        {
            _ValueType aux = (*First) - (*Second);
            Result += aux * aux;
        }
        Result = sqrt(Result);
        return;
    }

    // Mean Square Error Product between 2 arrays
    template<class _InIt, class _ValueType> 
    INLINE void MeanSquareErr(_InIt First, _InIt Second, unsigned int Count, _ValueType& Result)
    {	 
        // MeanSquareErr( First+i , Second+i ) where 0<=i<Count to Result

        Result = _ValueType();
        for(int i=0; i < Count; (++First,++Second,++i))
        {
            _ValueType aux = (*First) - (*Second);
            Result += aux * aux;
        }
        Result /= Count;
        return;
    }

    // Linear Interpolation between value A and B with coeficient coef
    // coef = 0 -> res = A			coef = 1 -> res = B
    template<class _ValueType> 
    INLINE _ValueType LERP(_ValueType A, _ValueType B, float coef)
    {	 
        // A * (1-coef) + B * coef

        return (A * (1-coef) + B * coef);
    }

    // Saturate a value, ie clamp it to the range [0,1]
    template<class _ValueType> 
    INLINE _ValueType Saturate(_ValueType Val)
    {	 
        _ValueType aux = _MAX(0,Val);
        _ValueType aux2 = _MIN(1, aux);
        return aux2;
        //return _MIN(static_cast<_ValueType>(1), _MAX(static_cast<_ValueType>(0),Val));//gives problems because of casting most probably
    }

    // Clamp a value to the range [Min,Max]
    template<class _ValueType> 
    INLINE _ValueType Clamp(_ValueType Val, _ValueType Min, _ValueType Max)
    {	 
        _ValueType aux = _MAX(Min,Val);
        _ValueType aux2 = _MIN(Max, aux);
        return aux2;
        //return _MIN(Max, _MAX(Min,Val));//gives problems because of casting most probably
    }
}

#undef _MIN
#undef _MAX

#endif // end HELPMATH_H
