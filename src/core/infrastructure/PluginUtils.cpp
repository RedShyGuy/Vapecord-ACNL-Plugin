#include <cstdint>
#include "core/infrastructure/PluginUtils.hpp"
#include "core/infrastructure/Address.hpp"
#include "Files.h"
#include "core/Config.hpp"
#include "features/cheats.hpp"
#include "core/HUD.hpp"

namespace CTRPluginFramework {
	namespace {
		u32 ToAddress(const MemoryRange &range) {
			return static_cast<u32>(reinterpret_cast<uintptr_t>(range.address));
		}

		std::string EnsureFileType(const std::string &filename, const std::string &filetype) {
			if (filetype.empty())
				return filename;

			if (filename.size() >= filetype.size() && filename.compare(filename.size() - filetype.size(), filetype.size(), filetype) == 0)
				return filename;

			return filename + filetype;
		}

	}

	OperationResult PluginUtils::Backup::DumpMemory(
		const std::string &path,
		const std::string &filename,
		const std::string &filetype,
		const std::vector<MemoryRange> &ranges,
		const DumpOptions &options
	) {
		if (ranges.empty())
			return OperationResult::Aborted;

		File file;
		Directory dir(path, true);
		const std::string outputFilename = EnsureFileType(filename, filetype);

		if (dir.OpenFile(file, outputFilename, File::RWC) != 0) {
			if (options.showMessages)
				MessageBox(Language::getInstance()->get(TextID::DUMP_ERROR2)).SetClear(ClearScreen::Top)();
			return OperationResult::OpenFailed;
		}

		for (const MemoryRange &range : ranges) {
			if (file.Dump(ToAddress(range), range.length) != 0) {
				if (options.showMessages)
					MessageBox(Language::getInstance()->get(TextID::DUMP_ERROR1)).SetClear(ClearScreen::Top)();
				return OperationResult::TransferFailed;
			}

			if (options.showRangeNotifications) {
				HUD::Notify(Utils::Format(Language::getInstance()->get(TextID::WRAPPER_ADDRESS).c_str(), range.address));
				HUD::Notify(Utils::Format(Language::getInstance()->get(TextID::WRAPPER_LENGTH).c_str(), range.length));
			}
		}

		if (options.showMessages)
			MessageBox(Language::getInstance()->get(TextID::DUMP_DUMPED) + file.GetFullName()).SetClear(ClearScreen::Top)();

		return OperationResult::Success;
	}

	OperationResult PluginUtils::Backup::DumpMemory(
		const std::string &path,
		const std::string &filename,
		const std::string &filetype,
		std::initializer_list<MemoryRange> ranges,
		const DumpOptions &options
	) {
		return DumpMemory(path, filename, filetype, std::vector<MemoryRange>(ranges), options);
	}

	Directory PluginUtils::Backup::restoreDirectory;

