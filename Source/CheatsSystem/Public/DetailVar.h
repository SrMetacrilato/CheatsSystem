#pragma once
#include "Config.h"

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

#if ENABLE_CHEATS
#define static_cheat static
#else
#define static_cheat constexpr static
#endif

namespace dbg
{
	struct KeyShortcut;
	class DebugSystem;

	template<typename T>
	struct properties
	{
	};


	namespace detail { class variable; }
	std::filesystem::path get_path(const detail::variable&);



	namespace detail
	{
		class CHEATSSYSTEM_API variable : virtual public slate::slate_capable
		{
		public:

			virtual ~variable();


		protected:
			variable();
			variable(std::filesystem::path i_path);
			variable(variable&& i_other);
			void NotifyChanged();
			void MakeEqual(variable&& i_other);
			void TryRegister();

		private:
			friend std::filesystem::path dbg::get_path(const detail::variable&);
			friend std::unique_ptr<boost::signals2::scoped_connection> connectOnValueChanged(const detail::variable&, std::function<void()>); //unique_ptr so it is not dependant on boost
			friend class dbg::DebugSystem;
			struct Data;

			virtual bool HandlesShortcut(const KeyShortcut&) const = 0;
			virtual void Activate() = 0;

			std::filesystem::path m_path;
			std::unique_ptr<Data> m_data;

		};
	}
}