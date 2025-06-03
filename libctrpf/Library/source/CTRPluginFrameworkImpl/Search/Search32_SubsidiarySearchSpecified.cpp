#include "CTRPluginFrameworkImpl/Search/Search32.hpp"

namespace CTRPluginFramework
{
    void    Search32::SubsidiarySearchSpecifiedU8(Storage<Results32WithOld> &data, SearchFlags compare, Results32WithOld *result)
    {
        u8 checkValue = _checkValue.U8;

        switch (compare)
        {
            case SearchFlags::Equal:
            {
                for (const Results32WithOld &res : data)
                {
                    if (_resultsInPool > _maxResults)
                        break;

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

                    if (EQ(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    if (NE(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    if (GT(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    if (GE(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    if (LT(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    if (LE(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

                    if (DB(newValue, oldValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

                    if (DBL(newValue, oldValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    u8 oldValue = res.newValue.U8;
                    u8 newValue = *(u8 *)res.address;

                    if (DBM(newValue, oldValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

    void Search32::SubsidiarySearchSpecifiedU16(Storage<Results32WithOld>& data, SearchFlags compare, Results32WithOld* result)
    {
        u16 checkValue = _checkValue.U16;

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

                    if (EQ(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    if (NE(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    if (GT(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    if (GE(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    if (LT(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    if (LE(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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
                        *result++ = {res.address, newValue, oldValue};
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
                        *result++ = {res.address, newValue, oldValue};
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
                        *result++ = {res.address, newValue, oldValue};
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

    void Search32::SubsidiarySearchSpecifiedU32(Storage<Results32WithOld>& data, SearchFlags compare, Results32WithOld* result)
    {
        u32 checkValue = _checkValue.U32;

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

                    if (EQ(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    if (NE(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    if (GT(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    if (GE(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    if (LT(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    if (LE(newValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

                    if (DB(newValue, oldValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

                    if (DBL(newValue, oldValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

                    u32 oldValue = res.newValue.U32;
                    u32 newValue = *(u32 *)res.address;

                    if (DBM(newValue, oldValue, checkValue))
                    {
                        *result++ = {res.address, newValue, oldValue};
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

    void Search32::SubsidiarySearchSpecifiedFloat(Storage<Results32WithOld>& data, SearchFlags compare, Results32WithOld* result)
    {
        float checkValue = _checkValue.Float;

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
