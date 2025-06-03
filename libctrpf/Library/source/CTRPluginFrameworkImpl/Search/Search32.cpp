#include "CTRPluginFrameworkImpl/Search/Search32.hpp"

#include "CTRPluginFrameworkImpl/System/ProcessImpl.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"
#include "CTRPluginFramework/System/Process.hpp"

#include <algorithm>

namespace CTRPluginFramework
{
    extern void    *_pool;

    Search32::Search32(SearchParameters& parameters) :
    Search(parameters.previous)
    {
        u32 poolSize = 0x60000;

        // Check that System::Heap has enough space available
        if (Heap::SpaceFree() < 0xB0000) /// 0x60000 (for pool) + 0x50000 (for Storage)
        {
            // Unload bmp
            //Preferences::UnloadBackgrounds();
            // Recheck
            if (Heap::SpaceFree() < 0xB0000)
            {
                Error.pool = true;
                return;
            }
        }

        _pool = Heap::Alloc(0x60000);
        // Init parent's variables
        _checkValue.U32 = parameters.value32.U32;
        _flags = parameters.flags;
        _resultSize = IsFirstSearch(_flags) ? (IsUnknownSearch(_flags) ? sizeof(u32) : sizeof(Results32)) : sizeof(Results32WithOld);
        _maxResults = (poolSize / _resultSize) - 1;

        // Init header
        _header.flags = _flags;

        if (!parameters.fullMemory)
        {
            _header.nbRegions = 1;
            _header.regions[0].startAddress = parameters.ranges[0].startAddress;
            _header.regions[0].endAddress = parameters.ranges[0].endAddress;
            _totalRegionSize = _header.regions[0].endAddress - _header.regions[0].startAddress;

            _startRegion = parameters.ranges[0].startAddress;
            _endRegion = parameters.ranges[0].endAddress - 1;

            // Flush memory
            svcFlushProcessDataCache(ProcessImpl::ProcessHandle, _startRegion, _endRegion - _startRegion);
        }
        else
        {
            _header.nbRegions = 0;
            for (Region &region : parameters.ranges)
            {
                u32     size;
                u32     regionSize = region.endAddress - region.startAddress;

                if (Process::CheckRegion(region.startAddress, size, MEMPERM_READ))
                {
                    // If size mismatch
                    if (size < regionSize)
                    {
                        // Region is okay, add it to header
                        _header.regions[_header.nbRegions] = { region.startAddress, region.startAddress + size, 0, 0 };
                        _header.nbRegions++;

                        // Increment total size
                        _totalRegionSize += size;
                    }
                    else
                    {
                        // Region is okay, add it to header
                        _header.regions[_header.nbRegions] = { region.startAddress, region.endAddress, 0, 0 };
                        _header.nbRegions++;

                        // Increment total size
                        _totalRegionSize += regionSize;
                    }
                }
            }

            _startRegion = _header.regions[0].startAddress;
            _endRegion = _header.regions[0].endAddress - 1;

            // Flush memory
            svcFlushProcessDataCache(ProcessImpl::ProcessHandle, _startRegion, _endRegion - _startRegion);
        }

        // Set first region's file offset
        _header.regions[0].fileOffset = _file.Tell();

        // If it's not the first search, _totalRegionSearch must be total results from previousSearch
        if (!IsFirstSearch(_flags))
        {
            const Header &prevHeader = _previous->GetHeader();
            _endRegion = prevHeader.regions[0].nbResults;
            _totalRegionSize = _previous->GetTotalResults(_header);
        }
    }

    Search32::Search32(Search *previous, const std::string &filename) :
        Search(previous, filename)
    {
    }

    Search32::~Search32()
    {
        if (_pool)
        {
            Heap::Free(_pool);
            _pool = nullptr;
        }
    }

