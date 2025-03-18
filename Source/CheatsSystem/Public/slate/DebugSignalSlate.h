#pragma once
#include <filesystem>
#include "slate/SlateCapable.h"
#include "DebugSignal.h"

namespace dbg
{
	namespace slate
	{
        class DebugSlateWidgetButton : public DebugSlateWidget, public TSharedFromThis<DebugSlateWidgetButton>
        {
        public:
            DebugSlateWidgetButton(dbg::signal& i_var);

            TSharedRef<SWidget> Init(const FSlateFontInfo& i_parentWindowFontInfo) override;

        private:
            TSharedPtr<SButton> button;
            std::reference_wrapper<dbg::signal> m_var;
        };

        template<>
        inline TSharedRef<DebugSlateWidget> make_widget<signal>(signal& i_var)
        {
            return MakeShared<DebugSlateWidgetButton>(i_var);
        }

	}
}

