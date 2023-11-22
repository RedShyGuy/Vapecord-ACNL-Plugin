#include "CTRPluginFrameworkImpl/Graphics/BMPImage.hpp"

#include "CTRPluginFramework/Graphics/Color.hpp"
#include "CTRPluginFrameworkImpl/Graphics/PrivColor.hpp"
#include "CTRPluginFramework/Graphics/OSD.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Renderer.hpp"
#include "CTRPluginFrameworkImpl/System/Screen.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"
#include "CTRPluginFrameworkImpl/System/IAllocator.hpp"

namespace CTRPluginFramework
{
    void    BMPImage::BitmapFileHeader::Clear(void)
    {
        std::fill((u8 *)this, (u8 *)this + sizeof(BitmapFileHeader), 0);
    }

    bool    BMPImage::BitmapFileHeader::Read(File &file)
    {
        return file.Read(this, sizeof(BitmapFileHeader)) != File::OPResult::SUCCESS;
    }

    void    BMPImage::BitmapInformationHeader::Clear(void)
    {
        std::fill((u8 *)this, (u8 *)this + sizeof(BitmapInformationHeader), 0);
    }

    bool    BMPImage::BitmapInformationHeader::Read(File &file)
    {
        return file.Read(this, sizeof(BitmapInformationHeader)) != File::OPResult::SUCCESS;
    }

    const u32     BMPImage::HeaderSize = sizeof(BMPImage::BitmapFileHeader) + sizeof(BMPImage::BitmapInformationHeader);

    // Default CTOR
    BMPImage::BMPImage() = default;

    BMPImage::BMPImage(void *data)
    {
        // Load the file
        _LoadBitmapFromMemory(data);
        RGBtoBGR();
    }

    BMPImage::BMPImage(const std::string &filename) :
        _filename{ filename }
    {
        // Load the file
        _LoadBitmap();
        // Reverse channels
        RGBtoBGR();
    }

    BMPImage::BMPImage(const u32 width, const u32 height) :
        _width{ static_cast<u16>(width) }, _height{ static_cast<u16>(height) }
    {
        _rowIncrement = _width * _bytesPerPixel;
        _dataSize = height * _rowIncrement;

        // Create buffer
        _data = static_cast<u8 *>(::operator new(_dataSize + HeaderSize, std::nothrow));
        if (_data == nullptr)
            return;

        DataClear();
        _loaded = true;
    }

    BMPImage::BMPImage(const BMPImage &src, const u32 width, const u32 height)
    {
        _CreateBitmap();
        FillWithImg(src);
    }

    BMPImage::~BMPImage(void)
    {
        if (_data != nullptr)
        {
            ::operator delete(_data, std::nothrow);
            _data = nullptr;
        }
    }

    bool    BMPImage::IsLoaded(void) const
    {
        return _loaded;
    }

    u32     BMPImage::Width(void) const
    {
        return _width;
    }

    u32     BMPImage::Height(void) const
    {
        return _height;
    }

    u32     BMPImage::BytesPerPixel(void) const
    {
        return _bytesPerPixel;
    }

    const IntVector & BMPImage::GetDimensions(void) const
    {
        return _dimensions;
    }

    u8  *BMPImage::data(void) const
    {
        if (_data == nullptr)
            return nullptr;

        return _data + HeaderSize;
    }

    u8  *BMPImage::end(void) const
    {
        if (_data == nullptr)
            return nullptr;
        return _data + _dataSize + HeaderSize;
    }

    u8  *BMPImage::Row(u32 rowIndex) const
    {
        return &(_data + HeaderSize)[(rowIndex * _rowIncrement)];
    }

    void    BMPImage::DataClear(void)
    {
        std::fill(_data + HeaderSize, _data + HeaderSize + _dataSize, 0);
    }

    void    BMPImage::SetWidthHeight(const u32 width, const u32 height)
    {
        if (width != _width || _height != height)
        {
            _width = width;
            _height = height;

            _CreateBitmap();
        }
    }

    void    BMPImage::Unload(void)
    {
        if (_data)
            ::operator delete(_data, std::nothrow);
        _data = nullptr;
        _dataSize = 0;
        _loaded = false;
    }

