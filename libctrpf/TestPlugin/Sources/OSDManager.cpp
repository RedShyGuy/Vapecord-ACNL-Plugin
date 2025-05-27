#include "OSDManager.hpp"

namespace CTRPluginFramework
{
    _OSDManager*  _OSDManager::_singleton = nullptr;

    OSDMI&  OSDMI::operator=(const std::string &str)
    {
        OSDManager.Lock();
        std::get<1>(data) = str;
        std::get<4>(data) = true;
        OSDManager.Unlock();
        return (*this);
    }

    OSDMI&  OSDMI::operator=(const OSDMITuple &tuple)
    {
        OSDManager.Lock();
        data = tuple;
        OSDManager.Unlock();
        return (*this);
    }

    OSDMI&  OSDMI::SetPos(u32 posX, u32 posY)
    {
        OSDManager.Lock();
        std::get<2>(data) = posX;
        std::get<3>(data) = posY;
        OSDManager.Unlock();
        return (*this);
    }

    OSDMI&  OSDMI::SetScreen(bool topScreen)
    {
        OSDManager.Lock();
        std::get<0>(data) = topScreen;
        OSDManager.Unlock();
        return (*this);
    }

    OSDMI&  OSDMI::Enable(void)
    {
        OSDManager.Lock();
        std::get<4>(data) = true;
        OSDManager.Unlock();
        return (*this);
    }

    OSDMI&  OSDMI::Disable(void)
    {
        OSDManager.Lock();
        std::get<4>(data) = false;
        OSDManager.Unlock();
        return (*this);
    }

    OSDMI::OSDMI(OSDMITuple &tuple) : data(tuple)
    {

    }

    _OSDManager::~_OSDManager(void)
    {
        OSD::Stop(OSDCallback);
        _items.clear();
    }

    _OSDManager* _OSDManager::GetInstance(void)
    {
        if (_singleton == nullptr)
            _singleton = new _OSDManager;
        return (_singleton);
    }

    void    _OSDManager::Lock(void)
    {
        _mutex.Lock();
    }

    void    _OSDManager::Unlock(void)
    {
        _mutex.Unlock();
    }

    u32     _OSDManager::Size(void)
    {
        return _items.size();
    }

    OSDMI   _OSDManager::operator[](const u32 key)
    {
        Lock();
        OSDMI i(_items[key]);
        Unlock();
        return (i);
    }

    void    _OSDManager::Remove(const u32 key)
    {
        Lock();
        _items.erase(key);
        Unlock();
    }

    _OSDManager::_OSDManager(void) :
        _renderTask1(RenderTaskFunc, nullptr, Task::AppCores),
        _renderTask2(RenderTaskFunc, (void *)1, Task::AppCores)
    {
        OSD::Run(OSDCallback);
    }

        enum
    {
        SCREEN,
        TEXT,
        POSX,
        POSY,
        STATE,
        FGCOLOR,
        BGCOLOR
    };

    static const Screen *g_screen;
    static std::vector<OSDMITuple *>   g_toDraw;

    static u32 divu3(u32 n)
    {
        u32 q, r;
        q = (n >> 2) + (n >> 4); // q = n*0.0101 (approx).
        q = q + (q >> 4); // q = n*0.01010101.
        q = q + (q >> 8);
        q = q + (q >> 16);
        r = n - q*3; // 0 <= r <= 15.
        //return q + (11*r >> 5); // Returning q + r/3.
        // return q + (5*(r + 1) >> 4); // Alternative 1.
        return q + ((r + 5 + (r << 2)) >> 4);// Alternative 2.
    }

    bool    _OSDManager::OSDCallback(const Screen &screen)
    {
        using CTRPFLock = CTRPluginFramework::Lock;

        _OSDManager &manager = OSDManager;
        CTRPFLock   lock(manager._mutex);

        // If there's no item to draw
        if (manager._items.empty())
            return false;

        g_screen = &screen;
        g_toDraw.clear();
        for (auto &item : _singleton->_items)
        {
            auto &t = item.second;

            if (std::get<STATE>(t) && !std::get<TEXT>(t).empty()
                && std::get<SCREEN>(t) == g_screen->IsTop)
                g_toDraw.push_back(&t);
        }

        if (g_toDraw.size() > 1)
        {
            manager._renderTask1.Start();
            manager._renderTask2.Start();
        }

        using Iter = std::vector<OSDMITuple *>::iterator;

        Iter iter;
        Iter end;
        u32 size = g_toDraw.size();
        u32 tier = size > 1 ? divu3(size) : size;


            // last tier of the items
            iter = g_toDraw.begin();
            end = std::next(iter, tier);

            for (; iter != end; ++iter)
            {
                auto &t = **iter;
                g_screen->Draw(std::get<TEXT>(t), std::get<POSX>(t), std::get<POSY>(t));
            }

        return (true | manager._renderTask1.Wait() | manager._renderTask2.Wait()) > 0;
    }

    s32     _OSDManager::RenderTaskFunc(void *arg)
    {
        using Iter = std::vector<OSDMITuple *>::iterator;

        Iter            iter;
        Iter            end;
        u32             size;
        u32             tier;
        s32             fbEdited = 1;
        std::vector<OSDMITuple *>   &toDraw = g_toDraw;

        size = toDraw.size();
        tier = divu3(size);

        if (arg != nullptr && size > 2)
        {
            // last tier of the items
            iter = std::next(toDraw.begin(),  tier << 1);
            end = toDraw.end();

            for (; iter != end; ++iter)
            {
                auto &t = **iter;
                g_screen->Draw(std::get<TEXT>(t), std::get<POSX>(t), std::get<POSY>(t));
            }
            //fbEdited = 1;
        }
        else
        {
            // Second tier of the items
            iter = std::next(toDraw.begin(), tier);
            end = std::next(iter, tier << 1);

            for (; iter != end; ++iter)
            {
                auto &t = **iter;
                g_screen->Draw(std::get<TEXT>(t), std::get<POSX>(t), std::get<POSY>(t));
            }
            //fbEdited = 1;
        }

        return fbEdited;
    }
}
