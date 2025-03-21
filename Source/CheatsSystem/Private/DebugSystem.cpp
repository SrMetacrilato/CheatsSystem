#include "DebugSystem.h"
#include "DetailVar.h"

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

	void DebugSystem::RegisterVariable(detail::variable& i_var)
	{
		m_vars.emplace_back(i_var);
		onVarRegistered(i_var);
	}

	void DebugSystem::UnregisterVariable(detail::variable& i_var)
	{
		m_vars.erase(std::find_if(m_vars.begin(), m_vars.end(), [&i_var](const auto& var)
			{
				return &var.get() == &i_var;
			}));
		onVarUnregistered(i_var);
	}

	std::vector<std::reference_wrapper<detail::variable>> DebugSystem::ListAllVars() const
	{
		std::lock_guard guard(m_varListMutex);
		return m_vars;
	}

	void DebugSystem::HandleKeyPress(const KeyShortcut& i_shortcut)
	{
		for (std::reference_wrapper<detail::variable> var : m_vars)
		{
			if (var.get().HandlesShortcut(i_shortcut))
			{
				var.get().Activate();
			}
		}
	}
}