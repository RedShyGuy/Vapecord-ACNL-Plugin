#include "CTRPluginFrameworkImpl/Menu/GuideReader.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include <cmath>
#include <cstring>

namespace CTRPluginFramework
{
    static MenuFolderImpl *CreateFolder(std::string path)
    {
        u32                         pos = path.rfind("/");
        std::string                 name = pos != std::string::npos ? path.substr(pos + 1) : path;
        MenuFolderImpl              *mFolder = new MenuFolderImpl(name);
        Directory                   folder;
        std::vector<std::string>    directories;
        std::vector<std::string>    files;

        if (mFolder == nullptr || Directory::Open(folder, path) != 0)
        {
            delete mFolder;
            return (nullptr);
        }

        mFolder->note = path;

        // List all directories
        folder.ListDirectories(directories);
        if (!directories.empty())
        {
            for (size_t i = 0; i < directories.size(); i++)
            {
                MenuFolderImpl *subMFolder = CreateFolder(path + "/" + directories[i]);
                if (subMFolder != nullptr)
                    mFolder->Append(subMFolder);
            }
        }
        directories.clear();
        // List all files
        folder.ListFiles(files, ".txt");
        if (!files.empty())
        {
            for (size_t i = 0; i < files.size(); i++)
            {
                u32 fpos = files[i].rfind(".txt");
                std::string fname = fpos != std::string::npos ? files[i].substr(0, fpos) : files[i];
                MenuEntryImpl *entry = new MenuEntryImpl(fname, path);
                mFolder->Append(entry);
            }
        }
        folder.Close();
        return (mFolder);
    }

    #define ABS(x) x >= 0 ? 0 : -x
    // In Preferences.cpp
    BMPImage *PostProcess(BMPImage *img, int maxX, int maxY);

    GuideReader::GuideReader(void):
    _isOpen(false), _menu(CreateFolder("Guide"), Icon::DrawFile), _guideTB("", "", Window::TopWindow.GetRect()), _text(""), _last(nullptr)
    //_closeBtn(*this, nullptr, IntRect(275, 24, 20, 20), Icon::DrawClose)
    {
        _isOpen = false;
        _image = nullptr;
        if (Directory::Open(_currentDirectory, "Guide") == 0)
        {
            _currentDirectory.ListFiles(_bmpList, ".bmp");
            if (!_bmpList.empty())
            {
                _currentBMP = 0;
                _image = new BMPImage("Guide/" + _bmpList[0]);
                _image = PostProcess(_image, 280, 200);
            }
            else
                _currentBMP = -1;
        }
        else
        {
            _currentBMP = -1;
        }

    }

    /*
    ** Operator ()
    *********************/
    bool    GuideReader::operator()(EventList &eventList, Time &delta)
    {
        _isOpen = true;
        // Process event
        for (size_t i = 0; i < eventList.size(); i++)
            _ProcessEvent(eventList[i]);

        // Draw
        Draw();
        return (Window::BottomWindow.MustClose() || !_isOpen);
    }

    bool    GuideReader::Draw(void)
    {
        if (!_isOpen)
            return (false);

        /*
        ** Top Screen
        **************************************************/
        Renderer::SetTarget(TOP);
        // If a textbox exist
        if (_guideTB.IsOpen())
        {
            _guideTB.Draw();
        }
        else
        {
            _menu.Draw();
        }

        /*
        ** Bottom Screen
        **************************************************/
        Renderer::SetTarget(BOTTOM);

        static IntRect  background(20, 20, 280, 200);
        const Color     &black = Color::Black;
        const Color     &blank = Color::White;
        const Color     &dimGrey = Color::BlackGrey;

        if (_image != nullptr && _image->IsLoaded())
        {
            _image->Draw(background);
        }
        else
        {
            Renderer::DrawRect2(background, black, dimGrey);
            Renderer::DrawRect(22, 22, 276, 196, blank, false);
        }

        bool isTouchDown = Touch::IsDown();
        IntVector touchPos(Touch::GetPosition());

        Window::BottomWindow.Update(isTouchDown, touchPos);
        Window::BottomWindow.Draw();

        return (true);
    }

    void    GuideReader::_LoadBMP(void)
    {
        if (_image)
            delete _image;
        _image = new BMPImage(_currentDirectory.GetFullName() + "/" + _bmpList[_currentBMP]);
        _image = PostProcess(_image, 280, 200);
    }

    bool    GuideReader::_ProcessEvent(Event &event)
    {
        if (!_isOpen)
            return (false);

        // Process TextBox Event
        if (_guideTB.IsOpen())
        {
            _guideTB.ProcessEvent(event);
        }
        else
        {
            MenuItem *item = nullptr;
            // Process Menu Event
            int ret = _menu.ProcessEvent(event, &item);

            // If menu ask for close
            if (ret == MenuEvent::MenuClose)
            {
                Close();
                return (false);
            }

            // If folder changed
            else if (ret == MenuEvent::FolderChanged)
            {
                if (_image)
                    delete _image;
                _image = nullptr;
                _currentBMP = -1;
                _bmpList.clear();
                _currentDirectory.Close();

                if (item != nullptr && Directory::Open(_currentDirectory, item->note) == 0)
                {
                    _currentDirectory.ListFiles(_bmpList, ".bmp");
                    if (!_bmpList.empty())
                    {
                        _currentBMP = 0;
                        _image = new BMPImage(item->note + "/" + _bmpList[0]);
                        _image = PostProcess(_image, 280, 200);
                    }
                }
            }
            // If an file entry was selected by user
            else if (ret == MenuEvent::EntrySelected)
            {
                MenuEntryImpl *entry = (MenuEntryImpl *)item;
                if (entry != _last)
                {
                    _last = entry;

                    char data[0x1001];
                    File file(entry->note + "/" + entry->name + ".txt");
                    s64  size = file.GetSize();

                    if (!file.IsOpen())
                        return (false);

                    file.Rewind();
                    _text.clear();
                    if (_text.capacity() < size)
                        _text.reserve(size);

                    while (size)
                    {
                        memset(data, 0, 0x1001);
                        s64 sizeToRead = size > 0x1000 ? 0x1000 : size;

                        if (file.Read(data, sizeToRead) == File::OPResult::SUCCESS)
                        {
                            _text += data;
                            size -= sizeToRead;
                        }
                    }

                    _guideTB.Update(entry->name, _text);
                    _guideTB.Open();
                }
                else
                    _guideTB.Open();
            }
        }

        // Change image on touch swip
        if (_currentBMP != -1)
        {
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Key::L && _currentBMP > 0)
                {
                    _currentBMP--;
                    _LoadBMP();
                }
                else if (event.key.code == Key::R && static_cast<size_t>(_currentBMP) < _bmpList.size() -1)
                {
                    _currentBMP++;
                    _LoadBMP();
                }
            }
            else if (event.type == Event::TouchSwipped)
            {
                if (event.swip.direction == Event::SwipDirection::Left && _currentBMP > 0)
                {
                    _currentBMP--;
                    _LoadBMP();
                }
                else if (event.swip.direction == Event::SwipDirection::Right && static_cast<size_t>(_currentBMP) < _bmpList.size() -1)
                {
                    _currentBMP++;
                    _LoadBMP();
                }
            }
        }

        return (true);
    }

    void    GuideReader::Open(void)
    {
        _isOpen = true;
    }

    void    GuideReader::Close(void)
    {
        _isOpen = false;
    }

    bool    GuideReader::IsOpen(void)
    {
        return (_isOpen);
    }
}