#ifndef CTRPLUGINFRAMEWORK_DIRECTORY_HPP
#define CTRPLUGINFRAMEWORK_DIRECTORY_HPP

#include "types.h"
#include "CTRPluginFramework/System/File.hpp"

#include <string>
#include <vector>

namespace CTRPluginFramework
{
    class File;
    class Directory
    {
    public:

        enum OPResult
        {
            SUCCESS = 0,        ///< Operation succeeded
            INVALID_PATH = -1,  ///< The path is invalid
            NOT_OPEN = -2,      ///< The File instance is not opened
            INVALID_ARG = -3,   ///< One of the args passed to the operation is invalid (nullptr, address unreachable, etc)
            UNEXPECTED_ERROR = -4 ///< An error occured
        };

        /**
         * \brief Change the working directory
         * \param path The new path to set as working directory
         * \return An \ref OPResult code
         */
        static  int     ChangeWorkingDirectory(const std::string &path);

        /**
         * \brief Create a directory
         * \param path Path of the directory to create
         * \return Either a value from \ref OPResult or a FS return value
         */
        static  int     Create(const std::string &path);

        /**
         * \brief Remove the specified directoy
         * \param path The directory to remove
         * \return Either a value from \ref OPResult or a FS return value
         */
        static  int     Remove(const std::string &path);

        /**
         * \brief Rename the specified directory
         * \param oldPath The directory to rename
         * \param newPath The new name of the directory
         * \return Either a value from \ref OPResult or a FS return value
         */
        static  int     Rename(const std::string &oldPath, const std::string &newPath);

        /**
         * \brief Check if the specified directory exists
         * \param path The directory to check
         * \return
         * 1: Exists
         * 0: Doesn't exists
         */
        static  int     IsExists(const std::string &path);

        /**
         * \brief Open a directory
         * \param output Reference to the Directory object
         * \param path The directory to open
         * \param create If the directory must be created
         * \return Either a value from \ref OPResult or a FS return value
         */
        static int     Open(Directory &output, const std::string &path, bool create = false);

        /**
         * \brief Close a Directory
         * \return Either a value from \ref OPResult or a FS return value
         */
        int     Close(void) const;

        /**
         * \brief Open a file within the current Directory
         * \param output The File object
         * \param path The file to open
         * \param mode The mode to open the file with
         * \return Either a value from \ref OPResult or a FS return value
         */
        int     OpenFile(File &output, const std::string &path, int mode = File::RW) const;

        /**
         * \brief List all the files within the current Directory
         * \param files The list of files that will be returned \n
         * Beware that the container is not emptied and results files are appended
         * \param pattern If specified, only the files that contain the pattern will be returned
         * \return The count of files found, OPResult::NOT_OPEN if the Directory is not opened
         */
        int     ListFiles(std::vector<std::string> &files, const std::string &pattern = "") const;

        /**
        * \brief List all the directories within the current Directory
        * \param directories The list of directories that will be returned \n
        * Beware that the container is not emptied and results directories are appended
        * \param pattern If specified, only the directories that contain the pattern will be returned
        * \return The count of directories found, OPResult::NOT_OPEN if the Directory is not opened
        */
        int     ListDirectories(std::vector<std::string> &directories, const std::string &pattern = "") const;

        /**
         * \brief Get the name of the current Directory
         * \return An std::string with the name of the current Directory
         */
        std::string     GetName(void) const;

        /**
         * \brief Get the full path of the current Directory
         * \return An std::string with the full path of the current Directory
         */
        std::string     GetFullName(void) const;

        /**
         * \brief Check if the current Directory is open
         * \return true if the specified path was successfully opened, false otherwise
         */
        bool    IsOpen(void) const;

        Directory(void);
        Directory(const std::string &path, bool create = false);
        ~Directory();

    private:

        int             _List(void) const;

        std::string     _path;
        Handle          _handle;
        mutable bool    _isOpen;
        mutable Mutex   _mutex;
        struct DirectoryEntry
        {
            DirectoryEntry(u32 attrib, u8 *name);
            u32         attributes;
            std::string name;
        };
        mutable std::vector<DirectoryEntry>    _list;
    };
}

#endif