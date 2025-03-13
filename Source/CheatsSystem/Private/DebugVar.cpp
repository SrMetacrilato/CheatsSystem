#include "DebugVar.h"
#include "DebugSystem.h"

namespace dbg
{
	namespace detail
	{
		var::var()
		{
			DebugSystem::GetInstance().RegisterVariable(*this);
		}

		var::~var()
		{
			DebugSystem::GetInstance().UnregisterVariable(*this);
		}
	}
}
