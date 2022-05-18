#include "types.h"
#include "3ds.h"

#include "CTRPluginFramework/Graphics.hpp"
#include "CTRPluginFrameworkImpl/Graphics.hpp"

namespace CTRPluginFramework
{
    extern "C" unsigned char *About15;
    extern "C" unsigned char *AddFavorite25;
    extern "C" unsigned char *AddFavoriteFilled25;
    extern "C" unsigned char *CheckedCheckbox;
    extern "C" unsigned char *UnCheckedCheckbox;
    extern "C" unsigned char *CapsLockOn15;
    extern "C" unsigned char *CapsLockOnFilled15;
    extern "C" unsigned char *CentreofGravity15;
    extern "C" unsigned char *ClearSymbol15;
    extern "C" unsigned char *ClearSymbolFilled15;
    extern "C" unsigned char *Clipboard25;
    extern "C" unsigned char *ClipboardFilled25;
    extern "C" unsigned char *CloseWindow20;
    extern "C" unsigned char *CloseWindowFilled20;
    extern "C" unsigned char *Controller15;
    extern "C" unsigned char *Cut25;
    extern "C" unsigned char *CutFilled25;
    extern "C" unsigned char *Duplicate25;
    extern "C" unsigned char *DuplicateFilled25;
    extern "C" unsigned char *Edit25;
    extern "C" unsigned char *EditFilled25;
    extern "C" unsigned char *EnterKey15;
    extern "C" unsigned char *EnterKeyFilled15;
    extern "C" unsigned char *FolderFilled;
    extern "C" unsigned char *File15;
    extern "C" unsigned char *GameController15;
    extern "C" unsigned char *GameController25;
    extern "C" unsigned char *GameControllerFilled25;
    extern "C" unsigned char *Grid15;
    extern "C" unsigned char *Happy15;
    extern "C" unsigned char *HappyFilled15;
    extern "C" unsigned char *Info25;
    extern "C" unsigned char *InfoFilled25;
    extern "C" unsigned char *HandCursor15;
    extern "C" unsigned char *Maintenance15;
    extern "C" unsigned char *More15;
    extern "C" unsigned char *Keyboard25;
    extern "C" unsigned char *KeyboardFilled25;
    extern "C" unsigned char *Plus25;
    extern "C" unsigned char *PlusFilled25;
    extern "C" unsigned char *RAM15;
    extern "C" unsigned char *Rocket40;
    extern "C" unsigned char *Restart15;
    extern "C" unsigned char *Save25;
    extern "C" unsigned char *Search15;
    extern "C" unsigned char *Settings15;
    extern "C" unsigned char *Shutdown15;
    extern "C" unsigned char *Star15;
    extern "C" unsigned char *Trash25;
    extern "C" unsigned char *TrashFilled25;
    extern "C" unsigned char *Unsplash15;
    extern "C" unsigned char *UserManualFilled15;
    extern "C" unsigned char *DefaultKeyboardCustomIcon;

    CustomIcon Icon::DefaultCustomIcon{(CustomIcon::Pixel*) DefaultKeyboardCustomIcon, 30, 30, true};

    #define RGBA8 GSP_RGBA8_OES

    struct Pixel
    {
        u8 a;
        u8 b;
        u8 g;
        u8 r;
    };

    inline int Icon::DrawImg(u8 *img, int posX, int posY, int sizeX, int sizeY)
    {
        u8      *framebuf = nullptr;
        u8      *imgb = img;
        u32     target = Renderer::GetContext()->target;
        int     rowstride;
        int     bpp;
        bool    is3d = false;
        Color   px;

        posY += sizeY;
        GSPGPU_FramebufferFormat fmt;
        // Get target infos
        switch (target)
        {
            case Target::TOP:
            {
                framebuf = ScreenImpl::Top->GetLeftFrameBuffer(posX, posY);
                ScreenImpl::Top->GetFrameBufferInfos(rowstride, bpp, fmt);
                break;
            }
            case Target::BOTTOM:
            {
                framebuf = ScreenImpl::Bottom->GetLeftFrameBuffer(posX, posY);
                ScreenImpl::Bottom->GetFrameBufferInfos(rowstride, bpp, fmt);
                break;
            }
            default:
                return (posX);
        }
    again3D:
        if (framebuf == nullptr)
            return (posX);

        // Draw
        for (int x = 0; x < sizeX; x++)
        {
            u8 *dst = framebuf + rowstride * x;
            int y = 0;
            while (y++ < sizeY)
            {
                Pixel *pix = (Pixel *)img;
                px.a = pix->a;
                px.r = pix->r;
                px.g = pix->g;
                px.b = pix->b;

                // Skip pixels with less than 10% visibility
                if (px.a > 25)
                {
                    Color &&bg = PrivColor::FromFramebuffer(dst);
                    Color &&blended = bg.Blend(px, Color::BlendMode::Alpha);
                    dst = PrivColor::ToFramebuffer(dst, blended);
                }
                else
                    dst += bpp;
                img += 4;
            }
        }

        if (!is3d && target == Target::TOP && ScreenImpl::Top->Is3DEnabled())
        {
            framebuf = ScreenImpl::Top->GetRightFrameBuffer(posX, posY);
            img = imgb;
            is3d = true;
            goto again3D;
        }

        return (posX + sizeX);
    }

