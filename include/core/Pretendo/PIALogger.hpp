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

#pragma once

#include <string>
#include "PatternManager.hpp"
#include "common.hpp"

namespace CTRPluginFramework
{
	static const std::string PiaNatMapping[] = {
		"Unknown", "Endpoint independent mapping", "Endpoint dependent mapping"
	};

	static const std::string PiaNatFiltering[] = {
		"Unknown", "Port independent filtering", "Port dependent filtering"
	};

	//bool onPiaPlayersFolderAction(MenuFolder& folder, ActionType type);
	void PretendoPlayersInSessionEntry(MenuEntry *entry);
	bool onPiaLoggerOptionsFolderAction(MenuFolder& folder, ActionType type);

	void initPiaLogger(PatternManager& pm);
	PatternStatus getPiaLoggerStatus();
}
