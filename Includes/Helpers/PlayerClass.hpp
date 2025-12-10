#pragma once

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
    class PlayerClass {
	public:
	//Player Pointer Data Stuff
		static PlayerClass* GetInstance(int pIndex = 4);	
		float* 				GetCoordinates();
		float*				GetCoordinates(u32 wX, u32 wY);
		bool 				GetWorldCoords(u32 *wX, u32 *wY);
		u16   				GetRotation();
		float* 				GetVelocity();
		float* 				GetCollisionSize();
		u8* 				GetAnimation();
		u16* 				GetSnake();	
		u32 				Offset(u32 add = 0);
		bool 				IsLoaded();

		void				CalculateMapCoordinates(u32& x, u32& y);
		static void			CalculateCoordinates(UIntVector touchPos, float* coords);
		
	private:
		PlayerClass(void);
		~PlayerClass(void);

		static PlayerClass* m_Instance;

		static u32 m_PlayerOffset;
		static int m_PlayerIndex;
		
		u32 m_CoordinateX;
		u32 m_CoordinateY;
		u32 m_CoordinateZ;
		u32 m_Rotation;
		u32 m_Velocity;
		u32 m_Animation;
		u32 m_Snake;
		u32 m_Collision;
		float m_Coords[3];
			
		bool m_PlayerIsIndoors; 
		bool m_PlayerLoaded;
	};
}