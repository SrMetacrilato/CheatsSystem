#include "slate/DebugSignalSlate.h"
#include "DebugDashboardSlate.h"

namespace dbg
{
	namespace slate
	{
        DebugSlateWidgetButton::DebugSlateWidgetButton(dbg::signal& i_var)
            : m_var(i_var)
        {

        }

        TSharedRef<SWidget> DebugSlateWidgetButton::Init()
        {
            std::filesystem::path name = get_path(m_var.get());
            button = SNew(SButton).OnPressed_Lambda([this]() {
                dbg::broadcast(m_var);
                })
                [
                    SNew(STextBlock).Text(FText::FromString(name.filename().c_str())).Font(DebugDashboardSlate::s_textStyle)
                ];
            return button.ToSharedRef();
        }
    }
}

