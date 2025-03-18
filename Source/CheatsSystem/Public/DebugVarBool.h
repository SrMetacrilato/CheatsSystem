#pragma once
#include "DebugVar.h"

namespace dbg
{
	template<>
	struct properties<bool>
	{

	};

	namespace slate
	{
		template<>
		CHEATSSYSTEM_API TSharedRef<DebugSlateWidget> make_widget<var<bool>>(var<bool>& i_var);
	}

	template class CHEATSSYSTEM_API var<bool>;
}