    void Search32::ReadResults(u32 index, StringVector& addr, StringVector& newVal, StringVector& oldVal)
    {
        addr.clear();
        newVal.clear();
        oldVal.clear();

        u32 nbItem = 500;

        if (index >= ResultsCount)
            return;

        if (nbItem + index >= ResultsCount)
            nbItem = ResultsCount - index;

        // First Search
        if (IsFirstSearch(_flags))
        {
            // Specified search
            if (IsSpecifiedSearch(_flags))
            {
                std::vector<Results32>  previousHits;

                // Resize container and create the items
                previousHits.resize(nbItem);

                // Extract previous hits
                ExtractPreviousHits(previousHits.data(), index, sizeof(Results32), nbItem, true);

                // Resize container in case nbItem is smaller
                previousHits.resize(nbItem);

                if (GetType() == SearchFlags::Float)
                {
                    for (Results32 &result : previousHits)
                    {
                        addr.push_back(Utils::ToHex(result.address));
                        newVal.push_back(Utils::ToString(result.value.Float, 4));
                    }
                }
                else
                {
                    for (Results32 &result : previousHits)
                    {
                        addr.push_back(Utils::ToHex(result.address));
                        newVal.push_back(Utils::ToHex(result.value.U32));
                    }
                }
                return;
            }
            // Unknown Search
            {
                addr.push_back("Start a new search to display the results");
                newVal.push_back("");
                return;
            }
        }
        // Subsidiary search
        {
            std::vector<Results32WithOld>  previousHits;

            // Resize container and create the items
            previousHits.resize(nbItem);

            // Extract previous hits
            ExtractPreviousHits(previousHits.data(), index, sizeof(Results32WithOld), nbItem, true);

            // Resize container in case nbItem is smaller
            previousHits.resize(nbItem);

            if (GetType() == SearchFlags::Float)
            {
                for (Results32WithOld &result : previousHits)
                {
                    addr.push_back(Utils::ToHex(result.address));
                    newVal.push_back(Utils::ToString(result.newValue.Float, 4));
                    oldVal.push_back(Utils::ToString(result.oldValue.Float, 4));
                }
            }
            else
            {
                for (Results32WithOld &result : previousHits)
                {
                    addr.push_back(Utils::ToHex(result.address));
                    newVal.push_back(Utils::ToHex(result.newValue.U32));
                    oldVal.push_back(Utils::ToHex(result.oldValue.U32));
                }
            }
        }
    }

    bool    Search32::FirstSearchSpecified(void)
    {
        if (_currentAddress < _startRegion)
            _currentAddress = _startRegion;

        u32         endAddress = std::min(_currentAddress + 0x40000, _endRegion);
        Results32   *result = reinterpret_cast<Results32 *>((u32)_pool + (sizeof(Results32) * _resultsInPool));
        SearchFlags compare = (SearchFlags)CompareFlags(_flags);
        SearchFlags type = (SearchFlags)TypeFlags(_flags);

        if (type == SearchFlags::U8) FirstSearchSpecifiedU8(endAddress, compare, result);
        else if (type == SearchFlags::U16) FirstSearchSpecifiedU16(endAddress, compare, result);
        else if (type == SearchFlags::U32) FirstSearchSpecifiedU32(endAddress, compare, result);
        else if (type == SearchFlags::Float) FirstSearchSpecifiedFloat(endAddress, compare, result);
        else return (true); ///< An error occured, skip region

        // Check if region is finished
        if (_currentAddress >= _endRegion)
            return (true);

        return (false);
    }

    bool    Search32::FirstSearchUnknown(void)
    {
        if (_currentAddress < _startRegion)
            _currentAddress = _startRegion;

        SearchFlags type = (SearchFlags)TypeFlags(_flags);
        u32         typeSize;

        if (type == SearchFlags::U8) typeSize = 1;
        else if (type == SearchFlags::U16) typeSize = 2;
        else if (type == SearchFlags::U32) typeSize = 4;
        else if (type == SearchFlags::Float) typeSize = 4;
        else return (true); ///< An error occured, skip region

        u32     *pool = (u32 *)_pool;

        pool += _resultsInPool;


        while (_currentAddress < _endRegion && _resultsInPool < _maxResults)
        {
            *pool++ = *(u32 *)_currentAddress;
            _resultsInPool++;
            _currentAddress += 4;
            ResultsCount += 4 / typeSize;
        }


        // Check if region is finished
        if (_currentAddress >= _endRegion)
            return (true);

        return (false);
    }

    bool    Search32::SecondSearchSpecified(void)
    {
        // If current region have no results
        if (_currentAddress >= _endRegion)
            return (true);

        u32     nbItem = std::min((u32)25000, _endRegion - _currentAddress);

        Storage<Results32>      previousHits(nbItem);
        Results32WithOld        *result = reinterpret_cast<Results32WithOld *>((u32)_pool + (sizeof(Results32WithOld) * _resultsInPool));

        // Extract previous hits
        _previous->ExtractPreviousHits(previousHits.data(), _currentAddress, sizeof(Results32), nbItem);

        // Resize container to real size
        previousHits.resize(nbItem);

        SearchFlags compare = (SearchFlags)CompareFlags(_flags);
        SearchFlags type = (SearchFlags)TypeFlags(_flags);

        // Do the comparison on previous hits
        if (type == SearchFlags::U8) SecondSearchSpecifiedU8(previousHits, compare, result);
        else if (type == SearchFlags::U16) SecondSearchSpecifiedU16(previousHits, compare, result);
        else if (type == SearchFlags::U32) SecondSearchSpecifiedU32(previousHits, compare, result);
        else if (type == SearchFlags::Float) SecondSearchSpecifiedFloat(previousHits, compare, result);
        else return (true); ///< An error occured, skip region

        // Check if region is finished
        if (_currentAddress >= _endRegion)
            return (true);

        return (false);
    }

