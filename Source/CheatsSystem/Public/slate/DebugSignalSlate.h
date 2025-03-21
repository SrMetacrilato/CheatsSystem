#pragma once
#include <filesystem>
#include "slate/SlateCapable.h"
#include "DebugSignal.h"

class SButton;
class SWidget;

namespace dbg
{
	namespace slate
	{
        class CHEATSSYSTEM_API DebugSlateWidgetButton : public DebugSlateWidget, public TSharedFromThis<DebugSlateWidgetButton>
        {
        public:
            DebugSlateWidgetButton(dbg::sig<void>& i_var);

            TSharedRef<SWidget> Init(const FSlateFontInfo& i_parentWindowFontInfo) override;

        private:
            TSharedPtr<SButton> button;
            std::reference_wrapper<dbg::sig<void>> m_var;
        };

        template<>
        inline TSharedRef<DebugSlateWidget> make_widget<sig<void>>(sig<void>& i_var)
        {
            return MakeShared<DebugSlateWidgetButton>(i_var);
        }

	}
}