	OperationResult PluginUtils::Backup::RestoreMemory(
		const std::string &path,
		const std::string &filetype,
		const std::string &selectionMessage,
		const std::vector<MemoryRange> &ranges,
		const RestoreOptions &options
	) {
		if (ranges.empty())
			return OperationResult::Aborted;

		for (;;) {
			std::vector<std::string> folders;
			std::vector<std::string> files;
			std::vector<std::string> allEntries;
			std::vector<bool> isFolder;

			File file;

			if (Directory::Open(restoreDirectory, path) != Directory::OPResult::SUCCESS)
				return OperationResult::Aborted;

			if (restoreDirectory.ListDirectories(folders) == Directory::OPResult::NOT_OPEN)
				return OperationResult::Aborted;

			if (restoreDirectory.ListFiles(files, filetype) == Directory::OPResult::NOT_OPEN)
				return OperationResult::Aborted;

			if (folders.empty() && files.empty()) {
				if (options.showMessages)
					MessageBox(Language::getInstance()->get(TextID::RESTORE_NOFILES)).SetClear(ClearScreen::Top)();
				return OperationResult::ListingFailed;
			}

			for (const std::string &name : folders) {
				allEntries.push_back(name);
				isFolder.push_back(true);
			}

			for (const std::string &name : files) {
				std::string::size_type index = name.find(filetype);
				if (index != std::string::npos) {
					allEntries.push_back(name.substr(0, index));
					isFolder.push_back(false);
				}
			}

			Keyboard kb(selectionMessage);
			kb.Populate(allEntries);
			kb.OnKeyboardEvent(options.onSelectionChange);
			if (options.onSelectionChange != nullptr && !allEntries.empty()) {
				KeyboardEvent event{};
				event.type = KeyboardEvent::SelectionChanged;
				event.selectedIndex = 0;
				kb.ForceEvent(event);
			}

			int choice = kb.Open();
			if (choice < 0)
				return OperationResult::Aborted;

			std::string selectedPath = path;
			std::string selectedFile = allEntries[choice];

			if (isFolder[choice]) {
				selectedPath = Utils::Format("%s/%s", path.c_str(), selectedFile.c_str());

				if (Directory::Open(restoreDirectory, selectedPath) != Directory::OPResult::SUCCESS)
					return OperationResult::Aborted;

				files.clear();
				if (restoreDirectory.ListFiles(files, filetype) == Directory::OPResult::NOT_OPEN)
					return OperationResult::Aborted;

				allEntries.clear();
				for (const std::string &name : files) {
					std::string::size_type index = name.find(filetype);
					if (index != std::string::npos)
						allEntries.push_back(name.substr(0, index));
				}

				kb.Populate(allEntries);
				if (options.onSelectionChange != nullptr && !allEntries.empty()) {
					KeyboardEvent event{};
					event.type = KeyboardEvent::SelectionChanged;
					event.selectedIndex = 0;
					kb.ForceEvent(event);
				}
				choice = kb.Open();
				if (choice < 0)
					continue;

				selectedFile = allEntries[choice];
			}

			if (Directory::Open(restoreDirectory, selectedPath) != Directory::OPResult::SUCCESS)
				return OperationResult::Aborted;

			if (restoreDirectory.OpenFile(file, selectedFile + filetype, File::RWC) != Directory::OPResult::SUCCESS) {
				if (options.showMessages)
					MessageBox(Language::getInstance()->get(TextID::RESTORE_ERROR2)).SetClear(ClearScreen::Top)();
				return OperationResult::OpenFailed;
			}

			for (const MemoryRange &range : ranges) {
				if (file.Inject(ToAddress(range), range.length) != 0) {
					if (options.showMessages)
						MessageBox(Language::getInstance()->get(TextID::RESTORE_ERROR1)).SetClear(ClearScreen::Top)();
					return OperationResult::TransferFailed;
				}
			}

			if (options.showMessages)
				MessageBox(Language::getInstance()->get(TextID::RESTORE_RESTORE)).SetClear(ClearScreen::Top)();

			return OperationResult::Success;
		}
	}

	OperationResult PluginUtils::Backup::RestoreMemory(
		const std::string &path,
		const std::string &filetype,
		const std::string &selectionMessage,
		std::initializer_list<MemoryRange> ranges,
		const RestoreOptions &options
	) {
		return RestoreMemory(path, filetype, selectionMessage, std::vector<MemoryRange>(ranges), options);
	}

	OperationResult PluginUtils::Backup::DeleteBackup(
		const std::string &path,
		const std::string &filetype,
		const DeleteOptions &options
	) {
		std::vector<std::string> files;
		File file;
		Directory dir(path);

		if (dir.ListFiles(files, filetype) == Directory::OPResult::NOT_OPEN)
			return OperationResult::Aborted;

		if (files.empty()) {
			if (options.showMessages)
				MessageBox(Language::getInstance()->get(TextID::FILE_RES2)).SetClear(ClearScreen::Top)();
			return OperationResult::ListingFailed;
		}

		Keyboard kb("");
		kb.Populate(files);

		int choice = kb.Open();
		if (choice < 0)
			return OperationResult::Aborted;

		if (dir.OpenFile(file, files[choice], File::RWC) != 0) {
			if (options.showMessages)
				MessageBox(Language::getInstance()->get(TextID::RESTORE_ERROR2)).SetClear(ClearScreen::Top)();
			return OperationResult::OpenFailed;
		}

		if ((MessageBox(Utils::Format(Language::getInstance()->get(TextID::FILE_PROMPT).c_str(), files[choice].c_str()), DialogType::DialogYesNo)).SetClear(ClearScreen::Top)()) {
			File::Open(file, path << std::string("/") << files[choice], File::Mode::WRITE);
			file.Flush();
			file.Close();

			if (File::Remove(path << std::string("/") << files[choice]) != 0) {
				if (options.showMessages)
					MessageBox(Language::getInstance()->get(TextID::FILE_RES3)).SetClear(ClearScreen::Top)();
				return OperationResult::TransferFailed;
			}

			if (options.showMessages)
				MessageBox(Utils::Format(Language::getInstance()->get(TextID::FILE_RES1).c_str(), files[choice].c_str())).SetClear(ClearScreen::Top)();
		}

		dir.Close();
		return OperationResult::Success;
	}

	u32 PluginUtils::Branch::CalculateBranchInstruction(u32 PC, u32 target) {
		u32 instruction = ((target - PC) >> 2) - 2;

		if(PC > target) {
			instruction -= 0x3F000000;
		}

		return instruction;
	}

	u32 PluginUtils::Branch::GetBranchTarget(u32 PC, u32 instruction) {
		u32 target = instruction & 0xFFFFFF;

		target += 2;
		target = target << 2;
		target += PC;

		return (target & 0xFFFFFF);
	}
}
