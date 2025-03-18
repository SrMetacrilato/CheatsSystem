#pragma once
#include "DebugVar.h"
#include <variant>

namespace dbg
{
	template<>
	struct properties<bool>
	{
		struct widget
		{
			struct Toggle
			{

			};

			struct Display
			{

			};
		};

		using TWidget = std::variant<widget::Toggle, widget::Display>;
		TWidget display;

		properties()
			: display(widget::Toggle())
		{ }

		properties(TWidget i_display)
			: display(std::move(i_display))
		{ }
	};

	namespace slate
	{
		template<>
		CHEATSSYSTEM_API TSharedRef<DebugSlateWidget> make_widget<var<bool>>(var<bool>& i_var);
	}

	template class CHEATSSYSTEM_API var<bool>;
}