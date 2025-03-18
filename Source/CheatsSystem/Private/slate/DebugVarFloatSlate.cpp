#include "slate/DebugVarFloatSlate.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Layout/SSpacer.h"
#include "Common.h"

#include "DebugDashboardSlate.h"

namespace dbg
{
	namespace slate
	{

       

        DebugSlateWidgetSliderInput::DebugSlateWidgetSliderInput(dbg::var<float>& i_var)
            : m_var(i_var)
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

        TSharedRef<SWidget> DebugSlateWidgetSliderInput::Init()
        {
            m_valueChangedConnection = connectOnValueChanged(m_var, std::bind(&DebugSlateWidgetSliderInput::OnVariableValueChanged, this));
            std::filesystem::path name = get_path(m_var.get());
            properties<float> properties = get_properties(m_var.get());
            valueText = SNew(STextBlock).Font(DebugDashboardSlate::s_textStyle);
            format.SetMinimumFractionalDigits(properties.minFractionDigits);
            format.SetMaximumFractionalDigits(properties.maxFractionDigits);
            float value = dbg::value(m_var.get());

            slider = SNew(SSlider)
                .OnValueChanged_Lambda([this](float i_value)
                    {
                        boost::signals2::shared_connection_block block(*m_valueChangedConnection);
                        dbg::set_value(m_var.get(), i_value);
                        valueText->SetText(FText::AsNumber(i_value, &format));
                    })
                .MinValue(properties.min)
                .MaxValue(properties.max)
                ;

            slider->SetValue(value);
            valueText->SetText(FText::AsNumber(value, &format));

            return SNew(SHorizontalBox)

                + SHorizontalBox::Slot().AutoWidth()[SNew(STextBlock).Text(FText::FromString(name.filename().c_str())).Font(DebugDashboardSlate::s_textStyle)]
                + SHorizontalBox::Slot().AutoWidth()[SNew(SSpacer)].MaxWidth(10.f)
                + SHorizontalBox::Slot().AutoWidth()[valueText.ToSharedRef()]
                + SHorizontalBox::Slot().AutoWidth()[SNew(SSpacer)].MaxWidth(10.f)
                + SHorizontalBox::Slot().FillWidth(1.0f)[slider.ToSharedRef()]
                ;
        }

    }
}