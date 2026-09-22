#pragma once

#include "ArcResAcc.hpp"
#include "../LoadSplit.hpp"
#include "core/libgarden/nw/font/Font.hpp"
#include "../../../font/Mgr.hpp"

namespace ssys::ma::lyt
{
	class ArcResAccReader final : public ArcResAcc
	{
	public:
		ArcResAccReader();
		ArcResAccReader(const char* path) : ArcResAccReader()
		{
			ReadArc(path);
		}
		virtual ~ArcResAccReader() override;

		bool ReadArc(const char* path);

		// bool ReadArcCustom(const char* path);

		void* RegisterFont(const char* name, const nw::font::Font* font)
		{
			return GetResourceAccessorVRAM()->RegistFont(name, font);
		}

		void* RegisterFont(font::FontID font = font::FontID::GARDEN_MSG_16)
		{
			const auto info = font::Mgr::GetFontInfo(font);
			return RegisterFont(info.name, info.font);
		}

		nw::lyt::TextureInfo GetTexture(const char* name)
		{
			return GetResourceAccessorVRAM()->GetTexture(name);
		}

		bool SetArc(const void* data)
		{
			m_pArc = data;
			return SetData(data, ".");
		}

		const void* GetArc()
		{
			return m_pArc;
		}

		void Clear();

	private:
		u8 field1_0x148;
		u8 field2_0x149;
		u8 field3_0x14a;
		u8 field4_0x14b;
		u8 field5_0x14c;
		u8 field6_0x14d;
		u8 field7_0x14e;
		u8 field8_0x14f;
		u8 field9_0x150;
		u8 field10_0x151;
		u8 field11_0x152;
		u8 field12_0x153;
		u8 field13_0x154;
		u8 field14_0x155;
		u8 field15_0x156;
		u8 field16_0x157;
		const void* m_pArc;
		LoadSplit m_LoadSplit;
	};
	ASSERT_SIZE(ArcResAccReader, 0x248);
}