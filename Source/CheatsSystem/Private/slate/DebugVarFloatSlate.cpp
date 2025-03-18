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
            : m_var(i_var)
            , m_sliderProperties(std::move(i_sliderProperties))
        {

        }

        DebugSlateWidgetSliderInput::~DebugSlateWidgetSliderInput()
        {

        }

        void DebugSlateWidgetSliderInput::OnVariableValueChanged() const
        {
            float value = dbg::value(m_var.get());
            valueText->SetText(FText::AsNumber(value, &format));
            slider->SetValue(value);
        }

        TSharedRef<SWidget> DebugSlateWidgetSliderInput::Init(const FSlateFontInfo& i_parentWindowFontInfo)
        {
            m_valueChangedConnection = connectOnValueChanged(m_var, std::bind(&DebugSlateWidgetSliderInput::OnVariableValueChanged, this));
            std::filesystem::path name = get_path(m_var.get());
            properties<float> properties = get_properties(m_var.get());
            valueText = SNew(STextBlock).Font(i_parentWindowFontInfo);
            format.SetMinimumFractionalDigits(m_sliderProperties.minFractionDigits);
            format.SetMaximumFractionalDigits(m_sliderProperties.maxFractionDigits);
            float value = dbg::value(m_var.get());

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

            slider->SetValue(value);
            valueText->SetText(FText::AsNumber(value, &format));

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
            : m_var(i_var)
            , m_displayProperties(std::move(i_displayProperties))
        {
        }

        DebugSlateWidgetFloatDisplay::~DebugSlateWidgetFloatDisplay()
        {
            float value = dbg::value(m_var.get());
            valueText->SetText(FText::AsNumber(value, &format));
        }

        void DebugSlateWidgetFloatDisplay::OnVariableValueChanged()
        {
            float value = dbg::value(m_var.get());
            valueText->SetText(FText::AsNumber(value, &format));
        }

        TSharedRef<SWidget> DebugSlateWidgetFloatDisplay::Init(const FSlateFontInfo& i_parentWindowFontInfo)
        {
            format.SetMinimumFractionalDigits(m_displayProperties.minFractionDigits);
            format.SetMaximumFractionalDigits(m_displayProperties.maxFractionDigits);

            m_valueChangedConnection = connectOnValueChanged(m_var, std::bind(&DebugSlateWidgetFloatDisplay::OnVariableValueChanged, this));
            std::filesystem::path name = get_path(m_var.get());
            properties<float> properties = get_properties(m_var.get());
            valueText = SNew(STextBlock).Font(i_parentWindowFontInfo);
            
            float value = dbg::value(m_var.get());

            valueText->SetText(FText::AsNumber(value, &format));

            return SNew(SHorizontalBox)

                + SHorizontalBox::Slot().AutoWidth()[SNew(STextBlock).Text(FText::FromString(name.filename().c_str())).Font(i_parentWindowFontInfo)]
                + SHorizontalBox::Slot().AutoWidth()[SNew(SSpacer)].MaxWidth(10.f).MinWidth(10.f)
                + SHorizontalBox::Slot().AutoWidth()[valueText.ToSharedRef()]
                
                ;
        }

    }
}