    void     BMPImage::Draw(int x, int y)
    {
        OptimiseForRendering();

        ScreenImpl *scr = Renderer::GetContext()->screen;

        int posX = scr->IsTopScreen() ? x + (340 - _width) / 2 : x + (280 - _width) / 2;
        int posY = y + (200 - _height) / 2;


        u16      *img = reinterpret_cast<u16 *>(data());
        u16      *left = reinterpret_cast<u16 *>(scr->GetLeftFrameBuffer(posX, posY));
        u32      rowsize = _height;
        u32      screenheight = 240; //scr->IsTopScreen() ? 400 : 320;

        left -= _height;
        for (u32 w = _width; w > 0; --w)
        {
            std::copy(img, img + rowsize, left);
            left += screenheight;
            img += rowsize;
        }
    }

    void    BMPImage::Draw(const IntVector &point)
    {
        Draw(point.x, point.y);
    }

    void     BMPImage::Draw(const IntRect &area, float fade UNUSED)
    {
        // TODO: re-implement fading ?
        OptimiseForRendering();

        ScreenImpl *scr = Renderer::GetContext()->screen;

        int posX = area.leftTop.x;
        int posY = area.leftTop.y;
        int width = area.size.x;
        int height = area.size.y;

        int xOffset = 0;
        int startY = 0;

        if (_width < width)
        {
          posX += (width - _width) / 2;
          width = _width;
        }
        else if (_width > width)
          xOffset = ((_width - area.size.x) / 2) * 3;

        if (_height < area.size.y)
        {
          posY += (height - _height) / 2;
          height = _height;
        }
        else if (_height > height)
          startY = (_height - height) / 2;


        if (xOffset < 0)
          xOffset = 0;

        u16      *img = reinterpret_cast<u16 *>(data());
        u16      *left = reinterpret_cast<u16 *>(scr->GetLeftFrameBuffer(posX, posY));
        u32      rowsize = height;

        left -= height;
        img += _height * xOffset + startY;
        for (u32 w = width; w > 0; --w)
        {
            std::copy(img, img + rowsize, left);
            left += 240;
            img += _height;
        }
    }

    void    BMPImage::SaveImage(const std::string &fileName) const
    {
        File file(fileName, File::RWC | File::TRUNCATE);

        if (!file.IsOpen())
        {
            OSD::Notify("BMP Error: couldn't open the file");
            return;
        }

        BitmapFileHeader *          bfh = (BitmapFileHeader *)_data;
        BitmapInformationHeader *   bih = (BitmapInformationHeader *)(_data + sizeof(BitmapFileHeader));

        bih->width            = _width;
        bih->height           = _height;
        bih->bit_count        = static_cast<unsigned short>(_bytesPerPixel << 3);
        bih->clr_important    = 0;
        bih->clr_used         = 0;
        bih->compression      = 0;
        bih->planes           = 1;
        bih->size             = sizeof(BitmapInformationHeader);
        bih->x_pels_per_meter = 0;
        bih->y_pels_per_meter = 0;
        bih->size_image       = (((bih->width * _bytesPerPixel) + 3) & 0x0000FFFC) * bih->height;

        bfh->type             = 19778;
        bfh->size             = sizeof(BitmapFileHeader) + sizeof(BitmapInformationHeader) + bih->size_image;
        bfh->reserved1        = 0;
        bfh->reserved2        = 0;
        bfh->off_bits         = sizeof(BitmapInformationHeader) + sizeof(BitmapFileHeader);

        file.Write(_data, _dataSize + HeaderSize);
    }

    void    BMPImage::BGRtoRGB(void)
    {
        if ((BGR_Mode == _channelMode) && (3 == _bytesPerPixel))
        {
            ReverseChannels();
            _channelMode = RGB_Mode;
        }
    }

    void    BMPImage::RGBtoBGR(void)
    {
        if ((RGB_Mode == _channelMode) && (3 == _bytesPerPixel))
        {
            ReverseChannels();
            _channelMode = BGR_Mode;
        }
    }

