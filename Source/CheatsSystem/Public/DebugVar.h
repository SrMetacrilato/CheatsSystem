#pragma once
#include <filesystem>
#include <xutility>

#include "SlateCapable.h"

namespace dbg
{
	

	template<typename T>
	struct properties 
	{
	};

	template<>
	struct properties<bool>
	{
		//properties(const properties<bool>& other) = default;
	};

	template<typename T>
	class var;
	
	template<typename T>
	dbg::var<T> make_var(T initialValue, std::filesystem::path i_name, properties<T> i_properties);

	template<typename T>
	auto value(const dbg::var<T>&);

	
	namespace detail
	{
		class CHEATSSYSTEM_API var: virtual public slate::slate_capable
		{
		public:
			virtual ~var();
			

		protected:
			var();

		private:
			//Widget creation functions
			
		};
	}

	#pragma warning( push )
	#pragma warning( disable : 4250) //Domination here is acceptable. var will NEVER implement the function


	template<typename T>
	class var: public detail::var, public slate::slate_capable_impl<var<T>>
	{
	
	public:

		

	private:
		friend dbg::var<T> make_var<T>(T initialValue, std::filesystem::path i_name, properties<T> i_properties);
		friend auto value(const dbg::var<T>&);

		inline var(T initialValue, std::filesystem::path i_name, properties<T> i_properties)
			: m_currentValue(std::move(initialValue))
			, m_properties(std::move(i_properties))
		{
			
		}

		inline T value() const
		{
			return m_currentValue;
		}

		inline void set(T i_value)
		{
			m_currentValue = std::move(i_value);
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
		return var<T>(std::move(initialValue), std::move(i_name), dbg::properties<T>(i_params));
	}

	template<typename T>
	inline auto value(const dbg::var<T>& i_var)
	{
		return i_var.value();
	}


	//template class CHEATSSYSTEM_API var<bool>;
}