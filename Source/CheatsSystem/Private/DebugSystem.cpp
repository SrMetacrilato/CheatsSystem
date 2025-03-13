#include "DebugSystem.h"

namespace dbg
{
	std::unique_ptr<DebugSystem> DebugSystem::s_debugSystem = nullptr;
	
	DebugSystem& DebugSystem::GetInstance()
	{
		if (s_debugSystem == nullptr)
		{
			s_debugSystem = std::make_unique<DebugSystem>();
		}
		return *s_debugSystem;
	}

	void DebugSystem::ClearInstance()
	{
		s_debugSystem = nullptr;
	}

	DebugSystem::DebugSystem()
	{
	}

	DebugSystem::~DebugSystem()
	{
	}

	void DebugSystem::RegisterVariable(detail::var& i_var)
	{
		m_vars.emplace_back(i_var);
		onVarRegistered(i_var);
	}

	void DebugSystem::UnregisterVariable(detail::var& i_var)
	{
		onVarUnregistered(i_var);
	}

	std::vector<std::reference_wrapper<detail::var>> DebugSystem::ListAllVars() const
	{
		std::lock_guard guard(m_varListMutex);
		return m_vars;
	}
}