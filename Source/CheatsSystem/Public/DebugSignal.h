#pragma once
#include "DetailSignal.h"


namespace dbg
{


#if ENABLE_CHEATS

	template<typename T>
	class sig;

	sig<void> make_signal(std::filesystem::path i_path, std::optional<KeyShortcut> i_shortcut = std::nullopt);

	#pragma warning( push )
	#pragma warning( disable : 4250) //Domination here is acceptable. signal will NEVER implement the function

	template<typename T>
	class sig : public detail::signal, public slate::slate_capable_impl<sig<T>>
	{
	private:
		
		sig(std::filesystem::path i_name, std::optional<KeyShortcut> i_shortcut)
			: signal(std::move(i_name), std::move(i_shortcut))
		{

		}

		friend sig<T> make_signal(std::filesystem::path i_path, std::optional<KeyShortcut> i_shortcut);

	};

	#pragma warning( pop )

	inline sig<void> make_signal(std::filesystem::path i_path, std::optional<KeyShortcut> i_shortcut)
	{
		return sig<void>(std::move(i_path), std::move(i_shortcut));
	}

	inline connection connect(const detail::signal& i_signal, std::function<void()> i_callback)
	{
		return _connect(i_signal, std::move(i_callback));
	}
	
	

#else

	template<typename T>
	class sig 
	{
	public:
		consteval sig() {}
	};

	template<typename... Params>
	constexpr inline sig<void> make_signal(Params... i_params)
	{
		return sig<void>();
	}

	inline void _broadcast(const dbg::detail::signal& i_signal)
	{

	}

	template<typename T>
	inline connection connect(const sig<T>& i_signal, std::function<void()> i_callback)
	{
		return {};
	}

	template<typename T>
	inline std::filesystem::path get_path(const sig<T>&)
	{
		return {};
	}

	template<typename T>
	inline void broadcast(const sig<T>&)
	{
		
	}


#endif

}