    bool    Search32::SecondSearchUnknown(void)
    {
        // If current region have no results
        if (_currentAddress >= _endRegion)
            return (true);

        u32 nbItem = std::min((u32)0x10000, (u32)_endRegion - _currentAddress);

        Storage<u32>        previousHits(nbItem);
        Results32WithOld    *result = reinterpret_cast<Results32WithOld *>((u32)_pool + (sizeof(Results32WithOld) * _resultsInPool));

        // Extract previous hits
        _previous->ExtractPreviousHits(previousHits.data(), _currentAddress, sizeof(u32), nbItem);

        // Resize container to real size
        previousHits.resize(nbItem);

        SearchFlags compare = (SearchFlags)CompareFlags(_flags);
        SearchFlags type = (SearchFlags)TypeFlags(_flags);

        // Do the comparison on previous hits
        if (type == SearchFlags::U8) SecondSearchUnknownU8(previousHits, compare, result);
        else if (type == SearchFlags::U16) SecondSearchUnknownU16(previousHits, compare, result);
        else if (type == SearchFlags::U32) SecondSearchUnknownU32(previousHits, compare, result);
        else if (type == SearchFlags::Float) SecondSearchUnknownFloat(previousHits, compare, result);
        else return (true); ///< An error occured, skip region

        // Check if region is finished
        if (_currentAddress >= _endRegion)
            return (true);

        return (false);
    }

    bool    Search32::SubsidiarySearchSpecified(void)
    {
        // If current region have no results
        if (_currentAddress >= _endRegion)
            return (true);

        u32     nbItem = std::min((u32)25000, _endRegion - _currentAddress);

        Storage<Results32WithOld>   previousHits(nbItem);
        Results32WithOld            *result = reinterpret_cast<Results32WithOld *>((u32)_pool + (sizeof(Results32WithOld) * _resultsInPool));

        // Extract previous hits
        _previous->ExtractPreviousHits(previousHits.data(), _currentAddress, sizeof(Results32WithOld), nbItem);

        // Resize container to real size
        previousHits.resize(nbItem);

        SearchFlags compare = (SearchFlags)CompareFlags(_flags);
        SearchFlags type = (SearchFlags)TypeFlags(_flags);

        // Do the comparison on previous hits
        if (type == SearchFlags::U8) SubsidiarySearchSpecifiedU8(previousHits, compare, result);
        else if (type == SearchFlags::U16) SubsidiarySearchSpecifiedU16(previousHits, compare, result);
        else if (type == SearchFlags::U32) SubsidiarySearchSpecifiedU32(previousHits, compare, result);
        else if (type == SearchFlags::Float) SubsidiarySearchSpecifiedFloat(previousHits, compare, result);
        else return (true); ///< An error occured, skip region

        // Check if region is finished
        if (_currentAddress >= _endRegion)
            return (true);

        return (false);
    }

    bool    Search32::SubsidiarySearchUnknown(void)
    {
        // If current region have no results
        if (_currentAddress >= _endRegion)
            return (true);

        u32     nbItem = std::min((u32)25000, _endRegion - _currentAddress);

        Storage<Results32WithOld>   previousHits(nbItem);
        Results32WithOld            *result = reinterpret_cast<Results32WithOld *>((u32)_pool + (sizeof(Results32WithOld) * _resultsInPool));

        // Extract previous hits
        _previous->ExtractPreviousHits(previousHits.data(), _currentAddress, sizeof(Results32WithOld), nbItem);

        // Resize container to real size
        previousHits.resize(nbItem);

        SearchFlags compare = (SearchFlags)CompareFlags(_flags);
        SearchFlags type = (SearchFlags)TypeFlags(_flags);

        // Do the comparison on previous hits
        if (type == SearchFlags::U8) SubsidiarySearchUnknownU8(previousHits, compare, result);
        else if (type == SearchFlags::U16) SubsidiarySearchUnknownU16(previousHits, compare, result);
        else if (type == SearchFlags::U32) SubsidiarySearchUnknownU32(previousHits, compare, result);
        else if (type == SearchFlags::Float) SubsidiarySearchUnknownFloat(previousHits, compare, result);
        else return (true); ///< An error occured, skip region

        // Check if region is finished
        if (_currentAddress >= _endRegion)
            return (true);

        return (false);
    }
}
