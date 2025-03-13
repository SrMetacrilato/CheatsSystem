#pragma once
#include "Common.h"
#include "DebugDashboard.h"

namespace dbg
{
	namespace detail
	{
		class var;
	}

	class DebugDashboardSlate: public DebugDashboard
	{
	public:
		DebugDashboardSlate();
		~DebugDashboardSlate();

	private:
		void Initialize();
		
		void OnVarRegistered(std::reference_wrapper<detail::var> i_var);
		void OnVarUnregistered(std::reference_wrapper<detail::var> i_var);

		boost::signals2::scoped_connection m_onVarRegisteredConnection;
		boost::signals2::scoped_connection m_onVarUnregisteredConnection;
	};
}