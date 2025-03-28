#pragma once
#include "Config.h"
#include "DetailVar.h"

namespace dbg
{
#if ENABLE_CHEATS

	//FWD declarations
	
	template<typename T> class var;
	
	template<typename T> dbg::var<T> make_var(T initialValue, std::filesystem::path i_name, properties<T> i_properties);
	template<typename T> T value(const dbg::var<T>&);
	template<typename T> void set_value(dbg::var<T>&, T i_value);
	template<typename T> properties<T> get_properties(const dbg::var<T>&);


	

	#pragma warning( push )
	#pragma warning( disable : 4250) //Domination here is acceptable. var will NEVER implement the function


	template<typename T>
	class var: public detail::variable, public slate::slate_capable_impl<var<T>>
	{
	
	public:
		inline var() = default;
		
		inline var(var<T>&& i_other)
			: variable(i_other)
			, m_properties(std::move(i_other.m_properties))
			, m_currentValue(std::move(i_other.m_currentValue))
		{
			TryRegister();
		}

		inline void operator= (var<T>&& i_other)
		{
			m_currentValue = std::move(i_other.m_currentValue);
			m_properties = std::move(i_other.m_properties);
			MakeEqual(std::move(i_other));
		}
		

	private:
		friend dbg::var<T> make_var<T>(T initialValue, std::filesystem::path i_name, properties<T> i_properties);
		friend T dbg::value(const dbg::var<T>&);
		friend void dbg::set_value(dbg::var<T>&, T i_value);
		friend properties<T> dbg::get_properties(const dbg::var<T>&);
		

		inline var(T initialValue, std::filesystem::path i_name, properties<T> i_properties)
			: detail::variable(std::move(i_name))
			, m_currentValue(std::move(initialValue))
			, m_properties(std::move(i_properties))
		{
			
		}

		inline T get() const
		{
			return m_currentValue;
		}

		inline void set(T i_value)
		{
			if (m_currentValue != i_value)
			{
				m_currentValue = std::move(i_value);
				NotifyChanged();
			}
		}

		inline bool HandlesShortcut(const KeyShortcut& i_shortcut) const override
		{
			return false;
		}

		inline void Activate() override
		{

		}
		


		//////////////////////////////////////////////////////////////////////////////
		//Members
		//////////////////////////////////////////////////////////////////////////////


		T m_currentValue;
		properties<T> m_properties;
	};

	#pragma warning( pop )

	template<typename T>
	inline dbg::var<T> make_var(T initialValue, std::filesystem::path i_name, properties<T> i_properties = properties<T>())
	{
		return var<T>(std::move(initialValue), std::move(i_name), std::move(i_properties));
	}

	template<typename T, typename... Params>
	inline dbg::var<T> make_var(T initialValue, std::filesystem::path i_name, Params... i_params)
	{
		return make_var<T>(std::move(initialValue), std::move(i_name), dbg::properties<T>(i_params...));
	}

	template<typename T>
	inline T value(const dbg::var<T>& i_var)
	{
		return i_var.get();
	}

	template<typename T>
	inline void set_value(dbg::var<T>& i_var, T i_value)
	{
		i_var.set(std::move(i_value));
	}

	template<typename T>
	inline properties<T> get_properties(const dbg::var<T>& i_var)
	{
		return i_var.m_properties;
	}

#else
	template<typename T>
	class var
	{
	public:
		constexpr var() {};
		constexpr var(const T i_value)
			: value(i_value)
		{

		}
		T value;
	};

	template<typename T, typename... Params>
	inline constexpr dbg::var<T> make_var(T initialValue, Params... i_params)
	{
		return dbg::var<T>(initialValue);
	}

	//This function is consteval (instead of contsexpr) because member debug variables
	//cannot be decaled constexpr, therefore, the compiler will not optimize them out,
	//therefore a cheating issue.
	//Do not use value con a member debug variable
	template<typename T>
	consteval inline T value(const dbg::var<T>& i_var)
	{
		return i_var.value;
	}

	template<typename T>
	inline void set_value(dbg::var<T>& i_var, T i_value)
	{
		
	}

	template<typename T>
	inline properties<T> get_properties(const dbg::var<T>& i_var)
	{
		return {};
	}

	template<typename T>
	inline std::filesystem::path get_path(const var<T>&)
	{
		return {};
	}

	template<typename T>
	inline std::unique_ptr<boost::signals2::scoped_connection> connectOnValueChanged(const var<T>&, std::function<void()>)
	{
		return nullptr;
	}

#endif
}

