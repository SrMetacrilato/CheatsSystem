#include "DetailSignal.h"
#include "Common.h"


namespace dbg
{
	namespace detail
	{
		struct signal::data
		{
			data(std::optional<KeyShortcut> i_shortcut)
				: shortcut(std::move(i_shortcut))
			{

			}

			boost::signals2::signal<void()> signal;
			std::optional<KeyShortcut> shortcut;
		};
	}

	struct connection::data
	{
		boost::signals2::scoped_connection connection;
	};

	connection _connect(const dbg::detail::signal& i_signal, std::function<void()> i_callback)
	{
		boost::signals2::scoped_connection conn = i_signal.m_data->signal.connect(i_callback);
		connection result;
		result.m_data->connection = std::move(conn);
		return std::move(result);
	}
	
	void _broadcast(const dbg::detail::signal& i_signal)
	{
		i_signal.m_data->signal();
	}


	namespace detail
	{
		signal::~signal()
		{
		}

		signal::signal(std::filesystem::path i_name, std::optional<KeyShortcut> i_shortcut)
			: variable(std::move(i_name))
			, m_data(std::make_unique<data>(std::move(i_shortcut)))
		{
		}

		signal::signal(signal&& i_other)
			: variable(std::move(i_other))
			, m_data(std::move(i_other.m_data))
		{
		}

		bool signal::HandlesShortcut(const KeyShortcut& i_shortcut) const
		{
			return m_data->shortcut != std::nullopt && *m_data->shortcut == i_shortcut;
		}

		void signal::Activate()
		{
			broadcast(*this);
		}
	}

	connection::connection()
		: m_data(std::make_unique<data>())
	{
	}

	connection::connection(connection&& i_other)
		: m_data(std::move(i_other.m_data))
	{
	}

	connection::~connection()
	{
	}
	void connection::operator=(connection&& i_other)
	{
		m_data = std::move(i_other.m_data);
	}
	void connection::disconnect()
	{
		m_data = nullptr;
	}
}