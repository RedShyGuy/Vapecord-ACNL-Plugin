#include "CTRPluginFrameworkImpl/Search/Search32.hpp"

namespace CTRPluginFramework
{
    void    Search32::FirstSearchSpecifiedU8(u32 endAddress, SearchFlags compare, Results32* result)
    {
        switch (compare)
        {
            case SearchFlags::Equal:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u8 value = *(u8 *)_currentAddress;

                    if (EQ(value, _checkValue.U8))
                    {
                        *result++ = {_currentAddress, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::NotEqual:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u8 value = *(u8 *)_currentAddress;

                    if (NE(value, _checkValue.U8))
                    {
                        *result++ = {_currentAddress, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::GreaterThan:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u8 value = *(u8 *)_currentAddress;

                    if (GT(value, _checkValue.U8))
                    {
                        *result++ = {_currentAddress, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::GreaterOrEqual:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u8 value = *(u8 *)_currentAddress;

                    if (GE(value, _checkValue.U8))
                    {
                        *result++ = {_currentAddress, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::LesserThan:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u8 value = *(u8 *)_currentAddress;

                    if (LT(value, _checkValue.U8))
                    {
                        *result++ = {_currentAddress, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::LesserOrEqual:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u8 value = *(u8 *)_currentAddress;

                    if (LE(value, _checkValue.U8))
                    {
                        *result++ = {_currentAddress, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            default:
                break;
        }
    }

    void    Search32::FirstSearchSpecifiedU16(u32 endAddress, SearchFlags compare, Results32* result)
    {
        switch (compare)
        {
            case SearchFlags::Equal:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u16 value = *(u16 *)_currentAddress;

                    if (EQ(value, _checkValue.U16))
                    {
                        *result++ = {_currentAddress, value = value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::NotEqual:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u16 value = *(u16 *)_currentAddress;

                    if (NE(value, _checkValue.U16))
                    {
                        *result++ = {_currentAddress, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::GreaterThan:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u16 value = *(u16 *)_currentAddress;

                    if (GT(value, _checkValue.U16))
                    {
                        *result++ = {_currentAddress, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::GreaterOrEqual:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u16 value = *(u16 *)_currentAddress;

                    if (GE(value, _checkValue.U16))
                    {
                        *result++ = {_currentAddress, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::LesserThan:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u16 value = *(u16 *)_currentAddress;

                    if (LT(value, _checkValue.U16))
                    {
                        *result++ = {_currentAddress, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::LesserOrEqual:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u16 value = *(u16 *)_currentAddress;

                    if (LE(value, _checkValue.U16))
                    {
                        *result++ = {_currentAddress, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            default:
                break;
        }
    }

    void    Search32::FirstSearchSpecifiedU32(u32 endAddress, SearchFlags compare, Results32* result)
    {
        switch (compare)
        {
            case SearchFlags::Equal:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u32 value = *(u32 *)_currentAddress;

                    if (EQ(value, _checkValue.U32))
                    {
                        *result++ = {_currentAddress, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 4;
                }
                break;
            }
            case SearchFlags::NotEqual:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u32 value = *(u32 *)_currentAddress;

                    if (NE(value, _checkValue.U32))
                    {
                        *result++ = {_currentAddress, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 4;
                }
                break;
            }
            case SearchFlags::GreaterThan:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u32 value = *(u32 *)_currentAddress;

                    if (GT(value, _checkValue.U32))
                    {
                        *result++ = {_currentAddress, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 4;
                }
                break;
            }
            case SearchFlags::GreaterOrEqual:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u32 value = *(u32 *)_currentAddress;

                    if (GE(value, _checkValue.U32))
                    {
                        *result++ = {_currentAddress, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 4;
                }
                break;
            }
            case SearchFlags::LesserThan:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u32 value = *(u32 *)_currentAddress;

                    if (LT(value, _checkValue.U32))
                    {
                        *result++ = {_currentAddress, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 4;
                }
                break;
            }
            case SearchFlags::LesserOrEqual:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    u32 value = *(u32 *)_currentAddress;

                    if (LE(value, _checkValue.U32))
                    {
                        *result++ = {_currentAddress, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 4;
                }
                break;
            }
            default:
                break;
        }
    }

    // From https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
    bool AlmostEqualRelative(float A, float B, float maxRelDiff)
    {
        // Calculate the difference.
        float diff = std::fabs(A - B);
        A = std::fabs(A);
        B = std::fabs(B);
        // Find the largest
        float largest = (B > A) ? B : A;

        return diff <= maxRelDiff * largest;
    }

    void    Search32::FirstSearchSpecifiedFloat(u32 endAddress, SearchFlags compare, Results32* result)
    {
        switch (compare)
        {
            case SearchFlags::Equal:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    float value = *(float *)_currentAddress;

                    if (FP_EQ(value, _checkValue.Float))
                    {
                        result->address = _currentAddress;
                        result++->value.Float = value;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 4;
                }
                break;
            }
            case SearchFlags::NotEqual:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    float value = *(float *)_currentAddress;

                    if (FP_NE(value, _checkValue.Float))
                    {
                        result->address = _currentAddress;
                        result++->value.Float = value;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 4;
                }
                break;
            }
            case SearchFlags::GreaterThan:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    float value = *(float *)_currentAddress;

                    if (FP_GT(value, _checkValue.Float))
                    {
                        result->address = _currentAddress;
                        result++->value.Float = value;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 4;
                }
                break;
            }
            case SearchFlags::GreaterOrEqual:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    float value = *(float *)_currentAddress;

                    if (FP_GE(value, _checkValue.Float))
                    {
                        result->address = _currentAddress;
                        result++->value.Float = value;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 4;
                }
                break;
            }
            case SearchFlags::LesserThan:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    float value = *(float *)_currentAddress;

                    if (FP_LT(value, _checkValue.Float))
                    {
                        result->address = _currentAddress;
                        result++->value.Float = value;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 4;
                }
                break;
            }
            case SearchFlags::LesserOrEqual:
            {
                while (_currentAddress <= endAddress && _resultsInPool < _maxResults)
                {
                    float value = *(float *)_currentAddress;

                    if (FP_LE(value, _checkValue.Float))
                    {
                        result->address = _currentAddress;
                        result++->value.Float = value;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 4;
                }
                break;
            }
            default:
                break;
        }
    }
}
