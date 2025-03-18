#pragma once
#include <filesystem>
#include "slate/SlateCapable.h"
#include "DebugVarFloat.h"

class SSlider;

namespace dbg
{
	namespace slate
	{

       


        class DebugSlateWidgetSliderInput : public DebugSlateWidget, public TSharedFromThis<DebugSlateWidgetSliderInput>
        {
        public:
            DebugSlateWidgetSliderInput(dbg::var<float>& i_var);

            ~DebugSlateWidgetSliderInput();

            void OnVariableValueChanged() const;

            TSharedRef<SWidget> Init() override;

        private:

            FNumberFormattingOptions format;
            std::reference_wrapper<dbg::var<float>> m_var;
            std::unique_ptr<boost::signals2::scoped_connection> m_valueChangedConnection;
            TSharedPtr<SSlider> slider;
            TSharedPtr<STextBlock> valueText;
        };

        template<>
        inline TSharedRef<DebugSlateWidget> make_widget<var<float>>(var<float>& i_var)
        {
            return MakeShared<DebugSlateWidgetSliderInput>(i_var);
        }
	}
}