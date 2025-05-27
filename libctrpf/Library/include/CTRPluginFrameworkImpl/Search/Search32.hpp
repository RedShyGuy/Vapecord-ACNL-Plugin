#ifndef CTRPLUGINFRAMEWORKIMPL_SEARCH32_HPP
#define CTRPLUGINFRAMEWORKIMPL_SEARCH32_HPP

#include "CTRPluginFrameworkImpl/Search/SearchBase.hpp"
#include "CTRPluginFrameworkImpl/System/IAllocator.hpp"

namespace CTRPluginFramework
{
    class Search32 : public Search
    {
        using StringVector = std::vector<std::string>;
        using u32Vector = std::vector<u32>;
//#define Storage std::vector<
template <typename T>
using Storage = std::vector<T, IAllocator<T>>;

    public:

        Search32(SearchParameters &parameters);
        Search32(Search *previous, const std::string &filename);
        ~Search32();

        // Read results
        void    ReadResults(u32 index, StringVector &addr, StringVector &newVal, StringVector &oldVal) override;

    private:

        Types32_u   _checkValue;

        // Return true if region is finished
        bool    FirstSearchSpecified(void) override;
        bool    FirstSearchUnknown(void) override;
        bool    SecondSearchSpecified(void) override;
        bool    SecondSearchUnknown(void) override;
        bool    SubsidiarySearchSpecified(void) override;
        bool    SubsidiarySearchUnknown(void) override;

        void    FirstSearchSpecifiedU8(u32 endAddress, SearchFlags compare, Results32 *result);
        void    FirstSearchSpecifiedU16(u32 endAddress, SearchFlags compare, Results32 *result);
        void    FirstSearchSpecifiedU32(u32 endAddress, SearchFlags compare, Results32 *result);
        void    FirstSearchSpecifiedFloat(u32 endAddress, SearchFlags compare, Results32 *result);

        void    SecondSearchSpecifiedU8(Storage<Results32> &data, SearchFlags compare, Results32WithOld *result);
        void    SecondSearchSpecifiedU16(Storage<Results32> &data, SearchFlags compare, Results32WithOld *result);
        void    SecondSearchSpecifiedU32(Storage<Results32> &data, SearchFlags compare, Results32WithOld *result);
        void    SecondSearchSpecifiedFloat(Storage<Results32> &data, SearchFlags compare, Results32WithOld *result);

        void    SecondSearchUnknownU8(Storage<u32> &data, SearchFlags compare, Results32WithOld *result);
        void    SecondSearchUnknownU16(Storage<u32> &data, SearchFlags compare, Results32WithOld *result);
        void    SecondSearchUnknownU32(Storage<u32> &data, SearchFlags compare, Results32WithOld *result);
        void    SecondSearchUnknownFloat(Storage<u32> &data, SearchFlags compare, Results32WithOld *result);

        void    SubsidiarySearchSpecifiedU8(Storage<Results32WithOld> &data, SearchFlags compare, Results32WithOld* result);
        void    SubsidiarySearchSpecifiedU16(Storage<Results32WithOld> &data, SearchFlags compare, Results32WithOld* result);
        void    SubsidiarySearchSpecifiedU32(Storage<Results32WithOld> &data, SearchFlags compare, Results32WithOld* result);
        void    SubsidiarySearchSpecifiedFloat(Storage<Results32WithOld> &data, SearchFlags compare, Results32WithOld* result);

        void    SubsidiarySearchUnknownU8(Storage<Results32WithOld> &data, SearchFlags compare, Results32WithOld* result);
        void    SubsidiarySearchUnknownU16(Storage<Results32WithOld> &data, SearchFlags compare, Results32WithOld* result);
        void    SubsidiarySearchUnknownU32(Storage<Results32WithOld> &data, SearchFlags compare, Results32WithOld* result);
        void    SubsidiarySearchUnknownFloat(Storage<Results32WithOld> &data, SearchFlags compare, Results32WithOld* result);
    };
}

#endif