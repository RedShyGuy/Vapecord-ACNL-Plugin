#include "CTRPluginFramework/System/File.hpp"
#include "CTRPluginFramework/Utils/LineWriter.hpp"
#include "CTRPluginFrameworkImpl/System/Heap.hpp"

namespace CTRPluginFramework
{
    #define BUFFER_SIZE 0x1000

    LineWriter::LineWriter(File &output) :
        _output{ output }, _offsetInBuffer{ 0 }, _buffer{ static_cast<u8 *>(Heap::Alloc(BUFFER_SIZE)) }
    {
    }

    LineWriter::~LineWriter(void)
    {
        Flush();
        Heap::Free(_buffer);
    }

    LineWriter & LineWriter::operator<<(const std::string &input)
    {
        if (input.empty())
            return *this;

        u8  *dest = _buffer + _offsetInBuffer;

        for (const char c : input)
        {
            if (_offsetInBuffer >= BUFFER_SIZE)
            {
                Flush();
                dest = _buffer;
            }

            *dest++ = c;
            ++_offsetInBuffer;
        }

        return *this;
    }

    const std::string &LineWriter::endl(void)
    {
        static const std::string end = "\r\n";
        return end;
    }

    void    LineWriter::Flush(void)
    {
        if (!_offsetInBuffer || !_output.IsOpen())
            return;
        _output.Write(_buffer, _offsetInBuffer);
        _output.Flush();
        _offsetInBuffer = 0;
    }

    void    LineWriter::Close(void)
    {
        Flush();
        _output.Close();
    }
}
