#include "DetailVar.h"
#include "DebugSystem.h"
#include "KeyShortcut.h"
namespace dbg
{
	namespace detail
	{
		struct variable::Data
		{
			boost::signals2::signal<void()> onValueChanged;
		};

		variable::variable()
		{
		}

		variable::variable(std::filesystem::path i_path)
			: m_data(std::make_unique<Data>())
			, m_path(std::move(i_path))

		{
			DebugSystem::GetInstance().RegisterVariable(*this);
		}

		variable::variable(variable&& i_other)
			: m_path(std::move(i_other.m_path))
			, m_data(std::move(i_other.m_data))
		{
			if (!i_other.m_path.empty())
			{
				DebugSystem::GetInstance().UnregisterVariable(i_other);
			}
		}

		void variable::NotifyChanged()
		{
			m_data->onValueChanged();
		}

		void variable::MakeEqual(variable&& i_other)
		{
			if (!m_path.empty())
			{
				DebugSystem::GetInstance().UnregisterVariable(*this);
			}

			if (!i_other.m_path.empty())
			{
				DebugSystem::GetInstance().UnregisterVariable(i_other);
			}

			m_path = std::move(i_other.m_path);
			m_data = std::move(i_other.m_data);
			
			
			TryRegister();
		}

		void variable::TryRegister()
		{
			if (!m_path.empty())
			{
				DebugSystem::GetInstance().RegisterVariable(*this);
			}
		}

		

		variable::~variable()
		{
			if (!m_path.empty())
			{
				DebugSystem::GetInstance().UnregisterVariable(*this);
			}
		}

		std::unique_ptr <boost::signals2::scoped_connection > connectOnValueChanged(const detail::variable& i_var, std::function<void()> i_callback)
		{
			return std::make_unique<boost::signals2::scoped_connection>(i_var.m_data->onValueChanged.connect(i_callback));
		}
	}

	std::filesystem::path get_path(const detail::variable& i_var)
	{
		return i_var.m_path;
	}
}
