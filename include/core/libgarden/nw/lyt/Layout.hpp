#pragma once

#include <core/libgarden/nw/lyt/Pane.hpp>
#include <core/libgarden/nw/lyt/Bounding.hpp>
#include <core/libgarden/nw/lyt/Picture.hpp>
#include <core/libgarden/nw/lyt/GroupContainer.hpp>
#include <core/libgarden/nw/lyt/AnimTransform.hpp>
#include <core/libgarden/nw/lyt/AnimResource.hpp>
#include <core/libgarden/nw/os/IAllocator.hpp>

namespace nw::lyt
{
	class Layout
	{
	public:
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Winvalid-offsetof"
		using Animations = ut::LinkList<AnimTransform, offsetof(AnimTransform, AnimTransform::m_Node)>;
		#pragma GCC diagnostic pop

		static void SetAllocator(os::IAllocator* allocator);
		static void SetDeviceMemoryAllocator(os::IAllocator* allocator);

		static void* AllocMemory(size_t size, u8 alignment = sizeof(void*));
		static void FreeMemory(void* ptr);

		Layout();
		virtual ~Layout();
		virtual bool Build(const void* resource, ResourceAccessor* resAcc);
		virtual AnimTransform* CreateAnimTransform();
		virtual AnimTransform* CreateAnimTransform(const void* resource, ResourceAccessor* resAcc);
		virtual AnimTransform* CreateAnimTransform(const AnimResource& resource, nw::lyt::ResourceAccessor* resAcc);

		Pane* GetRootPane() { return m_pRootPane; }
		GroupContainer* GetGroups() { return m_pGroups; }

		Pane* CreatePane(Pane& parent, nn::math::VEC2 size, const char* name)
		{
			return CreatePane<Pane>(parent, size, name);
		}

		Bounding* CreateBounding(Pane& parent, nn::math::VEC2 size, const char* name)
		{
			return CreatePane<Bounding>(parent, size, name);
		}

		Picture* CreatePicture(Pane& parent, nn::math::VEC2 size, const char* name, const TexMap& texMap)
		{
			return CreatePane<Picture>(parent, size, name, texMap);
		}

		Group* CreateGroup(const char* name)
		{
			Group* result = new (AllocMemory(sizeof(Group))) Group {name};
			m_pGroups->AppendGroup(result);
			return result;
		}

		Animations& GetAnimations() { return m_Animations; }

	private:
		template<typename T>
		T* CreatePane(Pane& parent, auto&&... args)
		{
			T* result = new (AllocMemory(sizeof(T))) T {std::forward<decltype(args)>(args)...};
			parent.AppendChild(result);
			return result;
		}

		Animations m_Animations;
		Pane* m_pRootPane;
		GroupContainer* m_pGroups;
		u8 m_Data2[0x8];
	};
	ASSERT_SIZE(Layout, 0x20);

	inline void* Allocate(size_t size)
	{
		return Layout::AllocMemory(size);
	}
}
