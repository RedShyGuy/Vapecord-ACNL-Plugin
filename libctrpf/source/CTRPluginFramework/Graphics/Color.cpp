#include "CTRPluginFramework/Graphics/Color.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"
#include <algorithm>
#include <algorithm>
#include <cstdarg>
#include <vector>

namespace CTRPluginFramework
{

    const Color     Color::Black;
    const Color     Color::White(255, 255, 255);
    const Color     Color::Red(255, 0, 0);
    const Color     Color::Lime(0, 255, 0);
    const Color     Color::Blue(0, 0, 255);
    const Color     Color::Yellow(255, 255, 0);
    const Color     Color::Cyan(0, 255, 255);
    const Color     Color::Magenta(255, 0, 255);
    const Color     Color::Silver(192, 192, 192);
    const Color     Color::Gray(128, 128, 128);
    const Color     Color::Maroon(128, 0, 0);
    const Color     Color::Olive(128, 128, 0);
    const Color     Color::Green(0, 128, 0);
    const Color     Color::Purple(128, 0, 128);
    const Color     Color::Teal(0, 128, 128);
    const Color     Color::Navy(0, 0, 128);

    const Color     Color::BlackGrey = Color(15, 15, 15);
    const Color     Color::Brown = Color(165, 42, 42);
    const Color     Color::DarkGrey = Color(169, 169, 169);
    const Color     Color::DeepSkyBlue = Color(0, 191, 255);
    const Color     Color::DimGrey = Color(105, 105, 105);
    const Color     Color::DodgerBlue = Color(30, 144, 255);
    const Color     Color::Gainsboro = Color(220, 220, 220);
    const Color     Color::ForestGreen = Color(34, 139, 34);
    const Color     Color::LimeGreen = Color(50, 205, 50);
    const Color     Color::Orange = Color(255, 128, 0);
    const Color     Color::SkyBlue = Color(135, 206, 235);
    const Color     Color::Turquoise = Color(64, 224, 208);

    Color&   Color::Fade(float fading)
    {
        if (fading > 1.0f || fading < -1.0f)
            return *this;

        if (fading > 0.0f)
        {
            float tint = 1.f - fading;
            r = std::min((int)(255 - (255 - r) * tint), 255);
            g = std::min((int)(255 - (255 - g) * tint), 255);
            b = std::min((int)(255 - (255 - b) * tint), 255);
        }
        else
        {
            float shade = 1.f + fading;

            r *= shade;
            g *= shade;
            b *= shade;
        }
        return *this;
    }

    Color Color::Blend(const Color &color, BlendMode mode) const
    {
        // This is background, color is foreground
        Color           ret;
        unsigned int    _r;
        unsigned int    _g;
        unsigned int    _b;
        unsigned int    _a;
        float           forealpha = (float)color.a / 255.f;
        float           minusForeAlpha = 1.f - forealpha;

        switch (mode)
        {
        case BlendMode::Alpha:
            _r = (forealpha * (float)color.r) + ((float)r * minusForeAlpha);
            _g = (forealpha * (float)color.g) + ((float)g * minusForeAlpha);
            _b = (forealpha * (float)color.b) + ((float)b * minusForeAlpha);
            _a = color.a * a;
            ret.r = std::min(_r, 255u);
            ret.g = std::min(_g, 255u);
            ret.b = std::min(_b, 255u);
            ret.a = std::min(_a, 255u);
            break;
        case BlendMode::Add:
            _r = a * r / 255 + 255 * color.r / 255;
            _g = a * g / 255 + 255 * color.g / 255;
            _b = a * b / 255 + 255 * color.b / 255;
            _a = a + color.a;
            ret.r = std::min(_r, 255u);
            ret.g = std::min(_g, 255u);
            ret.b = std::min(_b, 255u);
            ret.a = std::min(_a, 255u);
            break;
        case BlendMode::Sub:
            _r = a * r / 255 - 255 * color.r / 255;
            _g = a * g / 255 - 255 * color.g / 255;
            _b = a * b / 255 - 255 * color.b / 255;
            _a = a - color.a;
            ret.r = std::max(_r, 0u);
            ret.g = std::max(_g, 0u);
            ret.b = std::max(_b, 0u);
            ret.a = std::max(_a, 0u);
            break;
        case BlendMode::Mul:
            ret = *this * color;
            break;
        default:
            ret = color;
            break;
        }
        return (ret);
    }
    