    void    BMPImage::OptimiseForRendering(void)
    {
        // If already converted, nothing to do
        if (_channelMode == RGB565_Mode)
            return;

        // Ensure we're dealing with BGR8 color format
        RGBtoBGR();

        Pixel   *img = reinterpret_cast<Pixel *>(data());
        u16     *rgb565Image = static_cast<u16 *>(::operator new(_width * _height * 2 + HeaderSize, std::nothrow));

        u16     *dst = reinterpret_cast<u16 *>(reinterpret_cast<u32>(rgb565Image) + HeaderSize);

        if (rgb565Image == nullptr)
        {
            OSD::Notify("Error: rgb565 buffer alloc failed", Color::Red);
            Unload();
            return;
        }

        dst += _height;

        for (u32 w = _width; w > 0; --w)
        {
            Pixel *row = img;
            for (u32 h = _height; h > 0; --h)
            {
                union
                {
                    u16     u{0};
                    u8      b[2];
                }           c;

                c.u = (row->b & 0xF8) >> 3;
                c.u |= (row->g & 0xFC) << 3;
                c.u |= (row->r & 0xF8) << 8;

                *dst-- = c.u;
                row += _width;
            }
            dst += _height << 1;
            ++img;
        }

        ::operator delete(_data);
        _data = reinterpret_cast<u8 *>(rgb565Image);
        _channelMode = RGB565_Mode;
    }

    void    BMPImage::ReverseChannels(void)
    {
        if (3 != _bytesPerPixel)
            return;

        for (unsigned char *itr = data(); itr < end(); itr += _bytesPerPixel)
        {
            std::swap(*(itr + 0), *(itr + 2));
        }
    }

    void    BMPImage::FillWithImg(const BMPImage &src)
    {
        if (!src._loaded)
          return;

        _loaded = true;

        DataClear();

        int   startY = 0;
        int   offsetX = 0;

        int   srcWidth = src._width;
        int   srcHeight = src._height;

        if (srcWidth < _width)
          offsetX = (_width - srcWidth) / 2 * 3;
        if (src._height < _height)
          startY = (_height - srcHeight) / 2;

        for (int y = 0; y < srcHeight; y++)
        {
            unsigned char  *imgSrc = src.Row(y);
            unsigned char  *imgDst = Row(y + startY) + offsetX;
            for (int x = 0; x < srcWidth; x++)
            {
               *imgDst++ = *imgSrc++;
               *imgDst++ = *imgSrc++;
               *imgDst++ = *imgSrc++;
            }
        }
    }

    bool    BMPImage::Resample(BMPImage &dest, int newWidth, int newHeight)
    {
        //if(_data == NULL) return false;
        //
        // Get a new buuffer to interpolate into
        if (!_loaded)
            return (false);

        dest.SetWidthHeight(newWidth, newHeight);

        if (!dest._loaded)
            return (false);

        unsigned char *newData = dest.data(); //new unsigned char [newWidth * newHeight * 3];

        double scaleWidth = (double)newWidth / (double)_width;
        double scaleHeight = (double)newHeight / (double)_height;

        for (int cy = 0; cy < newHeight; cy++)
        {
            for (int cx = 0; cx < newWidth; cx++)
            {
                int pixel = (cy * (newWidth * 3)) + (cx * 3);
                int nearestMatch = (((int)(cy / scaleHeight) * (_width * 3)) + ((int)(cx / scaleWidth) * 3));

                newData[pixel] = _data[nearestMatch];
                newData[pixel + 1] = _data[nearestMatch + 1];
                newData[pixel + 2] = _data[nearestMatch + 2];
            }
        }

        return true;
    }

