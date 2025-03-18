#pragma once
#include <filesystem>
#include "slate/SlateCapable.h"
#include "DebugVarBool.h"
class SCheckBox;

namespace dbg
{
	namespace slate
	{
        
        


        //A la API, amb fwd declarations
        //Leak per cicle de shareds??
        class DebugSlateWidgetToggle : public DebugSlateWidget, public TSharedFromThis<DebugSlateWidgetToggle>
        {
        public:
            DebugSlateWidgetToggle(dbg::var<bool>& i_var);

            ~DebugSlateWidgetToggle();

            void OnStateChanged(ECheckBoxState i_state) const;

            void OnVariableValueChanged();

            TSharedRef<SWidget> Init() override;

        private:

            std::unique_ptr<boost::signals2::scoped_connection> m_valueChangedConnection;
            std::reference_wrapper<dbg::var<bool>> m_var;
            TSharedPtr<SCheckBox> checkbox;
        };

        template<>
        inline TSharedRef<DebugSlateWidget> make_widget<var<bool>>(var<bool>& i_var)
        {
            return MakeShared<DebugSlateWidgetToggle>(i_var);
        }
	}
}