#pragma once
#include "DebugVar.h"
namespace dbg
{
	class signal;
	class connection;
	

	CHEATSSYSTEM_API signal make_signal(std::filesystem::path i_path);
	CHEATSSYSTEM_API connection connect(const signal& i_signal, std::function<void()> i_callback);
	CHEATSSYSTEM_API void broadcast(const signal& i_signal);

	namespace slate
	{
		CHEATSSYSTEM_API TSharedRef<DebugSlateWidget> make_widget(signal& i_var);
	}

	class CHEATSSYSTEM_API connection
	{
	public:
		connection();
		connection(connection&&);
		~connection();

	private:
		friend CHEATSSYSTEM_API connection connect(const signal& i_signal, std::function<void()> i_callback);

		struct data;
		std::unique_ptr<data> m_data;
	};


	#pragma warning( push )
	#pragma warning( disable : 4250) //Domination here is acceptable. signal will NEVER implement the function

	class CHEATSSYSTEM_API signal : public detail::var, public slate::slate_capable_impl<signal>
	{
	public:
		~signal();

	private:
		friend CHEATSSYSTEM_API signal make_signal(std::filesystem::path i_path);
		friend CHEATSSYSTEM_API connection connect(const signal& i_signal, std::function<void()> i_callback);
		friend CHEATSSYSTEM_API void dbg::broadcast(const signal& i_signal);

		signal(std::filesystem::path i_name);


		//////////////////////////////////////////////////////////////////////////////
		//Members
		//////////////////////////////////////////////////////////////////////////////

		struct data;
		std::unique_ptr<data> m_data;
	};

	#pragma warning( pop )


}