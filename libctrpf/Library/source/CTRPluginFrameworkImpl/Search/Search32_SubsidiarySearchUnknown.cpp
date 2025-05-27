#include "CTRPluginFrameworkImpl/Search/Search32.hpp"

namespace CTRPluginFramework
{
    void    Search32::SubsidiarySearchUnknownU8(Storage<Results32WithOld>& data, SearchFlags compare, Results32WithOld* result)
    {
        //u8 checkValue = _checkValue.U8;

        // If current search is specified
        if (IsSpecifiedSearch(_flags))
        {
            switch (compare)
            {
            case SearchFlags::Equal:
            {
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

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
                u8 checkValue = _checkValue.U8;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

                    if (DB(newValue, oldValue, checkValue))
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
                u8 checkValue = _checkValue.U8;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

                    if (DBL(newValue, oldValue, checkValue))
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
                u8 checkValue = _checkValue.U8;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

                    if (DBM(newValue, oldValue, checkValue))
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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

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
                u8 checkValue = _checkValue.U8;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

                    if (DB(newValue, oldValue, checkValue))
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
                u8 checkValue = _checkValue.U8;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

                    if (DBL(newValue, oldValue, checkValue))
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
                u8 checkValue = _checkValue.U8;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

                    if (DBM(newValue, oldValue, checkValue))
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

    void    Search32::SubsidiarySearchUnknownU16(Storage<Results32WithOld>& data, SearchFlags compare, Results32WithOld* result)
    {
        u16 checkValue = _checkValue.U16;

        if (IsSpecifiedSearch(_flags))
        {
            switch (compare)
            {
            case SearchFlags::Equal:
            {
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

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
                u16 checkValue = _checkValue.U16;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

                    if (DB(newValue, oldValue, checkValue))
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
                u16 checkValue = _checkValue.U16;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

                    if (DBL(newValue, oldValue, checkValue))
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
                u16 checkValue = _checkValue.U16;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

                    if (DBM(newValue, oldValue, checkValue))
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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

                    if (DB(newValue, oldValue, checkValue))
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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

                    if (DBL(newValue, oldValue, checkValue))
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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u16 oldValue = res.newValue.U16;
                    u16 newValue = *(u16 *)res.address;

                    if (DBM(newValue, oldValue, checkValue))
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

    void    Search32::SubsidiarySearchUnknownU32(Storage<Results32WithOld>& data, SearchFlags compare, Results32WithOld* result)
    {
        //u32 checkValue = _checkValue.U32;

        // If current search is specified
        if (IsSpecifiedSearch(_flags))
        {
            switch (compare)
            {
            case SearchFlags::Equal:
            {
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

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
                u32 checkValue = _checkValue.U32;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

                    if (DB(newValue, oldValue, checkValue))
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
                u32 checkValue = _checkValue.U32;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

                    if (DBL(newValue, oldValue, checkValue))
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
                u32 checkValue = _checkValue.U32;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

                    if (DBM(newValue, oldValue, checkValue))
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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

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
                u32 checkValue = _checkValue.U32;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

                    if (DB(newValue, oldValue, checkValue))
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
                u32 checkValue = _checkValue.U32;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

                    if (DBL(newValue, oldValue, checkValue))
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
                u32 checkValue = _checkValue.U32;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

                    if (DBM(newValue, oldValue, checkValue))
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

    void    Search32::SubsidiarySearchUnknownFloat(Storage<Results32WithOld>& data, SearchFlags compare, Results32WithOld* result)
    {
        float checkValue = _checkValue.Float;

        // If current search is specified
        if (IsSpecifiedSearch(_flags))
        {
            switch (compare)
            {
            case SearchFlags::Equal:
            {
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

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
                float checkValue = _checkValue.Float;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

                    if (FP_DB(newValue, oldValue, checkValue))
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
                float checkValue = _checkValue.Float;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

                    if (FP_DBL(newValue, oldValue, checkValue))
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
                float checkValue = _checkValue.Float;
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

                    if (FP_DBM(newValue, oldValue, checkValue))
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
        {
        switch (compare)
        {
            case SearchFlags::Equal:
            {
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

                    if (FP_EQ(newValue, oldValue))
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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

                    if (FP_NE(newValue, oldValue))
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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

                    if (FP_GT(newValue, oldValue))
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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

                    if (FP_GE(newValue, oldValue))
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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

                    if (FP_LT(newValue, oldValue))
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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

                    if (FP_LE(newValue, oldValue))
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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

                    if (FP_DB(newValue, oldValue, checkValue))
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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

                    if (FP_DBL(newValue, oldValue, checkValue))
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
                for (const Results32WithOld& res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    float oldValue = res.newValue.Float;
                    float newValue = *(float *)res.address;

                    if (FP_DBM(newValue, oldValue, checkValue))
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
