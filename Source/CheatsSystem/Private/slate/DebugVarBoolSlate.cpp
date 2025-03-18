#include "slate/DebugVarBoolSlate.h"
#include "Common.h"
#include "DebugDashboardSlate.h"

namespace dbg
{
	namespace slate
	{
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
        TSharedRef<SWidget> DebugSlateWidgetToggle::Init()
        {
            m_valueChangedConnection = connectOnValueChanged(m_var, std::bind(&DebugSlateWidgetToggle::OnVariableValueChanged, this));
            std::filesystem::path name = get_path(m_var.get());
            bool value = dbg::value(m_var.get());
            checkbox = SNew(SCheckBox).OnCheckStateChanged(this, &DebugSlateWidgetToggle::OnStateChanged)
                [
                    SNew(STextBlock).Text(FText::FromString(name.filename().c_str())).Font(DebugDashboardSlate::s_textStyle)
                ];
            checkbox->SetIsChecked(value);
            return checkbox.ToSharedRef();
        }
    }
}