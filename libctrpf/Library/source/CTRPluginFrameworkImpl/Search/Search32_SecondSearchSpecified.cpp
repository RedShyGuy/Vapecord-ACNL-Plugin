#include "CTRPluginFrameworkImpl/Search/Search32.hpp"

namespace CTRPluginFramework
{
    void    Search32::SecondSearchSpecifiedU8(Storage<Results32>& data, SearchFlags compare, Results32WithOld* result)
    {
        u8  checkValue = _checkValue.U8;

        // If current search is specified
        if (IsSpecifiedSearch(_flags))
        {
            switch (compare)
            {
            case SearchFlags::Equal:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (EQ(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::NotEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (NE(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::GreaterThan:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (GT(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::GreaterOrEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (GE(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::LesserThan:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (LT(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::LesserOrEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (LE(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentBy:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (DB(newValue, oldValue, _checkValue.U8))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentByLess:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (DBL(newValue, oldValue, _checkValue.U8))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentByMore:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (DBM(newValue, oldValue, _checkValue.U8))
                    {
                        *result++ = { res.address, newValue, oldValue };
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
        // Else for unknown search
        else
        {
            switch (compare)
            {
            case SearchFlags::Equal:
            {
                for (Results32 &res : data)
                {
                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (EQ(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::NotEqual:
            {
                for (Results32 &res : data)
                {
                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (NE(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::GreaterThan:
            {
                for (Results32 &res : data)
                {
                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (GT(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::GreaterOrEqual:
            {
                for (Results32 &res : data)
                {
                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (GE(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::LesserThan:
            {
                for (Results32 &res : data)
                {
                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (LT(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::LesserOrEqual:
            {
                for (Results32 &res : data)
                {
                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (LE(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentBy:
            {
                for (Results32 &res : data)
                {
                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (DB(newValue, oldValue, _checkValue.U8))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentByLess:
            {
                for (Results32 &res : data)
                {
                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (DBL(newValue, oldValue, _checkValue.U8))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentByMore:
            {
                for (Results32 &res : data)
                {
                    u8 newValue = *(u8 *)res.address;
                    u8 oldValue = res.value.U8;

                    if (DBM(newValue, oldValue, _checkValue.U8))
                    {
                        *result++ = { res.address, newValue, oldValue };
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
    }

    void    Search32::SecondSearchSpecifiedU16(Storage<Results32>& data, SearchFlags compare, Results32WithOld* result)
    {
        u16 checkValue = _checkValue.U16;

        // If current search is specified
        if (IsSpecifiedSearch(_flags))
        {
            switch (compare)
            {
            case SearchFlags::Equal:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (EQ(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::NotEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (NE(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::GreaterThan:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (GT(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::GreaterOrEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (GE(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::LesserThan:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (LT(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::LesserOrEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (LE(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::DifferentBy:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (DB(newValue, oldValue, _checkValue.U16))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::DifferentByLess:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (DBL(newValue, oldValue, _checkValue.U16))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::DifferentByMore:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (DBM(newValue, oldValue, _checkValue.U16))
                    {
                        *result++ = { res.address, newValue, oldValue };
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

        // Else for unknown search
        else
        {
            switch (compare)
            {
            case SearchFlags::Equal:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (EQ(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::NotEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (NE(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::GreaterThan:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (GT(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::GreaterOrEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (GE(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::LesserThan:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (LT(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::LesserOrEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (LE(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::DifferentBy:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (DB(newValue, oldValue, _checkValue.U16))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::DifferentByLess:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (DBL(newValue, oldValue, _checkValue.U16))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress += 2;
                }
                break;
            }
            case SearchFlags::DifferentByMore:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 newValue = *(u16 *)res.address;
                    u16 oldValue = res.value.U16;

                    if (DBM(newValue, oldValue, _checkValue.U16))
                    {
                        *result++ = { res.address, newValue, oldValue };
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
    }

    void    Search32::SecondSearchSpecifiedU32(Storage<Results32>& data, SearchFlags compare, Results32WithOld* result)
    {
        u32 checkValue = _checkValue.U32;

        // If current search is specified
        if (IsSpecifiedSearch(_flags))
        {
            switch (compare)
            {
            case SearchFlags::Equal:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;

                    if (EQ(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::NotEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;

                    if (NE(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::GreaterThan:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;

                    if (GT(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::GreaterOrEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;

                    if (GE(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::LesserThan:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;

                    if (LT(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::LesserOrEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;

                    if (LE(newValue, checkValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentBy:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;

                    if (DB(newValue, oldValue, _checkValue.U32))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentByLess:
            {
                for (Results32 &res : data)
                {
                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;

                    if (DBL(newValue, oldValue, _checkValue.U32))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentByMore:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;

                    if (DBM(newValue, oldValue, _checkValue.U32))
                    {
                        *result++ = { res.address, newValue, oldValue };
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

        // Else for unknown search
        else
        {
            switch (compare)
            {
            case SearchFlags::Equal:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;


                    if (EQ(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::NotEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;

                    if (NE(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::GreaterThan:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;

                    if (GT(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::GreaterOrEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;

                    if (GE(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::LesserThan:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;

                    if (LT(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::LesserOrEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;

                    if (LE(newValue, oldValue))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentBy:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;

                    if (DB(newValue, oldValue, _checkValue.U32))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentByLess:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;

                    if (DBL(newValue, oldValue, _checkValue.U32))
                    {
                        *result++ = { res.address, newValue, oldValue };
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentByMore:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 newValue = *(u32 *)res.address;
                    u32 oldValue = res.value.U32;

                    if (DBM(newValue, oldValue, _checkValue.U32))
                    {
                        *result++ = { res.address, newValue, oldValue };
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
    }

    void    Search32::SecondSearchSpecifiedFloat(Storage<Results32>& data, SearchFlags compare, Results32WithOld* result)
    {
        float checkValue = _checkValue.Float;

        // If current search is specified
        if (IsSpecifiedSearch(_flags))
        {
            switch (compare)
            {
            case SearchFlags::Equal:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (FP_EQ(newValue, checkValue))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::NotEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (FP_NE(newValue, checkValue))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::GreaterThan:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (FP_GT(newValue, checkValue))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::GreaterOrEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (FP_GE(newValue, checkValue))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::LesserThan:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (FP_LT(newValue, checkValue))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::LesserOrEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (FP_LE(newValue, checkValue))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentBy:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (IsValid(oldValue) && FP_DB(newValue, oldValue, _checkValue.Float))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentByLess:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (IsValid(oldValue) && FP_DBL(newValue, oldValue, _checkValue.Float))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentByMore:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (IsValid(oldValue) && FP_DBM(newValue, oldValue, _checkValue.Float))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
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

        // Else for unknown search
        else
        {
            switch (compare)
            {
            case SearchFlags::Equal:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (IsValid(oldValue) && FP_EQ(newValue, oldValue))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::NotEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (IsValid(oldValue) && FP_NE(newValue, oldValue))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::GreaterThan:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (IsValid(oldValue) && FP_GT(newValue, oldValue))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::GreaterOrEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (IsValid(oldValue) && FP_GE(newValue, oldValue))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::LesserThan:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (IsValid(oldValue) && FP_LT(newValue, oldValue))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::LesserOrEqual:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (IsValid(oldValue) && FP_LE(newValue, oldValue))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentBy:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (IsValid(oldValue) && FP_DB(newValue, oldValue, _checkValue.Float))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentByLess:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (IsValid(oldValue) && FP_DBL(newValue, oldValue, _checkValue.Float))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
                        _resultsInPool++;
                        ResultsCount++;
                    }
                    _currentAddress++;
                }
                break;
            }
            case SearchFlags::DifferentByMore:
            {
                for (Results32 &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float newValue = *(float *)res.address;
                    float oldValue = res.value.Float;

                    if (IsValid(oldValue) && FP_DBM(newValue, oldValue, _checkValue.Float))
                    {
                        result->address = res.address;
                        result->newValue.Float = newValue;
                        result++->oldValue.Float = oldValue;
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
    }
}
