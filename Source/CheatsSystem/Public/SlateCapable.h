#pragma once

namespace dbg
{
	template<typename T>
	class var;
	class signal;
	
	class CHEATSSYSTEM_API DebugSlateWidget
	{
	public:
		
		virtual ~DebugSlateWidget() = default;

		//Required by some UI systems, such as Unreal's Slate. Will be called once, immediately after construction, and never again.
		//afterwards, widget will be accessed.
		virtual TSharedRef<SWidget> Init() = 0;

	};



	namespace slate
	{
		
		
		CHEATSSYSTEM_API TSharedRef<DebugSlateWidget> make_widget(var<bool>& i_var);
		CHEATSSYSTEM_API TSharedRef<DebugSlateWidget> make_widget(var<float>& i_var);
		CHEATSSYSTEM_API TSharedRef<DebugSlateWidget> make_widget(signal& i_var);

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