    void    BMPImage::SubSample(BMPImage &dest)
    {
        /*
           Half sub-sample of original image.
        */
        if (!_loaded)
            return;

        dest._loaded = true;
        unsigned int w = 0;
        unsigned int h = 0;

        bool odd_width = false;
        bool odd_height = false;

        if (0 == (_width % 2))
            w = _width / 2;
        else
        {
            w = 1 + (_width / 2);
            odd_width = true;
        }

        if (0 == (_height % 2))
            h = _height / 2;
        else
        {
            h = 1 + (_height / 2);
            odd_height = true;
        }

        unsigned int horizontal_upper = (odd_width) ? (w - 1) : w;
        unsigned int vertical_upper = (odd_height) ? (h - 1) : h;

        dest.SetWidthHeight(w, h);

        unsigned char *s_itr[3]{};
        const unsigned char *itr1[3]{};
        const unsigned char *itr2[3]{};

        s_itr[0] = dest.data() + 0;
        s_itr[1] = dest.data() + 1;
        s_itr[2] = dest.data() + 2;

        itr1[0] = data() + 0;
        itr1[1] = data() + 1;
        itr1[2] = data() + 2;

        itr2[0] = data() + _rowIncrement + 0;
        itr2[1] = data() + _rowIncrement + 1;
        itr2[2] = data() + _rowIncrement + 2;

        unsigned int total = 0;

        for (unsigned int j = 0; j < vertical_upper; ++j)
        {
            for (unsigned int i = 0; i < horizontal_upper; ++i)
            {
                for (unsigned int k = 0; k < _bytesPerPixel; s_itr[k] += _bytesPerPixel, ++k)
                {
                    total = 0;
                    total += *(itr1[k]);
                    total += *(itr1[k]);
                    total += *(itr2[k]);
                    total += *(itr2[k]);

                    itr1[k] += _bytesPerPixel;
                    itr1[k] += _bytesPerPixel;
                    itr2[k] += _bytesPerPixel;
                    itr2[k] += _bytesPerPixel;

                    *(s_itr[k]) = static_cast<unsigned char>(total >> 2);
                }
            }

            if (odd_width)
            {
                for (unsigned int k = 0; k < _bytesPerPixel; s_itr[k] += _bytesPerPixel, ++k)
                {
                    total = 0;
                    total += *(itr1[k]);
                    total += *(itr2[k]);

                    itr1[k] += _bytesPerPixel;
                    itr2[k] += _bytesPerPixel;

                    *(s_itr[k]) = static_cast<unsigned char>(total >> 1);
                }
            }

            for (unsigned int k = 0; k < _bytesPerPixel; ++k)
            {
                itr1[k] += _rowIncrement;
            }

            if (j != (vertical_upper - 1))
            {
                for (unsigned int k = 0; k < _bytesPerPixel; ++k)
                {
                    itr2[k] += _rowIncrement;
                }
            }
        }

        if (odd_height)
        {
            for (unsigned int i = 0; i < horizontal_upper; ++i)
            {
                for (unsigned int k = 0; k < _bytesPerPixel; s_itr[k] += _bytesPerPixel, ++k)
                {
                    total = 0;
                    total += *(itr1[k]);
                    total += *(itr2[k]);

                    itr1[k] += _bytesPerPixel;
                    itr2[k] += _bytesPerPixel;

                    *(s_itr[k]) = static_cast<unsigned char>(total >> 1);
                }
            }

            if (odd_width)
            {
                for (unsigned int k = 0; k < _bytesPerPixel; ++k)
                {
                    (*(s_itr[k])) = *(itr1[k]);
                }
            }
        }
    }

    void    BMPImage::UpSample(BMPImage &dest)
    {
        /*
           2x up-sample of original image.
        */

        dest.SetWidthHeight(2 * _width, 2 * _height);

        const unsigned char *s_itr[3];
        unsigned char *itr1[3];
        unsigned char *itr2[3];

        s_itr[0] = data() + 0;
        s_itr[1] = data() + 1;
        s_itr[2] = data() + 2;

        itr1[0] = dest.data() + 0;
        itr1[1] = dest.data() + 1;
        itr1[2] = dest.data() + 2;

        itr2[0] = dest.data() + dest._rowIncrement + 0;
        itr2[1] = dest.data() + dest._rowIncrement + 1;
        itr2[2] = dest.data() + dest._rowIncrement + 2;

        for (unsigned int j = 0; j < _height; ++j)
        {
            for (unsigned int i = 0; i < _width; ++i)
            {
                for (unsigned int k = 0; k < _bytesPerPixel; s_itr[k] += _bytesPerPixel, ++k)
                {
                    *(itr1[k]) = *(s_itr[k]);
                    itr1[k] += _bytesPerPixel;
                    *(itr1[k]) = *(s_itr[k]);
                    itr1[k] += _bytesPerPixel;

                    *(itr2[k]) = *(s_itr[k]);
                    itr2[k] += _bytesPerPixel;
                    *(itr2[k]) = *(s_itr[k]);
                    itr2[k] += _bytesPerPixel;
                }
            }

            for (unsigned int k = 0; k < _bytesPerPixel; ++k)
            {
                itr1[k] += dest._rowIncrement;
                itr2[k] += dest._rowIncrement;
            }
        }
    }

