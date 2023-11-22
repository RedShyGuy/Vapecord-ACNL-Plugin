#include "CTRPluginFramework/Utils/LineReader.hpp"
#include "CTRPluginFramework/System/File.hpp"
#include "CTRPluginFrameworkImpl/System/Heap.hpp"
#include <algorithm>

namespace CTRPluginFramework
{
    LineReader::LineReader(File &file) :
        _file(file),
        _offsetInBuffer(0),
        _dataInBuffer(0),
        _buffer(static_cast<char *>(Heap::Alloc(0x1000)))
    {

    }

    LineReader::~LineReader(void)
    {
        Heap::Free(_buffer);
    }

    bool    LineReader::operator()(std::string &line)
    {
        line.clear();

        u64     fileSize = _file.GetSize();

        do
        {
            u64     leftSize = std::max((s64)(fileSize - _file.Tell()), (s64)0);

            // EOF
            if (!leftSize && _offsetInBuffer >= _dataInBuffer)
                return (!line.empty());

            // If there's data in buffer
            if (_offsetInBuffer < _dataInBuffer)
            {
                while (_offsetInBuffer < _dataInBuffer)
                {
                    char c = _buffer[_offsetInBuffer++];

                    if (c == '\n')
                        return (true);

                    if (c && c != '\r')
                        line += c;
                }
                continue;
            }

            // Read file
            u32     size = (u32)(std::min((u64)0x1000, leftSize));

            if (_file.Read(_buffer, size)) goto error;

            _dataInBuffer = size;
            _offsetInBuffer = 0;

        } while (_dataInBuffer);

    error:
        _dataInBuffer = 0;
        _offsetInBuffer = 0;
        return (false);
    }

    StringLineReader::StringLineReader(const std::string &str) :
        _strin(str),
        _offset(0)
    {

    }

    bool    StringLineReader::operator()(std::string &line)
    {
        line.clear();

        const char* cStr = _strin.c_str();

        char c = cStr[_offset];
        bool ret = false;

        while (c)
        {

            if (c == '\n')
            {
                _offset++;
                return (true);
            }

            if (c == 0x1B)
            {
                line.append(cStr + _offset, 4);
                _offset += 4 - 1;
                ret = true;
            }
            else if (c == 0x11)
            {
                line.append(cStr + _offset, 3);
                _offset += 3 - 1;
                ret = true;
            }
            else if (c && c != '\r')
            {
                line += c;
                ret = true;
            }
            c = cStr[++_offset];
        }
        return (ret);
    }
}
