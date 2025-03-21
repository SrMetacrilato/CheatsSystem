#include "slate/DebugVarFloatSlate.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Layout/SSpacer.h"
#include "Common.h"

#include "DebugDashboardSlate.h"

namespace dbg
{
	namespace slate
	{

        ////////////////////////////////////////////////////////////////////////////////////////////////


        DebugSlateWidgetSliderInput::DebugSlateWidgetSliderInput(dbg::var<float>& i_var, dbg::properties<float>::widget::Slider i_sliderProperties)
            : DebugSlateWidget(i_var)
            , m_var(i_var)
            , m_sliderProperties(std::move(i_sliderProperties))
        {

        }

        DebugSlateWidgetSliderInput::~DebugSlateWidgetSliderInput()
        {

        }

        void DebugSlateWidgetSliderInput::OnVariableValueChanged() const
        {
#if ENABLE_CHEATS
            float value = dbg::value(m_var.get());
            valueText->SetText(FText::AsNumber(value, &format));
            slider->SetValue(value);
#endif
        }

        TSharedRef<SWidget> DebugSlateWidgetSliderInput::Init(const FSlateFontInfo& i_parentWindowFontInfo)
        {
            m_valueChangedConnection = connectOnValueChanged(m_var.get(), std::bind(&DebugSlateWidgetSliderInput::OnVariableValueChanged, this));
            std::filesystem::path name = get_path(m_var.get());
            properties<float> properties = get_properties(m_var.get());
            valueText = SNew(STextBlock).Font(i_parentWindowFontInfo);
            format.SetMinimumFractionalDigits(m_sliderProperties.minFractionDigits);
            format.SetMaximumFractionalDigits(m_sliderProperties.maxFractionDigits);

            slider = SNew(SSlider)
                .OnValueChanged_Lambda([this](float i_value)
                    {
                        boost::signals2::shared_connection_block block(*m_valueChangedConnection);
                        dbg::set_value(m_var.get(), i_value);
                        valueText->SetText(FText::AsNumber(i_value, &format));
                    })
                .MinValue(m_sliderProperties.min)
                .MaxValue(m_sliderProperties.max)
                ;
#if ENABLE_CHEATS
            float value = dbg::value(m_var.get());
            slider->SetValue(value);
            valueText->SetText(FText::AsNumber(value, &format));
#endif

            return SNew(SHorizontalBox)

                + SHorizontalBox::Slot().AutoWidth()[SNew(STextBlock).Text(FText::FromString(name.filename().c_str())).Font(i_parentWindowFontInfo)]
                + SHorizontalBox::Slot().AutoWidth()[SNew(SSpacer)].MaxWidth(10.f).MinWidth(10.f)
                + SHorizontalBox::Slot().AutoWidth()[valueText.ToSharedRef()]
                + SHorizontalBox::Slot().AutoWidth()[SNew(SSpacer)].MaxWidth(10.f).MinWidth(10.f)
                + SHorizontalBox::Slot().FillWidth(1.0f)[slider.ToSharedRef()]
                ;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////


        DebugSlateWidgetFloatDisplay::DebugSlateWidgetFloatDisplay(dbg::var<float>& i_var, dbg::properties<float>::widget::Display i_displayProperties)
            : DebugSlateWidget(i_var)
            , m_var(i_var)
            , m_displayProperties(std::move(i_displayProperties))
        {
        }

        DebugSlateWidgetFloatDisplay::~DebugSlateWidgetFloatDisplay()
        {
#if ENABLE_CHEATS
            float value = dbg::value(m_var.get());
            valueText->SetText(FText::AsNumber(value, &format));
#endif
        }

        void DebugSlateWidgetFloatDisplay::OnVariableValueChanged()
        {
#if ENABLE_CHEATS
            float value = dbg::value(m_var.get());
            valueText->SetText(FText::AsNumber(value, &format));
#endif
        }

        TSharedRef<SWidget> DebugSlateWidgetFloatDisplay::Init(const FSlateFontInfo& i_parentWindowFontInfo)
        {
            format.SetMinimumFractionalDigits(m_displayProperties.minFractionDigits);
            format.SetMaximumFractionalDigits(m_displayProperties.maxFractionDigits);

            m_valueChangedConnection = connectOnValueChanged(m_var.get(), std::bind(&DebugSlateWidgetFloatDisplay::OnVariableValueChanged, this));
            std::filesystem::path name = get_path(m_var.get());
            properties<float> properties = get_properties(m_var.get());
            valueText = SNew(STextBlock).Font(i_parentWindowFontInfo);
#if ENABLE_CHEATS
            float value = dbg::value(m_var.get());
            valueText->SetText(FText::AsNumber(value, &format));
#endif
            return SNew(SHorizontalBox)

                + SHorizontalBox::Slot().AutoWidth()[SNew(STextBlock).Text(FText::FromString(name.filename().c_str())).Font(i_parentWindowFontInfo)]
                + SHorizontalBox::Slot().AutoWidth()[SNew(SSpacer)].MaxWidth(10.f).MinWidth(10.f)
                + SHorizontalBox::Slot().AutoWidth()[valueText.ToSharedRef()]
                
                ;
        }

    }
}