    bool    BMPImage::Region(const u32 &x, const u32 &y, const u32 &width, const u32 &height, BMPImage &destImage)
    {
        if ((x + width) > _width)
            return (false);
        if ((y + height) > _height)
            return (false);

        if ((destImage._width < _width) || (destImage._height < _height))
            destImage.SetWidthHeight(width, height);

        for (u32 r = 0; r < height; ++r)
        {
            u8 *itr1 = Row(r + y) + x * _bytesPerPixel;
            u8 *itr1_end = itr1 + (width * _bytesPerPixel);
            u8 *itr2 = destImage.Row(r);

            std::copy(itr1, itr1_end, itr2);
        }
        destImage._loaded = true;
        return (true);
    }

    bool    BMPImage::RoiFromCenter(const u32 cx, const u32 cy, const u32 &width, const u32 &height, BMPImage &destImage)
    {
        return (Region(cx - (width / 2), cy - (height / 2), width, height, destImage));
    }

    int     BMPImage::_CreateBitmap(void)
    {
        // Free current buffer if necessary
        Unload();

        _rowIncrement = _width * _bytesPerPixel;
        _dataSize = _height * _rowIncrement;

        // Try to alloc in vram first
        _data = static_cast<u8 *>(::operator new(_dataSize + HeaderSize, std::nothrow));

        // If allocation failed
        if (_data == nullptr)
        {
            _dataSize = 0;
            _loaded = false;
            return (-1);
        }

        // Fill with black
        DataClear();
        _loaded = true;
        return (0);
    }

    void     BMPImage::_LoadBitmap(void)
    {
        Unload();
        _width  = 0;
        _height = 0;

        File                    file(_filename, File::READ);
        BitmapFileHeader        bfh = {};
        BitmapInformationHeader bih = {};

        if (!file.IsOpen())
            return;

        if (file.GetSize() >= 0x50000)
        {
            OSD::Notify("BMP Error: The file is too big.", Color::Red);
            return;
        }

        if (bfh.Read(file))
        {
            OSD::Notify("BMP Error: Error while reading BFH", Color::Red);
            return;
        }

        if (bih.Read(file))
        {
            OSD::Notify("BMP Error: Error while reading BIH", Color::Red);
            return;
        }

        if (bfh.type != 19778)
        {
            OSD::Notify(Utils::Format("BMP Error: Unexpected type value: %d", bfh.type), Color::Red);
            return;
        }

        if (bih.bit_count != 24)
        {
            OSD::Notify(Utils::Format("BMP Error: Unexpected bit depth: %d", bih.bit_count), Color::Red);
            return;
        }

        _width  = bih.width;
        _height = bih.height;

        // Temporary limit ?
        if (_width > 340 || _height > 200)
        {
            OSD::Notify("BMP Error: file should not be higher than 340px * 200px", Color::Red);
            return;
        }

        _dimensions = IntVector(_width, _height);
        _bytesPerPixel = bih.bit_count >> 3;

        // Go to the data position in file
        file.Seek(bfh.off_bits, File::SET);

        // Try to alloc the buffer
        if (_CreateBitmap())
        {
            OSD::Notify("BMP Error: Error while allocating required space.", Color::Red);
            return;
        }

        u32   padding = (4 - ((3 * _width) % 4)) % 4;
        int   rows = _height / 5;
        int   rowWidth = _rowIncrement + padding;
        int   totalwidth = rows * rowWidth;
        int   oddRows = rows * 5;

        // Temporary buffer
        u8    *buf = (u8 *)Heap::Alloc(totalwidth);

        if (buf == nullptr)
        {
            OSD::Notify("BMP Error: temp buffer allocation failed");
            return;
        }

        int i;
        for (i = 0; i < oddRows; )
        {
            file.Read(buf, totalwidth);

            for (int j = 0; j < rows && i < oddRows; ++j)
            {
                u8  *dataPtr = Row(_height - i++ - 1); // read in inverted row order
                u8  *bufPtr = buf + j * rowWidth;

                std::copy(bufPtr, bufPtr + _rowIncrement, dataPtr);
            }
        }

        file.Read(buf, totalwidth);

        int rest = _height % 5;
        for (int k = 0; k < rest; ++k)
        {
            unsigned char *dataPtr = Row(_height - i++ - 1);
            unsigned char *bufPtr = buf + k * rowWidth;

            std::copy(bufPtr, bufPtr + _rowIncrement, dataPtr);
        }

        // Release our temporary buffer
        Heap::Free(buf);
        _loaded = true;
    }

