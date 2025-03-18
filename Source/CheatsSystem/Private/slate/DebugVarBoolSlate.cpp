#include "slate/DebugVarBoolSlate.h"
#include "Common.h"
#include "DebugDashboardSlate.h"

namespace dbg
{
	namespace slate
	{
        ////////////////////////////////////////////////////////////////////////////////////////////////

        DebugSlateWidgetToggle::DebugSlateWidgetToggle(dbg::var<bool>& i_var)
            : m_var(i_var)
        {

        }
        DebugSlateWidgetToggle::~DebugSlateWidgetToggle()
        {

        }
        void DebugSlateWidgetToggle::OnStateChanged(ECheckBoxState i_state) const
        {
            boost::signals2::shared_connection_block block(*m_valueChangedConnection);
            dbg::set_value(m_var.get(), i_state == ECheckBoxState::Checked);
        }
        void DebugSlateWidgetToggle::OnVariableValueChanged()
        {
            bool value = dbg::value(m_var.get());
            checkbox->SetIsChecked(value);
        }
        TSharedRef<SWidget> DebugSlateWidgetToggle::Init(const FSlateFontInfo& i_parentWindowFontInfo)
        {
            m_valueChangedConnection = connectOnValueChanged(m_var, std::bind(&DebugSlateWidgetToggle::OnVariableValueChanged, this));
            std::filesystem::path name = get_path(m_var.get());
            bool value = dbg::value(m_var.get());
            checkbox = SNew(SCheckBox).OnCheckStateChanged(this, &DebugSlateWidgetToggle::OnStateChanged)
                ;
            checkbox->SetIsChecked(value);

            return SNew(SHorizontalBox)

                + SHorizontalBox::Slot().AutoWidth()[SNew(STextBlock).Text(FText::FromString(name.filename().c_str())).Font(i_parentWindowFontInfo)]
                + SHorizontalBox::Slot().AutoWidth()[SNew(SSpacer)].MaxWidth(10.f).MinWidth(10.f)
                + SHorizontalBox::Slot().AutoWidth()[checkbox.ToSharedRef()]
                
                ;

            return checkbox.ToSharedRef();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////

        DebugSlateWidgetBoolDisplay::DebugSlateWidgetBoolDisplay(dbg::var<bool>& i_var)
            : m_var(i_var)
        {
        }

        DebugSlateWidgetBoolDisplay::~DebugSlateWidgetBoolDisplay()
        {
        }

        void DebugSlateWidgetBoolDisplay::OnVariableValueChanged()
        {
            bool value = dbg::value(m_var.get());
            valueText->SetText(value ? FText::FromString("TRUE") : FText::FromString("FALSE"));
        }

        TSharedRef<SWidget> DebugSlateWidgetBoolDisplay::Init(const FSlateFontInfo& i_parentWindowFontInfo)
        {
            m_valueChangedConnection = connectOnValueChanged(m_var, std::bind(&DebugSlateWidgetBoolDisplay::OnVariableValueChanged, this));
            std::filesystem::path name = get_path(m_var.get());

            valueText = SNew(STextBlock).Font(i_parentWindowFontInfo);
            OnVariableValueChanged();
            return SNew(SHorizontalBox)

                + SHorizontalBox::Slot().AutoWidth()[SNew(STextBlock).Text(FText::FromString(name.filename().c_str())).Font(i_parentWindowFontInfo)]
                + SHorizontalBox::Slot().AutoWidth()[SNew(SSpacer)].MaxWidth(10.f).MinWidth(10.f)
                + SHorizontalBox::Slot().AutoWidth()[valueText.ToSharedRef()]
                ;
        }
    }
}