    /*
    ** About
    ** 15px * 15px
    **************/
    int     Icon::DrawAbout(int posX, int posY)
    {
        return (DrawImg(About15, posX, posY, 15, 15));
    }

    /*
    ** CapsLockOn
    ** 15px * 15px
    ************/
    int Icon::DrawCapsLockOn(int posX, int posY, bool isFilled)
    {
        u8 *img = isFilled ? CapsLockOnFilled15 : CapsLockOn15;

        return (DrawImg(img, posX, posY, 15, 15));
    }

    /*
    ** CentreOfGravity
    ** 15px * 15px
    **************/
    int     Icon::DrawCentreOfGravity(int posX, int posY)
    {
        return (DrawImg(CentreofGravity15, posX, posY, 15, 15));
    }

    /*
    ** CheckBox
    ** 15px * 15px
    ************/
   /* int     Icon::DrawCheckBox(IntVector &pos, bool isChecked)
    {
        // Define which version to draw
        u8 *img = isChecked ? CheckedCheckbox : UnCheckedCheckbox;

        return (DrawImg(img, pos.y, pos.y, 15, 15));
    }*/

    int     Icon::DrawCheckBox(int posX, int posY, bool isChecked)
    {
        // Define which version to draw
        u8 *img = isChecked ? CheckedCheckbox : UnCheckedCheckbox;

        return (DrawImg(img, posX, posY, 15, 15));
    }

    /*
    ** ClearSymbol
    ** 15px * 15px
    **********/
    int     Icon::DrawClearSymbol(int posX, int posY, bool filled)
    {
        u8 *img = filled ? ClearSymbolFilled15 : ClearSymbol15;

        return (DrawImg(img, posX, posY, 15, 15));
    }

