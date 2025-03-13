#pragma once

namespace dbg
{
	template<typename T>
	class var;
	class signal;
	class DebugWidget;

	CHEATSSYSTEM_API std::unique_ptr<DebugWidget> make_slate_widget_impl(const var<bool>& i_var);
	CHEATSSYSTEM_API std::unique_ptr<DebugWidget> make_slate_widget_impl(const var<float>& i_var);
	CHEATSSYSTEM_API std::unique_ptr<DebugWidget> make_slate_widget_impl(const signal& i_var);

	namespace slate
	{
		class slate_capable
		{
		public:
			virtual ~slate_capable() = default;
			virtual std::unique_ptr<DebugWidget> make_slate_widget() = 0;
		};

		template<typename VarT>
		class slate_capable_impl: virtual public slate_capable
		{
			//////////////////////////////////////////////////////////////////////////////
			//Widget creation functions
			//////////////////////////////////////////////////////////////////////////////

			inline std::unique_ptr<DebugWidget> make_slate_widget()
			{
				return make_slate_widget_impl(*static_cast<VarT*>(this));
			}
		};
	}
}