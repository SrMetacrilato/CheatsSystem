#pragma once
#include "DebugVar.h"

namespace dbg
{
	template<>
	struct properties<float>
	{
		inline properties() = default;
		inline properties(float i_min, float i_max)
			: min(i_min)
			, max(i_max)
		{
		}

		float min = -999.f;
		float max = 999.f;
		int minFractionDigits = 2;
		int maxFractionDigits = 2;
	};
	

	namespace slate
	{
		CHEATSSYSTEM_API TSharedRef<DebugSlateWidget> make_widget(var<float>& i_var);
	}

	template class CHEATSSYSTEM_API var<float>;

}