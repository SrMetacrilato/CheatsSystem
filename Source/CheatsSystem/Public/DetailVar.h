#pragma once
#include <filesystem>
#include <xutility>

#include "slate/SlateCapable.h"

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


	namespace detail { class var; }
	std::filesystem::path get_path(const detail::var&);



	namespace detail
	{
		class CHEATSSYSTEM_API var : virtual public slate::slate_capable
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
}