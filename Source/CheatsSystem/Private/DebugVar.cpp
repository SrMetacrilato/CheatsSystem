#include "DebugVar.h"
#include "DebugSystem.h"

namespace dbg
{
	namespace detail
	{
		var::var(std::filesystem::path i_path)
			: m_path(std::move(i_path))
		{
			DebugSystem::GetInstance().RegisterVariable(*this);
		}

		var::~var()
		{
			DebugSystem::GetInstance().UnregisterVariable(*this);
		}
	}

	CHEATSSYSTEM_API std::filesystem::path get_path(const detail::var& i_var)
	{
		return i_var.m_path;
	}
}
