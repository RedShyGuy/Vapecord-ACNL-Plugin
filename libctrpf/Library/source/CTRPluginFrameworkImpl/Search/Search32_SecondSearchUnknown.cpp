#include "CTRPluginFrameworkImpl/Search/Search32.hpp"
#include "memory.h"

namespace CTRPluginFramework
{
    void    Search32::SecondSearchUnknownU8(Storage<u32> &data, SearchFlags compare, Results32WithOld* result)
    {
        u32 address = _startRegion + _currentAddress * 4;

        switch (compare)
        {
            case SearchFlags::Equal:
            {
                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 4)
                        break;

                    Bytes32 oldValue = {v};
                    Bytes32 newValue = {*(u32 *)address};

                    if (EQ(newValue.b1, oldValue.b1))
                    {
                        *result++ = {address, newValue.b1, oldValue.b1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (EQ(newValue.b2, oldValue.b2))
                    {
                        *result++ = {address, newValue.b2, oldValue.b2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (EQ(newValue.b3, oldValue.b3))
                    {
                        *result++ = {address, newValue.b3, oldValue.b3};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (EQ(newValue.b4, oldValue.b4))
                    {
                        *result++ = {address, newValue.b4, oldValue.b4};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;
                }
                break;
            }
            case SearchFlags::NotEqual:
            {
                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 4)
                        break;

                    Bytes32 oldValue = {v};
                    Bytes32 newValue = {*(u32 *)address};

                    if (NE(newValue.b1, oldValue.b1))
                    {
                        *result++ = {address, newValue.b1, oldValue.b1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (NE(newValue.b2, oldValue.b2))
                    {
                        *result++ = {address, newValue.b2, oldValue.b2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (NE(newValue.b3, oldValue.b3))
                    {
                        *result++ = {address, newValue.b3, oldValue.b3};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (NE(newValue.b4, oldValue.b4))
                    {
                        *result++ = {address, newValue.b4, oldValue.b4};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;
                }
                break;
            }
            case SearchFlags::GreaterThan:
            {
                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 4)
                        break;

                    Bytes32 oldValue = {v};
                    Bytes32 newValue = {*(u32 *)address};

                    if (GT(newValue.b1, oldValue.b1))
                    {
                        *result++ = {address, newValue.b1, oldValue.b1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (GT(newValue.b2, oldValue.b2))
                    {
                        *result++ = {address, newValue.b2, oldValue.b2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (GT(newValue.b3, oldValue.b3))
                    {
                        *result++ = {address, newValue.b3, oldValue.b3};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (GT(newValue.b4, oldValue.b4))
                    {
                        *result++ = {address, newValue.b4, oldValue.b4};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;
                }
                break;
            }
            case SearchFlags::GreaterOrEqual:
            {
                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 4)
                        break;

                    Bytes32 oldValue = {v};
                    Bytes32 newValue = {*(u32 *)address};

                    if (GE(newValue.b1, oldValue.b1))
                    {
                        *result++ = {address, newValue.b1, oldValue.b1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (GE(newValue.b2, oldValue.b2))
                    {
                        *result++ = {address, newValue.b2, oldValue.b2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (GE(newValue.b3, oldValue.b3))
                    {
                        *result++ = {address, newValue.b3, oldValue.b3};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (GE(newValue.b4, oldValue.b4))
                    {
                        *result++ = {address, newValue.b4, oldValue.b4};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;
                }
                break;
            }
            case SearchFlags::LesserThan:
            {
                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 4)
                        break;

                    Bytes32 oldValue = {v};
                    Bytes32 newValue = {*(u32 *)address};

                    if (LT(newValue.b1, oldValue.b1))
                    {
                        *result++ = {address, newValue.b1, oldValue.b1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (LT(newValue.b2, oldValue.b2))
                    {
                        *result++ = {address, newValue.b2, oldValue.b2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (LT(newValue.b3, oldValue.b3))
                    {
                        *result++ = {address, newValue.b3, oldValue.b3};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (LT(newValue.b4, oldValue.b4))
                    {
                        *result++ = {address, newValue.b4, oldValue.b4};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;
                }
                break;
            }
            case SearchFlags::LesserOrEqual:
            {
                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 4)
                        break;

                    Bytes32 oldValue = {v};
                    Bytes32 newValue = {*(u32 *)address};

                    if (LE(newValue.b1, oldValue.b1))
                    {
                        *result++ = {address, newValue.b1, oldValue.b1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (LE(newValue.b2, oldValue.b2))
                    {
                        *result++ = {address, newValue.b2, oldValue.b2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (LE(newValue.b3, oldValue.b3))
                    {
                        *result++ = {address, newValue.b3, oldValue.b3};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (LE(newValue.b4, oldValue.b4))
                    {
                        *result++ = {address, newValue.b4, oldValue.b4};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;
                }
                break;
            }
            case SearchFlags::DifferentBy:
            {
                u8 difference = _checkValue.U8;

                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 4)
                        break;

                    Bytes32 oldValue = {v};
                    Bytes32 newValue = {*(u32 *)address};

                    if (DB(newValue.b1, oldValue.b1, difference))
                    {
                        *result++ = {address, newValue.b1, oldValue.b1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (DB(newValue.b2, oldValue.b2, difference))
                    {
                        *result++ = {address, newValue.b2, oldValue.b2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (DB(newValue.b3, oldValue.b3, difference))
                    {
                        *result++ = {address, newValue.b3, oldValue.b3};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (DB(newValue.b4, oldValue.b4, difference))
                    {
                        *result++ = {address, newValue.b4, oldValue.b4};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;
                }
                break;
            }
            case SearchFlags::DifferentByLess:
            {
                u8 difference = _checkValue.U8;

                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 4)
                        break;

                    Bytes32 oldValue = {v};
                    Bytes32 newValue = {*(u32 *)address};

                    if (DBL(newValue.b1, oldValue.b1, difference))
                    {
                        *result++ = {address, newValue.b1, oldValue.b1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (DBL(newValue.b2, oldValue.b2, difference))
                    {
                        *result++ = {address, newValue.b2, oldValue.b2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (DBL(newValue.b3, oldValue.b3, difference))
                    {
                        *result++ = {address, newValue.b3, oldValue.b3};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (DBL(newValue.b4, oldValue.b4, difference))
                    {
                        *result++ = {address, newValue.b4, oldValue.b4};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;
                }
                break;
            }
            case SearchFlags::DifferentByMore:
            {
                u8 difference = _checkValue.U8;

                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 4)
                        break;

                    Bytes32 oldValue = {v};
                    Bytes32 newValue = {*(u32 *)address};

                    if (DBM(newValue.b1, oldValue.b1, difference))
                    {
                        *result++ = {address, newValue.b1, oldValue.b1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (DBM(newValue.b2, oldValue.b2, difference))
                    {
                        *result++ = {address, newValue.b2, oldValue.b2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (DBM(newValue.b3, oldValue.b3, difference))
                    {
                        *result++ = {address, newValue.b3, oldValue.b3};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;

                    if (DBM(newValue.b4, oldValue.b4, difference))
                    {
                        *result++ = {address, newValue.b4, oldValue.b4};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address++;
                }
                break;
            }
            default:
                break;
        }

        _currentAddress = (address - _startRegion) / 4;
    }

    void    Search32::SecondSearchUnknownU16(Storage<u32> &data, SearchFlags compare, Results32WithOld* result)
    {
        u32 address = _startRegion + _currentAddress * 4;

        switch (compare)
        {
            case SearchFlags::Equal:
            {
                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 2)
                        break;

                    Short32 oldValue = {v};
                    Short32 newValue = {*(u32 *)address};

                    if (EQ(newValue.s1, oldValue.s1))
                    {
                        *result++ = {address, newValue.s1, oldValue.s1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;

                    if (EQ(newValue.s2, oldValue.s2))
                    {
                        *result++ = {address, newValue.s2, oldValue.s2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;
                }
                break;
            }
            case SearchFlags::NotEqual:
            {
                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 2)
                        break;

                    Short32 oldValue = {v};
                    Short32 newValue = {*(u32 *)address};

                    if (NE(newValue.s1, oldValue.s1))
                    {
                        *result++ = {address, newValue.s1, oldValue.s1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;

                    if (NE(newValue.s2, oldValue.s2))
                    {
                        *result++ = {address, newValue.s2, oldValue.s2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;
                }
                break;
            }
            case SearchFlags::GreaterThan:
            {
                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 2)
                        break;

                    Short32 oldValue = {v};
                    Short32 newValue = {*(u32 *)address};

                    if (GT(newValue.s1, oldValue.s1))
                    {
                        *result++ = {address, newValue.s1, oldValue.s1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;

                    if (GT(newValue.s2, oldValue.s2))
                    {
                        *result++ = {address, newValue.s2, oldValue.s2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;
                }
                break;
            }
            case SearchFlags::GreaterOrEqual:
            {
                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 2)
                        break;

                    Short32 oldValue = {v};
                    Short32 newValue = {*(u32 *)address};

                    if (GE(newValue.s1, oldValue.s1))
                    {
                        *result++ = {address, newValue.s1, oldValue.s1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;

                    if (GE(newValue.s2, oldValue.s2))
                    {
                        *result++ = {address, newValue.s2, oldValue.s2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;
                }
                break;
            }
            case SearchFlags::LesserThan:
            {
                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 2)
                        break;

                    Short32 oldValue = {v};
                    Short32 newValue = {*(u32 *)address};

                    if (LT(newValue.s1, oldValue.s1))
                    {
                        *result++ = {address, newValue.s1, oldValue.s1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;

                    if (LT(newValue.s2, oldValue.s2))
                    {
                        *result++ = {address, newValue.s2, oldValue.s2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;
                }
                break;
            }
            case SearchFlags::LesserOrEqual:
            {
                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 2)
                        break;

                    Short32 oldValue = {v};
                    Short32 newValue = {*(u32 *)address};

                    if (LE(newValue.s1, oldValue.s1))
                    {
                        *result++ = {address, newValue.s1, oldValue.s1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;

                    if (LE(newValue.s2, oldValue.s2))
                    {
                        *result++ = {address, newValue.s2, oldValue.s2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;
                }
                break;
            }
            case SearchFlags::DifferentBy:
            {
                u16 difference = _checkValue.U16;

                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 2)
                        break;

                    Short32 oldValue = {v};
                    Short32 newValue = {*(u32 *)address};

                    if (DB(newValue.s1, oldValue.s1, difference))
                    {
                        *result++ = {address, newValue.s1, oldValue.s1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;

                    if (DB(newValue.s2, oldValue.s2, difference))
                    {
                        *result++ = {address, newValue.s2, oldValue.s2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;
                }
                break;
            }
            case SearchFlags::DifferentByLess:
            {
                u16 difference = _checkValue.U16;

                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 2)
                        break;

                    Short32 oldValue = {v};
                    Short32 newValue = {*(u32 *)address};

                    if (DBL(newValue.s1, oldValue.s1, difference))
                    {
                        *result++ = {address, newValue.s1, oldValue.s1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;

                    if (DBL(newValue.s2, oldValue.s2, difference))
                    {
                        *result++ = {address, newValue.s2, oldValue.s2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;
                }
                break;
            }
            case SearchFlags::DifferentByMore:
            {
                u16 difference = _checkValue.U16;

                for (u32 v : data)
                {
                    if (_resultsInPool > _maxResults - 2)
                        break;

                    Short32 oldValue = {v};
                    Short32 newValue = {*(u32 *)address};

                    if (DBM(newValue.s1, oldValue.s1, difference))
                    {
                        *result++ = {address, newValue.s1, oldValue.s1};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;

                    if (DBM(newValue.s2, oldValue.s2, difference))
                    {
                        *result++ = {address, newValue.s2, oldValue.s2};
                        ResultsCount++;
                        _resultsInPool++;
                    }

                    address += 2;
                }
                break;
            }
            default:
                break;
        }

        _currentAddress = (address - _startRegion) / 4;
    }

    void    Search32::SecondSearchUnknownU32(Storage<u32> &data, SearchFlags compare, Results32WithOld* result)
    {
        u32 address = _startRegion + _currentAddress * 4;

        switch (compare)
        {
            case SearchFlags::Equal:
            {
                for (u32 value : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)address;

                    if (EQ(newValue, value))
                    {
                        *result++ = {address, newValue, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            case SearchFlags::NotEqual:
            {
                for (u32 value : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)address;

                    if (NE(newValue, value))
                    {
                        *result++ = {address, newValue, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            case SearchFlags::GreaterThan:
            {
                for (u32 value : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)address;

                    if (GT(newValue, value))
                    {
                        *result++ = {address, newValue, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            case SearchFlags::GreaterOrEqual:
            {
                for (u32 value : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)address;

                    if (GE(newValue, value))
                    {
                        *result++ = {address, newValue, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            case SearchFlags::LesserThan:
            {
                for (u32 value : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)address;

                    if (LT(newValue, value))
                    {
                        *result++ = {address, newValue, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            case SearchFlags::LesserOrEqual:
            {
                for (u32 value : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)address;

                    if (LE(newValue, value))
                    {
                        *result++ = {address, newValue, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            case SearchFlags::DifferentBy:
            {
                u32 difference = _checkValue.U32;

                for (u32 value : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)address;

                    if (DB(newValue, value, difference))
                    {
                        *result++ = {address, newValue, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            case SearchFlags::DifferentByLess:
            {
                u32 difference = _checkValue.U32;

                for (u32 value : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)address;

                    if (DBL(newValue, value, difference))
                    {
                        *result++ = {address, newValue, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            case SearchFlags::DifferentByMore:
            {
                u32 difference = _checkValue.U32;

                for (u32 value : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)address;

                    if (DBM(newValue, value, difference))
                    {
                        *result++ = {address, newValue, value};
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            default:
                break;
        }

        _currentAddress = (address - _startRegion) / 4;
    }

    void    Search32::SecondSearchUnknownFloat(Storage<u32> &data, SearchFlags compare, Results32WithOld* result)
    {
        u32 address = _startRegion + _currentAddress * 4;

        switch (compare)
        {
            case SearchFlags::Equal:
            {
                for (u32 v : data)
                {
                    float value;
                    memcpy(&value, &v, sizeof(float));

                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)address;

                    if (IsValid(value) && FP_EQ(newValue, value))
                    {
                        result->address = address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = value;
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            case SearchFlags::NotEqual:
            {
                for (u32 v : data)
                {
                    float value;
                    memcpy(&value, &v, sizeof(float));

                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)address;

                    if (IsValid(value) && FP_NE(newValue, value))
                    {
                        result->address = address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = value;
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            case SearchFlags::GreaterThan:
            {
                for (u32 v : data)
                {
                    float value;
                    memcpy(&value, &v, sizeof(float));

                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)address;

                    if (IsValid(value) && FP_GT(newValue, value))
                    {
                        result->address = address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = value;
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            case SearchFlags::GreaterOrEqual:
            {
                for (u32 v : data)
                {
                    float value;
                    memcpy(&value, &v, sizeof(float));

                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)address;

                    if (IsValid(value) && FP_GE(newValue, value))
                    {
                        result->address = address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = value;
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            case SearchFlags::LesserThan:
            {
                for (u32 v : data)
                {
                    float value;
                    memcpy(&value, &v, sizeof(float));

                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)address;

                    if (IsValid(value) && FP_LT(newValue, value))
                    {
                        result->address = address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = value;
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            case SearchFlags::LesserOrEqual:
            {
                for (u32 v : data)
                {
                    float value;
                    memcpy(&value, &v, sizeof(float));

                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)address;

                    if (IsValid(value) && FP_LE(newValue, value))
                    {
                        result->address = address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = value;
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            case SearchFlags::DifferentBy:
            {
                float difference = _checkValue.Float;

                for (u32 v : data)
                {
                    float value;
                    memcpy(&value, &v, sizeof(float));

                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)address;

                    if (IsValid(value) && FP_DB(newValue, value, difference))
                    {
                        result->address = address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = value;
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            case SearchFlags::DifferentByLess:
            {
                float difference = _checkValue.Float;

                for (u32 v : data)
                {
                    float value;
                    memcpy(&value, &v, sizeof(float));

                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)address;

                    if (IsValid(value) && FP_DBL(newValue, value, difference))
                    {
                        result->address = address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = value;
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            case SearchFlags::DifferentByMore:
            {
                float difference = _checkValue.Float;

                for (u32 v : data)
                {
                    float value;
                    memcpy(&value, &v, sizeof(float));

                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)address;

                    if (IsValid(value) && FP_DBM(newValue, value, difference))
                    {
                        result->address = address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = value;
                        _resultsInPool++;
                        ResultsCount++;
                    }

                    address += 4;
                }
                break;
            }
            default:
                break;
        }

        _currentAddress = (address - _startRegion) / 4;
    }
}
