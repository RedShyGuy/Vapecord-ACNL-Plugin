#pragma once

#include <core/libgarden/nw/lyt/PaneBase.hpp>
#include <core/libgarden/nw/lyt/AnimationLink.hpp>
#include <core/libgarden/nw/lyt/Material.hpp>
#include <core/libgarden/nw/lyt/Drawer.hpp>
#include <core/libgarden/nw/lyt/DrawInfo.hpp>
#include <core/libgarden/nn/math/Vector.hpp>

#include <cstring>
#include <utility>

namespace nw::lyt
{
	class Pane : public PaneBase
	{
	public:
		enum class OriginX : u8
		{
			LEFT,
			CENTER,
			RIGHT,
		};

		enum class OriginY : u8
		{
			TOP,
			CENTER,
			BOTTOM,
		};

		Pane(const void* res);

		constexpr Pane() = default;
		constexpr Pane(nn::math::VEC2 size) : m_Size {size} { }
		constexpr Pane(nn::math::VEC2 size, const char* name) : Pane {size}
		{
			strncpy(m_Name.data(), name, m_Name.size());
			m_Name.back() = 0;
		}

		virtual ~Pane() override;
		virtual void GetRuntimeTypeInfo();
		virtual void GetVertexColor();
		virtual void SetVertexColor();
		virtual void GetColorComponent();
		virtual void SetColorComponent();
		virtual void GetVertexColorComponent();
		virtual void SetVertexColorComponent();
		virtual void GetMaterialNum();
		virtual Material* GetMaterial(size_t = 0);
		virtual Pane* FindPane(const char* name, bool recursive = true);
		virtual Material* FindMaterial(const char* name, bool recursive = true);
		virtual void Animate(u32 options);
		virtual void AnimateSelf(u32 options);
		virtual void BindAnimation(class AnimTransform* anim, bool recursive, bool disable);
		virtual void UnbindAnimation(class AnimTransform* anim, bool recursive);
		virtual void Unk_44();
		virtual void UnbindAnimationSelf(class AnimTransform* anim);
		virtual void Unk_4c();
		virtual void Unk_50();
		virtual void Unk_54();
		virtual void Unk_58();
		virtual void CalculateMtx(const DrawInfo& info);
		virtual void Unk_60();
		virtual void Unk_64();
		virtual u32* MakeUniformDataSelf(u32* cmdbuf, DrawInfo& info, Drawer& drawer);
		virtual void Unk_6c();

		OriginX GetOriginX() const { return GetOriginX(m_Origin); }
		OriginY GetOriginY() const { return GetOriginY(m_Origin); }

		void SetOrigin(OriginX x, OriginY y) { m_Origin = EncodeOrigin(x, y); }

		void SetVisible(bool visible = true)
		{
			m_Flags = visible  ? m_Flags | VISIBLE : m_Flags & (u8)(~VISIBLE);
		}

		void SetInfluencedAlpha(bool set = true)
		{
			m_Flags = set  ? m_Flags | INFLUENCED_ALPHA : m_Flags & (u8)(~INFLUENCED_ALPHA);
		}

		bool IsVisible() const { return m_Flags & VISIBLE; }

		void SetMatrixDirty() { m_Flags &= 0xcf; }
		nn::math::VEC3& GetTranslation() { return m_Translation; }
		const nn::math::VEC3& GetTranslation() const { return m_Translation; }
		nn::math::VEC3 GetGlobalTranslation() const
		{ 
			return {m_GlobalMatrix.data[0][3], m_GlobalMatrix.data[1][3], m_GlobalMatrix.data[2][3]};
		}
		void SetTranslation(const nn::math::VEC3& translation) { m_Translation = translation; }

		nn::math::VEC3& GetRotation() { return m_Rotation; }

		const auto& GetGlobalMatrix() const { return m_GlobalMatrix; }

		void SetMatrix(const auto& matrix)
		{
			m_GlobalMatrix = matrix;
		}

		void SetAlpha(u8 value) { m_Alpha = value; }
		u8 GetAlpha() const { return m_Alpha; }

		auto& GetName() { return m_Name; }
		const auto& GetName() const { return m_Name; }

		Pane* GetParent() { return m_pParent; }
		void AppendChild(Pane* child);
		void RemoveChild(Pane* child);

	protected:

		u8 GetOrigin() const { return m_Origin; }

		OriginX GetOriginX(u8 value) const { return static_cast<OriginX>(value % 3u); }
		OriginY GetOriginY(u8 value) const { return static_cast<OriginY>(value / 3u); }

		u8 EncodeOrigin(OriginX x, OriginY y)
		{
			return static_cast<u8>(std::to_underlying(x) + std::to_underlying(y) * 3u);
		} 

	private:
		enum Flags : u8
		{
			VISIBLE = 1,
			INFLUENCED_ALPHA = 2,
			LOCATION_ADJUST = 4,
			USER_OWNED = 8,
		};

		Pane* m_pParent {};
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Winvalid-offsetof"
		ut::LinkList<Pane, offsetof(PaneBase, PaneBase::m_Node)> m_Children;
		ut::LinkList<AnimationLink, offsetof(AnimationLink, AnimationLink::node)> m_Animations;
		#pragma GCC diagnostic pop
		nn::math::VEC3 m_Translation {};
		nn::math::VEC3 m_Rotation {};
		nn::math::VEC2 m_Scale {1.0f, 1.0f};
		nn::math::VEC2 m_Size {};
		nn::math::MTX34 m_Matrix {};
		nn::math::MTX34 m_GlobalMatrix {};
		void* m_pUnk0 {};
		u8 m_Alpha {0xff};
		u8 m_GlobalAlpha {0xff};
		u8 m_Origin {EncodeOrigin(OriginX::LEFT, OriginY::TOP)};
		u8 m_Flags {VISIBLE};
		std::array<char, 17> m_Name {};
		std::array<char, 9> m_Data {};
		u8 padding[2] {};
	};
	ASSERT_SIZE(Pane, 0xd4);
}
