#include "Helpers/PlayerClass.hpp"
#include "Helpers/Player.hpp"
#include "Address/Address.hpp"


namespace CTRPluginFramework {
    PlayerClass* PlayerClass::m_Instance = nullptr;
	u32	PlayerClass::m_PlayerOffset = 0;
	int	PlayerClass::m_PlayerIndex = 0;
	
	PlayerClass::~PlayerClass() {
		m_Instance = nullptr;
		m_Coords[0] = 0;
		m_Coords[1] = 0;
		m_Coords[2] = 0;
	}
	
	PlayerClass* PlayerClass::GetInstance(int pIndex) {
		if(!m_Instance) {
			m_Instance = new PlayerClass;
		}

		m_PlayerIndex = pIndex;
		m_PlayerOffset = Address(0x5C3EA0).Call<u32>(pIndex, 1);

		return m_Instance;
	}
	
	PlayerClass::PlayerClass() {
		m_CoordinateX = 0x14;
		m_CoordinateY = 0x18;
		m_CoordinateZ = 0x1C;
		m_Rotation = 0x2C;	
		m_Velocity = 0x40;
		m_Animation = 0x1A9;
		m_Snake = 0x224;
		m_Collision = 0x4F4;
		m_Coords[0] = 0;
		m_Coords[1] = 0;
		m_Coords[2] = 0;
	}
	
	bool PlayerClass::IsLoaded() {
		return m_PlayerOffset != 0;
	}
	
	u32 PlayerClass::Offset(u32 add) {
		if(m_PlayerOffset != 0) {
			return m_PlayerOffset + add;
		}
		
		return m_PlayerOffset;
	}
	
	float *PlayerClass::GetCoordinates() { //FUN_5C37B0
        if(m_PlayerOffset != 0) {
			return (float *)(m_PlayerOffset += m_CoordinateX);
		}
		
		return nullptr;
    }

	//uint FUN_0065370c(uint32_t param_1) FUNCTION which lets player walk

	float *PlayerClass::GetCoordinates(u32 wX, u32 wY) {
		if(m_PlayerOffset != 0) {
			m_Coords[0] = (float)(wX * 0x20 + 0x10);
			m_Coords[1] = GetCoordinates()[1]; //world coords don't hold Z coord
			m_Coords[2] = (float)(wY * 0x20 + 0x10);
			return m_Coords;
		}
		return m_Coords;
    }

	bool PlayerClass::GetWorldCoords(u32 *wX, u32 *wY) {
		if(m_PlayerOffset != 0) {
			static Address WorldCoords(0x5C13AC);
			return WorldCoords.Call<bool>(wX, wY, m_PlayerIndex, 1);
		}
		return 0;
	}
	
	u16 PlayerClass::GetRotation() {
        if(m_PlayerOffset != 0) {
			return *(u16 *)(m_PlayerOffset += m_Rotation);
		}
		
		return 0;
    }
	
	float *PlayerClass::GetVelocity() {
        if(m_PlayerOffset != 0) {
			return (float *)(m_PlayerOffset += m_Velocity);
		}
		
		return nullptr;
    }
	
	float *PlayerClass::GetCollisionSize() {
		if(m_PlayerOffset != 0) {
			return (float *)(m_PlayerOffset += m_Collision);
		}
		
		return nullptr;
	}
	
	u8 *PlayerClass::GetAnimation() {
		if(m_PlayerOffset != 0) {
			return (u8 *)(m_PlayerOffset += m_Animation);
		}
		
		return nullptr;
	}
	
	u16 *PlayerClass::GetSnake() {
		if(m_PlayerOffset != 0) {
			return (u16 *)(m_PlayerOffset += m_Snake);
		}
		
		return nullptr;
	}

