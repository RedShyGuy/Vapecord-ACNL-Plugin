#include <CTRPluginFramework.hpp>
#include "core/infrastructure/SaveBackupManager.hpp"
#include "core/infrastructure/Language.hpp"
#include "core/HUD.hpp"
#include <vector>
#include <string>

namespace CTRPluginFramework {

	static const std::string AUTOBACKUP_PREFIX = "AutoBackup";

	void PerformAutoSaveBackup(void) {
		const Language *lang = Language::getInstance();

		HUD::Notify(lang->get(TextID::AUTOBACKUP_START), Color::Gray);

		const FwkSettings &settings = FwkSettings::Get();
		u32 maxSlots = settings.SaveBackupMaxSlots;

		std::string gamePath = SaveBackupManager::GetGamePath();

	//Only count our auto-backups, ignore user-created Checkpoint backups
		std::vector<std::string> autoBackups;
		SaveBackupManager::ListBackups(gamePath, AUTOBACKUP_PREFIX, autoBackups);

	//If at max capacity, delete the oldest to make room
		if (autoBackups.size() >= maxSlots) {
			std::string oldestFolder = SaveBackupManager::FindOldestBackup(gamePath, autoBackups);
			Directory::Remove(gamePath + "/" + oldestFolder);
		}

		std::string folderName;
		if (SaveBackupManager::BackupAllToCheckpoint(AUTOBACKUP_PREFIX, folderName)) {
			HUD::Notify(Utils::Format("%s", folderName.c_str()), Color::LimeGreen);
		}
		else if (!folderName.empty()) {
			HUD::Notify(Utils::Format(lang->get(TextID::AUTOBACKUP_PARTIAL).c_str(), folderName.c_str()), Color::Orange);
		}
		else {
			HUD::Notify(lang->get(TextID::AUTOBACKUP_FAIL), Color::Red);
		}
	}
}
