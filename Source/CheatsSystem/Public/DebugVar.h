#pragma once
#include <filesystem>
#include <xutility>

#include "SlateCapable.h"

namespace boost
{
	namespace signals2
	{
		class scoped_connection;
	}
}

namespace dbg
{
	

	template<typename T>
	struct properties 
	{
	};

	template<>
	struct properties<bool>
	{
		
	};

	template<>
	struct properties<float>
	{
		properties() = default;
		properties(float i_min, float i_max)
			: min(i_min)
			, max(i_max)
		{ }

		float min = -999.f;
		float max = 999.f;
		int minFractionDigits = 2;
		int maxFractionDigits = 2;
	};

	//FWD declarations
	template<typename T> class var;
	namespace detail { class var; }
	
	template<typename T> dbg::var<T> make_var(T initialValue, std::filesystem::path i_name, properties<T> i_properties);
	template<typename T> T value(const dbg::var<T>&);
	template<typename T> void set_value(dbg::var<T>&, T i_value);
	template<typename T> properties<T> get_properties(const dbg::var<T>&);

	std::filesystem::path get_path(const detail::var&);

	
	namespace detail
	{
		class CHEATSSYSTEM_API var: virtual public slate::slate_capable
		{
		public:

			virtual ~var();
			

		protected:
			var(std::filesystem::path i_path);
			void NotifyChanged();

		private:
			friend std::filesystem::path dbg::get_path(const detail::var&);
			friend std::unique_ptr<boost::signals2::scoped_connection> connectOnValueChanged(const detail::var&, std::function<void()>); //unique_ptr so it is not dependant on boost
			struct Data;

			std::filesystem::path m_path;
			std::unique_ptr<Data> m_data;
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
		friend T dbg::value(const dbg::var<T>&);
		friend void dbg::set_value(dbg::var<T>&, T i_value);
		friend properties<T> dbg::get_properties(const dbg::var<T>&);
		

		inline var(T initialValue, std::filesystem::path i_name, properties<T> i_properties)
			: detail::var(std::move(i_name))
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


	//template class CHEATSSYSTEM_API var<bool>;
	//template class CHEATSSYSTEM_API var<float>;
}