	void PlayerClass::CalculateMapCoordinates(u32& x, u32& y) {
		bool IsInfoOpen = *(bool *)(*(u32 *)(Address(0x950C30).addr + 0x1C) + 0x5D8);
		float* coords = GetCoordinates();

	//Town Map | Can open info menu
		if(Player::GetRoom(m_PlayerIndex) == 0) {
			if(IsInfoOpen) {
				x = (u32)(-33.0 + (coords[0] / 14.2f)), y = (u32)(6.0 + (coords[2] / 14.2f));
				if(x < 12) {
					x = 12;
				}
				else if(x > 174) {
					x = 174;
				}

				if(y < 40) {
					y = 40;
				}
				else if(y > 185) {
					y = 185;
				}
			}
			else {
				x = (u32)(30.0f + (coords[0] / 14.2f)), y = (u32)(5.0f + (coords[2] / 14.2f));
				if(x < 75) {
					x = 75;
				}
				else if(x > 237) {
					x = 237;
				}

				if(y < 40) {
					y = 40;
				}
				else if(y > 185) {
					y = 185;
				}
			}
		}

	//Island Map | Can't open info menu
		else if(Player::GetRoom(m_PlayerIndex) == 0x68) {
			x = (u32)(72.0f + (coords[0] / 12.1f)), y = (u32)(23.0f + (coords[2] / 12.1f));
			if(x < 75) {
				x = 75;
			}
			else if(x > 235) {
				x = 235;
			}

			if(y < 27) {
				y = 27;
			}
			else if(y > 187) {
				y = 187;
			}
		}

	//Mainstreet Map | Can't open info menu
		else if(Player::GetRoom(m_PlayerIndex) == 1) {
			x = (u32)(-16.0 + (coords[0] / 6.2)), y = (u32)(55.0 + (coords[2] / 6.2));
			if(x < 10) {
				x = 10;
			}
			else if(x > 303) {
				x = 303;
			}

			if(y < 38) {
				y = 38;
			}
			else if(y > 182) {
				y = 182;
			}
		}

	//Tour Map | Can't open info menu
		else if(Player::GetRoom(m_PlayerIndex) >= 0x69 && Player::GetRoom(m_PlayerIndex) < 0x80) {
			x = (u32)(24.0 + (coords[0] / 13.5)), y = (u32)(-7.0 + (coords[2] / 13.5));
			if(x < 65) {
				x = 65;
			}
			else if(x > 247) {
				x = 247;
			}

			if(y < 25) {
				y = 25;
			}
			else if(y > 187) {
				y = 187;
			}
		}
	}

	void PlayerClass::CalculateCoordinates(UIntVector touchPos, float* coords) {
		static UIntRect Town_InfoClosed(70, 32, 180, 175);
		static UIntRect Town_InfoOpened(7, 32, 180, 175);
		static UIntRect Island(76, 36, 168, 168);
		static UIntRect MainStreet(4, 43, 312, 159);
		static UIntRect Tour(65, 34, 190, 170);

		bool IsInfoOpen = *(bool *)(*(u32 *)(Address(0x950C30).addr + 0x1C) + 0x5D8);
		FloatVector fPos(touchPos);

		float x = 0, y = 0;

	//Town Map | Can open info menu
		if(Player::GetRoom(m_PlayerIndex) == 0) {
			if(IsInfoOpen) {
				if(Town_InfoOpened.Contains(touchPos)) {
					x = (fPos.x - -33.0) * 14.2f, y = (fPos.y - 6.0) * 14.2f;
				}
				else {
					return;
				}
			}
			else {
				if(Town_InfoClosed.Contains(touchPos)) {
					x = (fPos.x - 30.0f) * 14.2f, y = (fPos.y - 5.0f) * 14.2f;
				}
				else {
					return;
				}
			}
		}

	//Island Map | Can't open info menu
		else if(Player::GetRoom(m_PlayerIndex) == 0x68) {
			if(Island.Contains(touchPos)) {
				x = (fPos.x - 72.0f) * 12.1f, y = (fPos.y - 23.0f) * 12.1f;
			}
			else {
				return;
			}
		}

	//Mainstreet Map | Can't open info menu
		else if(Player::GetRoom(m_PlayerIndex) == 1) {
			if(MainStreet.Contains(touchPos)) {
				x = (fPos.x - -16.0) * 6.2, y = (fPos.y - 55.0) * 6.2;
			}
			else {
				return;
			}
		}

	//Tour Map | Can't open info menu
		else if(Player::GetRoom(m_PlayerIndex) >= 0x69 && Player::GetRoom(m_PlayerIndex) < 0x80) {
			if(Tour.Contains(touchPos)) {
				x = (fPos.x - 24.0) * 13.5, y = (fPos.y - -7.0) * 13.5;
			}
			else {
				return;
			}
		}

		else {
			return;
		}

		coords[0] = x;
		coords[2] = y;
	}
}