    void     BMPImage::_LoadBitmapFromMemory(void *data)
    {
        Unload();
        _width  = 0;
        _height = 0;

        BitmapFileHeader        bfh = {};
        BitmapInformationHeader bih = {};

        std::copy((u8 *)data, (u8 *)data + sizeof(BitmapFileHeader), (u8 *)&bfh);
        std::copy((u8 *)data + sizeof(BitmapFileHeader), (u8 *)data + sizeof(BitmapFileHeader) + sizeof(BitmapInformationHeader), (u8 *)&bih);

        if (bfh.type != 19778)
        {
            OSD::Notify(Utils::Format("BMP Error: Unexpected type value: %d", bfh.type), Color::Red);
            return;
        }

        if (bih.bit_count != 24)
        {
            OSD::Notify(Utils::Format("BMP Error: Unexpected bit depth: %d", bih.bit_count), Color::Red);
            return;
        }

        _width  = bih.width;
        _height = bih.height;

        // Temporary limit ?
        if (_width > 340 || _height > 200)
        {
            OSD::Notify("BMP Error: file should not be higher than 340px * 200px", Color::Red);
            return;
        }

        _dimensions = IntVector(_width, _height);
        _bytesPerPixel = bih.bit_count >> 3;

        // Try to alloc the buffer
        if (_CreateBitmap())
        {
            OSD::Notify("BMP Error: Error while allocating required space.", Color::Red);
            return;
        }

        u32   padding = (4 - ((3 * _width) % 4)) % 4;
        int   rows = _height / 5;
        int   rowWidth = _rowIncrement + padding;
        int   totalwidth = rows * rowWidth;
        int   oddRows = rows * 5;

        // Temporary buffer
        u8    *buf = (u8 *)Heap::Alloc(totalwidth);

        if (buf == nullptr)
        {
            OSD::Notify("BMP Error: temp buffer allocation failed");
            return;
        }

        // Go to the data position in file
        u8 *pdata = (u8 *)data + bfh.off_bits;
        int i;
        for (i = 0; i < oddRows; )
        {
            std::copy(pdata, pdata + totalwidth, buf);
            pdata += totalwidth;

            for (int j = 0; j < rows && i < oddRows; ++j)
            {
                u8  *dataPtr = Row(_height - i++ - 1); // read in inverted row order
                u8  *bufPtr = buf + j * rowWidth;

                std::copy(bufPtr, bufPtr + _rowIncrement, dataPtr);
            }
        }

        std::copy(pdata, pdata + totalwidth, buf);

        int rest = _height % 5;
        for (int k = 0; k < rest; ++k)
        {
            unsigned char *dataPtr = Row(_height - i++ - 1);
            unsigned char *bufPtr = buf + k * rowWidth;

            std::copy(bufPtr, bufPtr + _rowIncrement, dataPtr);
        }

        // Release our temporary buffer
        Heap::Free(buf);
        _loaded = true;
    }
}
