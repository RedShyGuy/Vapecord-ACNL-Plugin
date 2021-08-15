#include <CTRPluginFramework.hpp>
#include "cheats.hpp"
#include "RegionCodes.hpp"
#include "TextFileParser.hpp"

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
		if(!m_Instance)
			m_Instance = new PlayerClass;

		m_PlayerIndex = pIndex;

		static FUNCT func(Code::PlayerInstance);
		m_PlayerOffset = func.Call<u32>(pIndex, 1);

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
		if(m_PlayerOffset != 0)
			return m_PlayerOffset + add;
		
		return m_PlayerOffset;
	}
	
	float *PlayerClass::GetCoordinates() { //FUN_5C37B0
        if(m_PlayerOffset != 0)
			return (float *)(m_PlayerOffset += m_CoordinateX);
		
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
			static const u32 WorldCoords = Region::AutoRegion(0x5C13AC, 0x5C08DC, 0x5C03F4, 0x5C03F4, 0x5BFCE4, 0x5BFCE4, 0x5BF9B8, 0x5BF9B8);
			static FUNCT func(WorldCoords);
			return func.Call<bool>(wX, wY, m_PlayerIndex, 1);
		}
		return 0;
	}
	
	u16 PlayerClass::GetRotation() {
        if(m_PlayerOffset != 0)
			return *(u16 *)(m_PlayerOffset += m_Rotation);
		
		return 0;
    }
	
	float *PlayerClass::GetVelocity() {
        if(m_PlayerOffset != 0)
			return (float *)(m_PlayerOffset += m_Velocity);
		
		return nullptr;
    }
	
	float *PlayerClass::GetCollisionSize() {
		if(m_PlayerOffset != 0)
			return (float *)(m_PlayerOffset += m_Collision);
		
		return nullptr;
	}
	
	u8 *PlayerClass::GetAnimation() {
		if(m_PlayerOffset != 0)
			return (u8 *)(m_PlayerOffset += m_Animation);
		
		return nullptr;
	}
	
	u16 *PlayerClass::GetSnake() {
		if(m_PlayerOffset != 0)
			return (u16 *)(m_PlayerOffset += m_Snake);
		
		return nullptr;
	}
}