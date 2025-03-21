#pragma once
#include <filesystem>
#include "slate/SlateCapable.h"
#include "DebugVarFloat.h"

namespace boost::signals2
{
    class scoped_connection;
}

class SSlider;
class STextBlock;

namespace dbg
{
	namespace slate
	{

       


        class CHEATSSYSTEM_API DebugSlateWidgetSliderInput : public DebugSlateWidget, public TSharedFromThis<DebugSlateWidgetSliderInput>
        {
        public:
            DebugSlateWidgetSliderInput(dbg::var<float>& i_var, dbg::properties<float>::widget::Slider i_sliderProperties);

            ~DebugSlateWidgetSliderInput();

            void OnVariableValueChanged() const;

            TSharedRef<SWidget> Init(const FSlateFontInfo& i_parentWindowFontInfo) override;

        private:
            dbg::properties<float>::widget::Slider m_sliderProperties;
            FNumberFormattingOptions format;
            std::reference_wrapper<dbg::var<float>> m_var;
            std::unique_ptr<boost::signals2::scoped_connection> m_valueChangedConnection;
            TSharedPtr<SSlider> slider;
            TSharedPtr<STextBlock> valueText;
        };

        class CHEATSSYSTEM_API DebugSlateWidgetFloatDisplay : public DebugSlateWidget, public TSharedFromThis<DebugSlateWidgetFloatDisplay>
        {
        public:
            DebugSlateWidgetFloatDisplay(dbg::var<float>& i_var, dbg::properties<float>::widget::Display i_displayProperties);

            ~DebugSlateWidgetFloatDisplay();

            void OnVariableValueChanged();

            TSharedRef<SWidget> Init(const FSlateFontInfo& i_parentWindowFontInfo) override;

        private:
            dbg::properties<float>::widget::Display m_displayProperties;
            FNumberFormattingOptions format;
            std::unique_ptr<boost::signals2::scoped_connection> m_valueChangedConnection;
            std::reference_wrapper<dbg::var<float>> m_var;
            TSharedPtr<STextBlock> valueText;
        };


        struct FloatWidgetFactory
        {
            static TSharedRef<DebugSlateWidget> create(const properties<float>::widget::Display& i_display, var<float>& i_var)
            {
                return MakeShared<DebugSlateWidgetFloatDisplay>(i_var, i_display);
            }

            static TSharedRef<DebugSlateWidget> create(const properties<float>::widget::Slider& i_display, var<float>& i_var)
            {
                return MakeShared<DebugSlateWidgetSliderInput>(i_var, i_display);
            }
        };

        template<>
        inline TSharedRef<DebugSlateWidget> make_widget<var<float>>(var<float>& i_var)
        {
            properties<float> props = get_properties(i_var);
            return std::visit([&i_var](auto i_display) -> TSharedRef<DebugSlateWidget>
                {
                    return FloatWidgetFactory::create(i_display, i_var);
                }, props.display);
        }
	}
}