    /*
    ** Clipboard
    ** 25px * 25px
    *************/
    int     Icon::DrawClipboard(int posX, int posY, bool filled)
    {
        u8 *img = filled ? ClipboardFilled25 : Clipboard25;

        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** Close
    ** 20px * 20px
    **********/

    /*int     Icon::DrawClose(IntVector &pos, bool filled)
    {
        // Define which version to draw
        u8 *img = filled ? CloseWindowFilled20 : CloseWindow20;

        return (DrawImg(img, pos.x, pos.y, 20, 20));
    } */

    int     Icon::DrawClose(int posX, int posY, bool filled)
    {
        // Define which version to draw
        u8 *img = filled ? CloseWindowFilled20 : CloseWindow20;

        return (DrawImg(img, posX, posY, 20, 20));
    }

    /*
    ** Controller
    ** 15px * 15px
    ***************/

    int     Icon::DrawController(int posX, int posY)
    {
        return (DrawImg(Controller15, posX, posY, 15, 15));
    }

    /*
    ** Cut
    ** 25px * 25px
    *************/
    int     Icon::DrawCut(int posX, int posY, bool filled)
    {
        u8 *img = filled ? CutFilled25 : Cut25;

        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** Cut
    ** 25px * 25px
    *************/
    int     Icon::DrawDuplicate(int posX, int posY, bool filled)
    {
        u8 *img = filled ? DuplicateFilled25 : Duplicate25;

        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** Edit
    ** 25px * 25px
    *************/
    int     Icon::DrawEdit(int posX, int posY, bool filled)
    {
        u8 *img = filled ? EditFilled25 : Edit25;
        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** Enter Key
    ** 15px * 15px
    **************/
    int     Icon::DrawEnterKey(int posX, int posY, bool filled)
    {
        u8 *img = filled ? EnterKeyFilled15 : EnterKey15;

        return (DrawImg(img, posX, posY, 15, 15));
    }

    /*
    ** Folder
    ** 15px * 15px
    ***************/

    int     Icon::DrawFolder(IntVector &pos)
    {
        return (DrawImg(FolderFilled, pos.x, pos.y, 15, 15));
    }

    int     Icon::DrawFolder(int posX, int posY)
    {
        return (DrawImg(FolderFilled, posX, posY, 15, 15));
    }

    /*
    ** Favorite
    ** 25px * 25px
    ***************/

    /*int     Icon::DrawAddFavorite(IntVector &pos, bool filled)
    {
        u8 *img = filled ? AddFavoriteFilled25 : AddFavorite25;
        return (DrawImg(img, pos.x, pos.y, 25, 25));
    }*/

    int     Icon::DrawAddFavorite(int posX, int posY, bool filled)
    {
        u8 *img = filled ? AddFavoriteFilled25 : AddFavorite25;
        return (DrawImg(img, posX, posY, 25, 25));
    }

   /* int     Icon::DrawFavorite(IntVector &pos)
    {
        return (DrawImg(Star15, pos.x, pos.y, 15, 15));
    } */

    int     Icon::DrawFavorite(int posX, int posY)
    {
        return (DrawImg(Star15, posX, posY, 15, 15));
    }

    /*
    ** File
    ** 15px * 15px
    **************/
    int     Icon::DrawFile(int posX, int posY)
    {
        return (DrawImg(File15, posX, posY, 15, 15));
    }

    /*
    ** Game Controller
    ** 15px * 15px
    **************/
    int     Icon::DrawGameController(int posX, int posY)
    {
        return (DrawImg(GameController15, posX, posY, 15, 15));
    }

    int Icon::DrawGameController25(int posX, int posY, bool filled)
    {
        u8 *img = filled ? GameControllerFilled25 : GameController25;
        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** Grid
    ** 15px * 15px
    **************/
    int     Icon::DrawGrid(int posX, int posY)
    {
        return (DrawImg(Grid15, posX, posY, 15, 15));
    }

    /*
    ** Info
    ** 25px * 25px
    ***************/

    /*int     Icon::DrawInfo(IntVector &pos, bool filled)
    {
        u8 *img = filled ? InfoFilled25 : Info25;
        return (DrawImg(img, pos.x, pos.y, 25, 25));
    }*/

    int     Icon::DrawInfo(int posX, int posY, bool filled)
    {
        u8 *img = filled ? InfoFilled25 : Info25;
        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** Guide
    ** 15px * 15px
    ***************/

    /*int     Icon::DrawGuide(IntVector &pos)
    {
        return (DrawImg(UserManualFilled15, pos.x, pos.y, 15, 15));
    }*/

    int     Icon::DrawGuide(int posX, int posY)
    {
        return (DrawImg(UserManualFilled15, posX, posY, 15, 15));
    }

    /*
    ** Hand Cursor
    ** 15px * 15px
    ***************/
    int     Icon::DrawHandCursor(int posX, int posY)
    {
        DrawImg(HandCursor15, posX, posY, 15, 15);
        return (posX + 15);
    }

    /*
    ** Happy face
    ** 15 px * 15 px
    ****************/
    int     Icon::DrawHappyFace(int posX, int posY, bool isFilled)
    {
        u8 *img = isFilled ? HappyFilled15 : Happy15;

        return (DrawImg(img, posX, posY, 15, 15));
    }

    /*
    ** Keyboard
    ** 25px * 25px
    **************/
    int     Icon::DrawKeyboard(int posX, int posY, bool filled)
    {
        u8 *img = filled ? KeyboardFilled25 : Keyboard25;

        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** More
    ** 15px * 15px
    **************/
    int     Icon::DrawMore(int posX, int posY)
    {
        return (DrawImg(More15, posX, posY, 15, 15));
    }

    /*
    ** Plus
    ** 25px * 25px
    **************/
    int     Icon::DrawPlus(int posX, int posY, bool filled)
    {
        u8 *img = filled ? PlusFilled25 : Plus25;

        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** RAM
    ** 15px * 15px
    ***************/

    int     Icon::DrawRAM(int posX, int posY)
    {
        return (DrawImg(RAM15, posX, posY, 15, 15));
    }

    /*
    ** Restart
    ** 15px * 15 px
    **************/
    int     Icon::DrawRestart(int posX, int posY)
    {
        return (DrawImg(Restart15, posX, posY, 15, 15));
    }

    /*
    ** Rocket
    ** 40px * 40px
    **************/
    int     Icon::DrawRocket(int posX, int posY)
    {
        return (DrawImg(Rocket40, posX, posY, 40, 40));
    }

    /*
    ** Save
    ** 25px * 25px
    **************/
    int     Icon::DrawSave(int posX, int posY)
    {
        return (DrawImg(Save25, posX, posY, 25, 25));
    }

    /*
    ** Search
    ** 15px * 15px
    ***************/

    int     Icon::DrawSearch(int posX, int posY)
    {
        return (DrawImg(Search15, posX, posY, 15, 15));
    }

    /*
    ** Settings
    ** 15px * 15px
    ***************/

    int     Icon::DrawSettings(int posX, int posY)
    {
        return (DrawImg(Settings15, posX, posY, 15, 15));
    }

    /*
    ** Shutdown
    ** 15px * 15 px
    **************/
    int     Icon::DrawShutdown(int posX, int posY)
    {
        return (DrawImg(Shutdown15, posX, posY, 15, 15));
    }

    /*
    ** Tools
    ** 15px * 15px
    ***************/

    int     Icon::DrawTools(int posX, int posY)
    {
        return (DrawImg(Maintenance15, posX, posY, 15, 15));
    }

    /*
    ** Trash
    ** 25px * 25px
    ***************/
    int     Icon::DrawTrash(int posX, int posY, bool filled)
    {
        u8 *img = filled ? TrashFilled25 : Trash25;

        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** Unsplash
    ** 15px * 15 px
    **************/
    int     Icon::DrawUnsplash(int posX, int posY)
    {
        return (DrawImg(Unsplash15, posX, posY, 15, 15));
    }

    int     Icon::DrawCustomIcon(const CustomIcon& icon, int posX, int posY)
    {
        return (DrawImg((u8*)icon.pixArray, posX, posY, icon.sizeX, icon.sizeY));
    }
}
