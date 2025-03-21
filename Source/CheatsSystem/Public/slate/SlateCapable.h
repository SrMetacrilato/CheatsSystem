#pragma once

struct FSlateFontInfo;
namespace dbg
{
	template<typename T>
	class var;
	class signal;

	namespace detail
	{
		class variable;
	}

	class CHEATSSYSTEM_API DebugSlateWidget
	{
	public:
		
		virtual ~DebugSlateWidget() = default;

		//Required by some UI systems, such as Unreal's Slate. Will be called once, immediately after construction, and never again.
		//afterwards, widget will be accessed.
		virtual TSharedRef<SWidget> Init(const FSlateFontInfo& i_parentWindowFontInfo) = 0;

		std::reference_wrapper<detail::variable> variable;
	protected:
		inline DebugSlateWidget(detail::variable& i_var)
			: variable(i_var)
		{

		}

	};



	namespace slate
	{

		template<typename T>
		CHEATSSYSTEM_API TSharedRef<DebugSlateWidget> make_widget(T& i_var); //unimplemented

		class slate_capable
		{
		public:
			virtual ~slate_capable() = default;
			virtual TSharedRef<DebugSlateWidget> make_slate_widget() = 0;
		};

		template<typename VarT>
		class slate_capable_impl: virtual public slate_capable
		{
			//////////////////////////////////////////////////////////////////////////////
			//Widget creation functions
			//////////////////////////////////////////////////////////////////////////////

			inline TSharedRef<DebugSlateWidget> make_slate_widget()
			{
				return slate::make_widget(*static_cast<VarT*>(this));
			}
		};
	}
}