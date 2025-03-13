#include "DebugSignal.h"
#include "Common.h"


namespace dbg
{
	struct signal::data
	{
		boost::signals2::signal<void()> signal;
	};

	signal dbg::make_signal(std::filesystem::path i_path)
	{
		return signal();
	}

	struct connection::data
	{
		boost::signals2::scoped_connection connection;
	};

	connection connect(const signal& i_signal, std::function<void()> i_callback)
	{
		boost::signals2::scoped_connection conn = i_signal.m_data->signal.connect(i_callback);
		connection result;
		result.m_data->connection = std::move(conn);
		return std::move(result);
	}

	signal::~signal()
	{
	}

	dbg::signal::signal()
		: m_data(std::make_unique<data>())
	{
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
}