#ifndef CTRPLUGINFRAMEWORKIMPL_GATEWAYRAMDUMPER_HPP
#define CTRPLUGINFRAMEWORKIMPL_GATEWAYRAMDUMPER_HPP

#include "types.h"
#include "CTRPluginFrameworkImpl/Search/Search.hpp"

namespace CTRPluginFramework
{
    class GatewayRAMDumper
    {
    public:
        GatewayRAMDumper(void);
        ~GatewayRAMDumper(void){}

        // Return true if finished
        bool operator()(void);
        bool _SelectRegion();
    private:

        void    _OpenFile(void);
        void    _WriteHeader(void);
        void    _DrawProgress(void);


        std::string             _fileName;
        File                    _file;
        u32                     _currentAddress;
        u32                     _endAddress;
        u32                     _regionIndex;
        u32                     _achievedSize;
        u32                     _totalSize;
        std::vector<Region>     _regions;
    };
}

#endif