#ifndef CTRPLUGINFRAMEWORKIMPL_SEARCHBASE_HPP
#define CTRPLUGINFRAMEWORKIMPL_SEARCHBASE_HPP

#include "CTRPluginFrameworkImpl/Search/Search.hpp"

namespace CTRPluginFramework
{
    class Search
    {
        using StringVector = std::vector<std::string>;
        using u32Vector = std::vector<u32>;

    public:

        struct RegionIndex
        {
            u32 startIndex;
            u32 endIndex;
        };

        bool            IsInProgress{true};
        bool            Canceled{false};
        SearchError     Error;
        u32             ResultsCount;
        float           Progress;
        Time            SearchTime;
        u32             Step;

        // Cancel the current search
        virtual void    Cancel(void);

        // Execute the current search parameters
        // Return true if the search is finished
        bool    ExecuteSearch(void);

        // Read results
        virtual void    ReadResults(u32 index, StringVector &addr, StringVector &newVal, StringVector &oldVal) = 0;
        const Header    &GetHeader(void) const;
        u32             GetTotalResults(const Header &header) const;
        void            ExtractPreviousHits(void *data, u32 index, u32 structSize, u32 &nbItem, bool forced = false);

        // Return the type targeted by the search
        SearchFlags     GetType(void) const;
        bool            IsFirstUnknownSearch(void) const;
        virtual ~Search() {};

    protected:

        Search(Search *previous);
        Search(Search *previous, const std::string &filename);

        // Write the header to the file
        void    WriteHeader(void);

        // Write results to file
        void    WriteResults(void);

        // Update progress
        virtual void    UpdateProgress(void);

        // Check next region
        // Return true if no more region available
        virtual bool    CheckNextRegion(void);


        virtual bool    FirstSearchSpecified(void) = 0;
        virtual bool    FirstSearchUnknown(void) = 0;
        virtual bool    SecondSearchSpecified(void) = 0;
        virtual bool    SecondSearchUnknown(void) = 0;
        virtual bool    SubsidiarySearchSpecified(void) = 0;
        virtual bool    SubsidiarySearchUnknown(void) = 0;

        void    CreateIndexTable(void);
        u32     GetRegionIndex(u32 index);

        u32     _indexRegion;
        u32     _startRegion;
        u32     _endRegion;
        u32     _achievedRegionSize;
        u32     _totalRegionSize;
        u32     _currentAddress;
        u32     _maxResults; ///< Initialized in child class
        u32     _resultsInPool;
        u32     _resultSize; ///< Initialized in child class
        u32     _flags; ///< Initialized in child class
        Clock   clock;
        File    _file;
        Header  _header; ///< Initialized in child class
        Search  *_previous;
       // void    *_pool; ///< Initialized in child class

        std::vector<RegionIndex> _indexTable;
    };
}

#endif
