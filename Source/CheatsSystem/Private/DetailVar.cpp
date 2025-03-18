#include "DetailVar.h"
#include "DebugSystem.h"
#include "KeyShortcut.h"
namespace dbg
{
	namespace detail
	{
		struct var::Data
		{
			boost::signals2::signal<void()> onValueChanged;
		};

		var::var(std::filesystem::path i_path)
			: m_data(std::make_unique<Data>())
			, m_path(std::move(i_path))

		{
			DebugSystem::GetInstance().RegisterVariable(*this);
		}

		void var::NotifyChanged()
		{
			m_data->onValueChanged();
		}

		

		var::~var()
		{
			DebugSystem::GetInstance().UnregisterVariable(*this);
		}

		std::unique_ptr <boost::signals2::scoped_connection > connectOnValueChanged(const detail::var& i_var, std::function<void()> i_callback)
		{
			return std::make_unique<boost::signals2::scoped_connection>(i_var.m_data->onValueChanged.connect(i_callback));
		}
	}

	std::filesystem::path get_path(const detail::var& i_var)
	{
		return i_var.m_path;
	}
}
