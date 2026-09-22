#pragma once

#include <core/libgarden/nw/lyt/Pane.hpp>


namespace nw::lyt
{
	void* Allocate(size_t);

	struct TexCoords
	{
		std::array<Vector2, 4> uv;
	};

	namespace internal
	{
		struct TexCoordAry
		{
			void Reserve(u8 capacity);

			u8 capacity {};
			u8 count {};
			TexCoords* data {};
		};
		ASSERT_SIZE(TexCoordAry, 8);
	}

	class Picture : public Pane
	{
	public:
		Picture(nn::math::VEC2 size, const char* name, const TexMap& texMap) : Pane {size, name}
		{
			m_TexCoords.Reserve(1);
			m_pMaterial = new (Allocate(sizeof(Material))) Material {};
			m_pMaterial->ReserveMem(1, 1, 1);
			Append(texMap);
		}

		virtual ~Picture() override;
		virtual Material* GetMaterial(size_t = 0) override;

		virtual void Append(const TexMap& texMap);

	private:
		bool m_TexCoordsUpdated = false;
		u32 m_TexCoordBufCount;
		Vector4 m_TexCoordBuf[6];
		Material* m_pMaterial;
		std::array<ut::Color8, 4> m_VertexColors {nw::ut::Color8::White(),
			nw::ut::Color8::White(), nw::ut::Color8::White(), nw::ut::Color8::White()};
		internal::TexCoordAry m_TexCoords;
	};
	ASSERT_SIZE(Picture, 0x158);
}
