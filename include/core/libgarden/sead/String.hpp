#pragma once

#include <core/libgarden/nn/types.h>

namespace sead
{
	template<typename T>
	class SafeStringBase
	{
	public:
		constexpr SafeStringBase(const T* string) : m_pString(string)
		{
		}
		virtual constexpr ~SafeStringBase() = default;
		const T* GetString() const { return m_pString; }

	protected:
		virtual void assureTerminationImpl_() const;

		const T* m_pString;
	};

	template<typename T>
	class BufferedSafeStringBase : public SafeStringBase<T>
	{
	public:
		virtual ~BufferedSafeStringBase() override;

	protected:
		virtual void assureTerminationImpl_() const override;

		s32 m_Size;
	};


	template<typename T, s32 Length>
	class FixedSafeStringBase : public BufferedSafeStringBase<T>
	{
	public:

		T m_Buffer[Length];
	};

	template<s32 Length>
	class FixedSafeString final : public FixedSafeStringBase<char, Length>
	{

	};

	template<s32 Length>
	class WFixedSafeString final : public FixedSafeStringBase<char16, Length>
	{

	};

	extern template class FixedSafeString<24>;
	extern template class FixedSafeString<32>;
	extern template class FixedSafeString<64>;

	extern template class WFixedSafeString<18>;
	extern template class WFixedSafeString<32>;
	extern template class WFixedSafeString<64>;

	using SafeString = SafeStringBase<char>;
	using WSafeString = SafeStringBase<char16>;
	using BufferedSafeString = BufferedSafeStringBase<char>;
	using WBufferedSafeString = BufferedSafeStringBase<char16>;

}
