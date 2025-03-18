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

        TSharedRef<SWidget> DebugSlateWidgetButton::Init(const FSlateFontInfo& i_parentWindowFontInfo)
        {
            std::filesystem::path name = get_path(m_var.get());
            button = SNew(SButton).OnPressed_Lambda([this]() {
                dbg::broadcast(m_var);
                })
                [
                    SNew(STextBlock).Text(FText::FromString("Activate")).Font(i_parentWindowFontInfo)
                ];
            return SNew(SHorizontalBox)

                + SHorizontalBox::Slot().AutoWidth()[SNew(STextBlock).Text(FText::FromString(name.filename().c_str())).Font(i_parentWindowFontInfo)]
                + SHorizontalBox::Slot().AutoWidth()[SNew(SSpacer)].MaxWidth(10.f).MinWidth(10.f)
                + SHorizontalBox::Slot().AutoWidth()[button.ToSharedRef()]
                ;
        }
    }
}

