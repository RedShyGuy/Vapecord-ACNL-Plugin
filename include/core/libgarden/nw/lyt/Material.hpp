#pragma once

#include <core/libgarden/nw/lyt/AnimationLink.hpp>
#include <core/libgarden/nw/lyt/TexMap.hpp>
#include <core/libgarden/nw/ut/Color.hpp>

namespace nw::lyt
{
	class Material
	{
	public:
		constexpr Material() = default;
		constexpr Material(bool userOwned) :
			m_Flags {userOwned ? u8 {1} : u8 {0}}
		{ }

		virtual ~Material() {
			using FN_Destructor = void (*)(Material* self);
			static const FN_Destructor destructor = reinterpret_cast<FN_Destructor>(0x004be8a8);
			destructor(this);
		}

		void SetTexture(size_t index, const TextureInfo& info)
		{
			((TexMap*)m_pElements)[index].Set(info);
			m_Flags &= 0xfb;
		}

		void ReserveMem(u8 texMapCount, u8 texMatrixCount, u8 texCoordGenCount,
			u8 tevStageCount = 0, bool alphaCompare = false, bool blendMode = false) {
				using FN_ReserveMem = void (*)(Material* self, u8 texMapCount, u8 texMatrixCount, u8 texCoordGenCount,
					u8 tevStageCount, bool alphaCompare, bool blendMode);
				static const FN_ReserveMem reserveMem = reinterpret_cast<FN_ReserveMem>(0x004bc900);
				reserveMem(this, texMapCount, texMatrixCount, texCoordGenCount, tevStageCount, alphaCompare, blendMode);
		}

		ut::Color8& GetColor(size_t index) { return m_Colors[index]; }
		const ut::Color8& GetColor(size_t index) const { return m_Colors[index]; }

		TexMatrix* GetTexSRTAry();
		const TexMatrix* GetTexSRTAry() const;

		TexCoordGen* GetTexCoordGenAry();
		const TexCoordGen* GetTexCoordGenAry() const;

	private:
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Winvalid-offsetof"
		ut::LinkList<AnimationLink, offsetof(AnimationLink, AnimationLink::node)> m_Animations;
		#pragma GCC diagnostic pop
		ut::Color8 m_Colors[7] 
		{
			ut::Color8::Black(),
			ut::Color8::White(),
			ut::Color8::White(),
			ut::Color8::White(),
			ut::Color8::White(),
			ut::Color8::White(),
			ut::Color8::White()
		};
		u32 m_Capacity {};
		u32 m_Count {};
		void* m_pElements {};
		char m_Name[21] {};
		u8 m_Flags {};
	};
	ASSERT_SIZE(Material, 0x50);
}