    std::string Color::Format(const char *fmt, ...) 
    {
		std::string res;

		va_list list;
		va_start(list, fmt);
		while(*fmt != '\0') {
			if(*fmt != '%') {
				res += *fmt;
				fmt++;
				continue;
			}

			fmt++;

			if(*fmt == '\0') 
				break;

			if(*fmt == 'c') {
				void* p = va_arg(list, void *);

				u32 RawColor = (u32)p;

				char strColor[5] = { 
					0x1B, 
					std::max((u8)1, (u8)(RawColor & 0x000000FF)), 
					std::max((u8)1, (u8)((RawColor & 0x0000FF00) >> 8)),
					std::max((u8)1, (u8)((RawColor & 0x00FF0000) >> 16)), 
				};
				res += strColor[0];
				res += strColor[1];
				res += strColor[2];
				res += strColor[3];
       		}      

			fmt++;
		}

		return res;
	}

	Color Color::GetColor(const std::string &str) 
    {
		std::vector<char> r_char(str.begin(), str.end());

		if((u8)r_char[0] == 0x1B) { //if colorcode
			u32 c_code = ((u8)r_char[1] << 24 | (u8)r_char[2] << 16 | (u8)r_char[3] << 8 | 0xFF);
			return Color(c_code);
		}
		return Color(0xFFFFFFFF);
	}

	std::string Color::RemoveColor(const std::string &str) 
    {
		std::vector<char> r_char(str.begin(), str.end());
		if((u8)r_char[0] == 0x1B) { //if colorcode
			return str.substr(4);
		}
		return str;
	}

    Color Color::Rainbow(void) {
		u8 ShiftR = Utils::Random(0, 255);
        u8 ShiftG = Utils::Random(0, 255);
        u8 ShiftB = Utils::Random(0, 255);	
		u8 ShiftA = Utils::Random(0, 255);
			
		return(ShiftR << 24 | ShiftG << 16 | ShiftB << 8 | ShiftA);
    }


    bool Color::operator < (const Color &right) const
    {
        return (( r < right.r)
            && (b < right.b)
            && (g < right.g));
    }

    bool Color::operator <= (const Color &right) const
    {
        return (( r <= right.r)
            && (b <= right.b)
            && (g <= right.g));
    }

    bool Color::operator > (const Color &right) const
    {
        return (( r > right.r)
            && (b > right.b)
            && (g > right.g));
    }

    bool Color::operator >= (const Color &right) const
    {
        return (( r >= right.r)
            && (b >= right.b)
            && (g >= right.g));
    }

    Color Color::operator+(const Color& right) const
    {
        u8 _r(std::min(r + right.r, 255));
        u8 _g(std::min(g + right.g, 255));
        u8 _b(std::min(b + right.b, 255));
        u8 _a(std::min(a + right.a, 255));

        return (Color(_r, _g, _b, _a));

    }

    Color Color::operator-(const Color& right) const
    {
        u8 _r(std::max(r - right.r, 0));
        u8 _g(std::max(g - right.g, 0));
        u8 _b(std::max(b - right.b, 0));
        u8 _a(std::max(a - right.a, 0));

        return (Color(_r, _g, _b, _a));
    }

    Color Color::operator*(const Color& right) const
    {
        u8 _r(r * right.r / 255);
        u8 _g(g * right.g / 255);
        u8 _b(b * right.b / 255);
        u8 _a(a * right.a / 255);

        return (Color(_r, _g, _b, _a));
    }

    Color &Color::operator+=(const Color& right)
    {
        *this = *this + right;
        return (*this);
    }

    Color &Color::operator-=(const Color& right)
    {
        *this = *this - right;
        return (*this);
    }

    Color &Color::operator*=(const Color& right)
    {
        *this = *this * right;
        return (*this);
    }
 }
