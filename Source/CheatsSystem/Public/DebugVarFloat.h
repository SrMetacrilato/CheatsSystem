#pragma once
#include <variant>
#include "DebugVar.h"

namespace dbg
{
	template<>
	struct properties<float>
	{
		struct widget
		{
			struct Slider
			{
				constexpr Slider(float i_min, float i_max)
					: min(i_min)
					, max(i_max)
				{

				}
				float min = -999.f;
				float max = 999.f;
				int minFractionDigits = 2;
				int maxFractionDigits = 2;
			};

			struct Display
			{
				int minFractionDigits = 2;
				int maxFractionDigits = 2;
			};
		};

		using TWidget = std::variant<widget::Slider, widget::Display>;
		TWidget display;

		inline properties()
			: display(widget::Slider(-999.f, 999.f))
		{

		}
		inline properties(TWidget i_display)
			: display(std::move(i_display))
		{
		}

		
	};
	

	namespace slate
	{
		TSharedRef<DebugSlateWidget> make_widget(var<float>& i_var);
	}

	//template class CHEATSSYSTEM_API var<float>;

}