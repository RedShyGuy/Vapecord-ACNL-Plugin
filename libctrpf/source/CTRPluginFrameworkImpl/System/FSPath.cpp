#include "CTRPluginFrameworkImpl/System/FSPath.hpp"
#include "CTRPluginFramework/System/Lock.hpp"

#include "3ds.h"

namespace CTRPluginFramework
{
    Mutex           FSPath::FSMutex;
    std::string     FSPath::CurrentWorkingDirectory;

    int     FSPath::SdmcFixPath(std::string &path)
    {
        uint32_t        units;
        uint32_t        code;
        std::string     fixPath;

        const uint8_t *p = (const uint8_t *)path.c_str();

        // Move the path pointer to the start of the actual path
        int     offset = 0;
        do
        {
            units = decode_utf8(&code, p);
            if (units < 0)
            {
                //r->_errno = EILSEQ;
                return (-1);
            }
            p += units;
            offset += units;
        } while (code != ':' && code != 0);

        // We found a colon; p points to the actual path
        if (code == ':')
            path = path.substr(offset);

        // Make sure there are no more colons and that the
        // remainder of the filename is valid UTF-8
        p = (const uint8_t*)path.c_str();
        do
        {
            units = decode_utf8(&code, p);
            if (units < 0)
            {
                //r->_errno = EILSEQ;
                return (-1);
            }

            if (code == ':')
            {
                //r->_errno = EINVAL;
                return (-1);
            }

            p += units;
        } while (code != 0);

        if (path[0] == '/')
            return (0);
        else
        {
            fixPath = FSPath::CurrentWorkingDirectory;
            fixPath += path;
            path = fixPath;
        }

        if (path.size() >= PATH_LENGTH_MAX)
        {
            //__fixedpath[PATH_LENGTH_MAX] = 0;
            //r->_errno = ENAMETOOLONG;
            return (-1);
        }
        return (0);
    }

    FSPath::FSPath(std::string path)
    {
        Lock    lock(FSMutex);

        if ((Error = SdmcFixPath(path)) != 0)
            return;

        Utf16Path.resize(256, 0);
        Units = utf8_to_utf16(Utf16Path.data(), (const uint8_t*)path.c_str(), PATH_LENGTH_MAX);
        if (Units <= 0)
            Error = -1;
    }
}
