#pragma once
#include "Common.h"
#include <vector>
#include <mutex>

namespace dbg
{
	struct KeyShortcut;

	namespace detail
	{
		class var;
	}

	class DebugSystem
	{
	public:
		static DebugSystem& GetInstance();
		static void ClearInstance();
		
		DebugSystem();
		~DebugSystem();

		void RegisterVariable(detail::var& i_var);
		void UnregisterVariable(detail::var& i_var);

		boost::signals2::signal<void(detail::var& i_var)> onVarRegistered;
		boost::signals2::signal<void(detail::var& i_var)> onVarUnregistered;

		std::vector<std::reference_wrapper<detail::var>> ListAllVars() const;

		void HandleKeyPress(const KeyShortcut& i_shortcut);

	private:

		static std::unique_ptr<DebugSystem> s_debugSystem;
		mutable std::mutex m_varListMutex;

		std::vector<std::reference_wrapper<detail::var>> m_vars;
	};
}