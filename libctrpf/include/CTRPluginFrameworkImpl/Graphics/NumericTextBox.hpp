#ifndef CTRPLUGINFRAMEWORKIMPL_NUMERICTEXTBOX_HPP
#define CTRPLUGINFRAMEWORKIMPL_NUMERICTEXTBOX_HPP

#include "CTRPluginFrameworkImpl/Graphics/Drawable.hpp"
#include "CTRPluginFramework/System/Vector.hpp"
#include "CTRPluginFramework/System/Rect.hpp"
#include <string>
#include <vector>

namespace CTRPluginFramework
{
    class NumericTextBox : public Drawable
    {
    public:

        enum class Type
        {
            Bits8,
            Bits16,
            Bits32,
         //   Bits64,
            Float,
         //   Double
        };

        NumericTextBox(int posX, int posY, int width, int height);

        void    SetValue(u8 val);
        void    SetValue(u16 val);
        void    SetValue(u32 val);
      //  void    SetValue(u64 val);
        void    SetValue(float val);
       // void    SetValue(double val);

		void	UseHexadecimal(bool useHex);

        void    Clear(void);
        void    Draw(void) override;
        void    Update(const bool isTouchDown, const IntVector &touchPos) override;
        bool    operator()(void) override;

        bool    IsEnabled;
        bool    IsVisible;
        Type    ValueType;

        union
        {
            u8      Bits8;
            u16     Bits16;
            u32     Bits32;
          //  u64     Bits64;
            float   Float;
          //  double  Double;
        };

    private:
        bool                        _execute;
        bool                        _isTouched;
		bool						_isHexadecimal;
        IntRect                     _rectPos;
        std::string                 _text;

        void    _UpdateVal(void);

    };
}

#endif