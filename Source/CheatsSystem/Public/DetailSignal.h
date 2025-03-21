#pragma once
#include "DebugVar.h"
#include "KeyShortcut.h"

namespace dbg
{

	class connection;

	namespace detail
	{
		class signal;
	}

	CHEATSSYSTEM_API connection _connect(const dbg::detail::signal& i_signal, std::function<void()> i_callback);
	CHEATSSYSTEM_API void _broadcast(const dbg::detail::signal& i_signal);

	namespace slate
	{
		CHEATSSYSTEM_API TSharedRef<DebugSlateWidget> make_widget(detail::signal& i_var);
	}

	class CHEATSSYSTEM_API connection
	{
	public:
		connection();
		connection(connection&&);
		~connection();

		void operator= (connection&&);

		void disconnect();

	private:
		friend CHEATSSYSTEM_API connection _connect(const dbg::detail::signal& i_signal, std::function<void()> i_callback);

		struct data;
		std::unique_ptr<data> m_data;
	};





	namespace detail
	{
		class CHEATSSYSTEM_API signal : public detail::variable
		{
		public:
			~signal();

		protected:
			friend CHEATSSYSTEM_API connection dbg::_connect(const dbg::detail::signal& i_signal, std::function<void()> i_callback);
			friend CHEATSSYSTEM_API void dbg::_broadcast(const dbg::detail::signal& i_signal);

			signal(std::filesystem::path i_name, std::optional<KeyShortcut> i_shortcut);
			signal(signal&& i_other);
			bool HandlesShortcut(const KeyShortcut& i_shortcut) const override;
			void Activate() override;


			//////////////////////////////////////////////////////////////////////////////
			//Members
			//////////////////////////////////////////////////////////////////////////////

			struct data;
			std::unique_ptr<data> m_data;
		};
	}

	inline void broadcast(const detail::signal& i_signal)
	{
		return _broadcast(i_signal);
	}
}