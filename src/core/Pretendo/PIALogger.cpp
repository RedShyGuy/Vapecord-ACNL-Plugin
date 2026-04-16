/*
 * This file is part of Nimbus.
 * https://github.com/PretendoNetwork/nimbus
 *
 * Copyright (C) 2025 Nimbus contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <3ds.h>
#include <CTRPluginFramework.hpp>
#include "core/Pretendo/common.hpp"
#include "core/Pretendo/pia.hpp"
#include "core/Pretendo/PIALogger.hpp"
#include "core/Pretendo/teeny-sha1.h"
#include "platform/ctrpf/MenuEntryIds.hpp"
#include "core/infrastructure/TextID.hpp"
#include "platform/ctrpf/UtilsExtras.hpp"

namespace CTRPluginFramework
{
	u32 piaConnectionInfoTableCreateInstanceAddr = 0;
	nn::pia::transport::StationConnectionInfoTable** piaConnectionInfoTableAddr = 0;
	Mutex piaConnectionInfoTableCreateInstanceMutex;

	u32 piaIdentificationInfoTableCreateInstanceAddr = 0;
	nn::pia::transport::IdentificationInfoTable** piaIdentificationInfoTableAddr = 0;
	Mutex piaIdentificationInfoTableCreateInstanceMutex;

	// The pattern is located inside the nn::pia::transport::StationConnectionInfoTable::CreateInstance function,
	// from there we can extract the pointer to the actual instance being used
	bool extractPiaConnectionInfoTable(u32 addr) {
		Lock lock(piaConnectionInfoTableCreateInstanceMutex);
		if (piaConnectionInfoTableCreateInstanceAddr) return true;
		if (!addr) return false;
		piaConnectionInfoTableCreateInstanceAddr = addr;
		piaConnectionInfoTableAddr = *(nn::pia::transport::StationConnectionInfoTable***)(addr + 0x18);
		return true;
	}

	// The pattern is located inside the nn::pia::transport::IdentificationInfoTable::CreateInstance function,
	// from there we can extract the pointer to the actual instance being used
	bool extractPiaIdentificationInfoTable(u32 addr) {
		Lock lock(piaIdentificationInfoTableCreateInstanceMutex);
		if (piaIdentificationInfoTableCreateInstanceAddr) return true;
		if (!addr) return false;
		piaIdentificationInfoTableCreateInstanceAddr = addr;
		piaIdentificationInfoTableAddr = *(nn::pia::transport::IdentificationInfoTable***)(addr + 0x14);
		return true;
	}

	/*bool onPiaPlayersFolderAction(MenuFolder& folder, ActionType type) {
		if (type == ActionType::Opening) {

		} else if (type == ActionType::Closing) {
			folder.Clear();
		}

		return true;
	}*/
	
	std::vector<std::string> notes;
	
	void titlesChange(Keyboard& keyboard, KeyboardEvent& event) {
		if(event.type != KeyboardEvent::SelectionChanged) {
			return;
		}
		//OSD::Notify("callback");
		int index = event.selectedIndex;
		if(index < 0) {
			return;
		}
		keyboard.GetMessage() =
			Language::getInstance()->get(TextID::PLAYERS_IN_SESSION) +
			"\n\n" +
			notes.at(index);
	}
	
    void PretendoPlayersInSessionEntry(MenuEntry *entry) {
		if (*piaConnectionInfoTableAddr == nullptr) {
			MessageBox("PIA Connection Info Table not initialized yet")();
			return;
		}
		
		if (*piaIdentificationInfoTableAddr == nullptr) {
			MessageBox("PIA Identification Info Table not initialized yet")();
			return;
		}
		
		std::vector<std::string> titles;
		notes.clear();
		
		{
			auto* connectionInfoTable = *piaConnectionInfoTableAddr;
			auto* identificationInfoTable = *piaIdentificationInfoTableAddr;
			Lock connLock(connectionInfoTable->m_cs);
			Lock identLock(identificationInfoTable->m_cs);
			u32 playerCount = 0;
			for (u32 i = 0; i < connectionInfoTable->m_participantSlotCount; i++) {
				++playerCount;
				
				auto station = connectionInfoTable->m_participantStations[i];
				if (station == 0) continue; // This would technically be: if (station == nullptr) continue;
				
				nn::pia::transport::Station::IdentificationInfo* identificationInfo;
				for (u32 j = 0; j < identificationInfoTable->m_participantSlotCount; j++) {
					if (station == identificationInfoTable->m_participantStations[j]) {
						identificationInfo = &identificationInfoTable->m_participantIdentificationInfos[j];
						break;
					}
				}
				
				const u32 constantID = connectionInfoTable->m_participantConnectionInfos[i].m_publicLocation.m_constantID;
				
				std::string playerName;
				if (identificationInfo != nullptr && identificationInfo->m_playerName.m_length > 0) {
					Utils::ConvertUTF16ToUTF8(playerName, identificationInfo->m_playerName.m_name);
					playerName = Utils::Format("\"%s\"", playerName.c_str());
				}
				
				if (playerName.length() == 0) {
					playerName = Utils::Format("Player %u", playerCount);
				}
				
				std::string title = Utils::Format("%u. %s", playerCount, playerName.c_str());
				
				if (constantID == connectionInfoTable->m_localConnectionInfo.m_publicLocation.m_constantID) {
					title += " [You]";
					
					// If there is no remote connection, then we are the host
					if (connectionInfoTable->m_remoteConnectionInfo.m_publicLocation.m_constantID == 0) {
						title += " [Host]";
					}
				}
				
				if (constantID == connectionInfoTable->m_remoteConnectionInfo.m_publicLocation.m_constantID) {
					title += " [Host]";
				}
				
				u8 pidHash[20] = {};
				sha1digest(pidHash, nullptr, reinterpret_cast<const u8*>(&constantID), sizeof(u32));
				
				const u32 variableID = connectionInfoTable->m_participantConnectionInfos[i].m_publicLocation.m_variableID;
				const u32 serviceVariableID = connectionInfoTable->m_participantConnectionInfos[i].m_publicLocation.m_serviceVariableID;
				
				std::string friendCode = Utils::Format("%012llu", ((static_cast<u64>(pidHash[0]) >> 1) << 32) | constantID);
				const char* rawFriendCode = friendCode.c_str();
				std::string formattedFriendCode = Utils::Format("%.4s-%.4s-%.4s", rawFriendCode, rawFriendCode + 4, rawFriendCode + 8);
				
				const u8 rawNatMapping = connectionInfoTable->m_participantConnectionInfos[i].m_publicLocation.m_natMapping;
				std::string natMapping = rawNatMapping <= 2 ? PiaNatMapping[rawNatMapping] : Utils::Format("%d", rawNatMapping);
				
				const u8 rawNatFiltering = connectionInfoTable->m_participantConnectionInfos[i].m_publicLocation.m_natFiltering;
				std::string natFiltering = rawNatFiltering <= 2 ? PiaNatFiltering[rawNatFiltering] : Utils::Format("%d", rawNatFiltering);
				
				std::string note = Utils::Format(
					"PID: %u\n"
					"CID: %u\n"
					"RVCID: %u\n"
					"Friend code: %s\n"
					"NAT mapping: %s\n"
					"NAT filtering: %s\n",
					constantID,
					variableID,
					serviceVariableID,
					formattedFriendCode.c_str(),
					natMapping.c_str(),
					natFiltering.c_str()
				);
				//folder.Append(new MenuEntry(title, note));
				titles.emplace_back(title);
				notes.emplace_back(note);
			}
		}
		
		Keyboard optKb(Language::getInstance()->get(TextID::KEY_CHOOSE_OPTION), titles);
		
		optKb.OnKeyboardEvent(titlesChange);
		
		int index = -1;
		do {
			index = optKb.Open();
			if(index < 0) {
				continue;
			}
			optKb.GetMessage() =
				Language::getInstance()->get(TextID::PLAYERS_IN_SESSION) +
				"\n\n" +
				notes.at(index);
		} while(index >= 0);
	}

	void initPiaLogger(PatternManager& pm) {
		const u8 piaConnectionTableCreateInstancePat[] = { 0xF4, 0x00, 0x40, 0xE2, 0x00, 0x00, 0x84, 0xE5, 0x00, 0x00, 0xA0, 0xE3 };
		pm.Add(piaConnectionTableCreateInstancePat, sizeof(piaConnectionTableCreateInstancePat), extractPiaConnectionInfoTable);

		const u8 piaIdentificationTableCreateInstancePat[] = { 0x00, 0x00, 0xA0, 0xE3, 0x00, 0x40, 0x85, 0xE5, 0x70, 0x80, 0xBD, 0xE8 };
		pm.Add(piaIdentificationTableCreateInstancePat, sizeof(piaIdentificationTableCreateInstancePat), extractPiaIdentificationInfoTable);
	}

	PatternStatus getPiaLoggerStatus() {
		if (!piaConnectionInfoTableCreateInstanceAddr || !piaConnectionInfoTableCreateInstanceAddr) {
			return PatternStatus::NotFound;
		}

		return PatternStatus::Active;
	}

	bool onPiaLoggerOptionsFolderAction(MenuFolder& folder, ActionType type) {
		if (type == ActionType::Opening) {
			TextID statusNameKey = getPiaLoggerStatus() == PatternStatus::NotFound ? TextID::PIA_LOGGER_NOT_AVAILABLE : TextID::PIA_LOGGER_ACTIVE;
			
			MenuEntry *statusEntry = new MenuEntry(
				UtilsExtras::setLanguageByKey(statusNameKey),
				"",
				MenuEntryId::PretendoPIALoggerOptions
			);
			
			statusEntry->SetNameKey(static_cast<u32>(statusNameKey));
			statusEntry->SetNoteKey(0);
			
			folder.Append(statusEntry);
		} else if (type == ActionType::Closing) {
			folder.Clear();
		}
		
		return true;
	}
}
