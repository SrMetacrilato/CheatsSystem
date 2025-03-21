#pragma once
#include <filesystem>
#include "slate/SlateCapable.h"
#include "DebugVarBool.h"
class SCheckBox;
class STextBlock;

namespace boost::signals2
{
    class scoped_connection;
}

namespace dbg
{
	namespace slate
	{
        
        


        //A la API, amb fwd declarations
        //Leak per cicle de shareds??
        class CHEATSSYSTEM_API DebugSlateWidgetToggle : public DebugSlateWidget, public TSharedFromThis<DebugSlateWidgetToggle>
        {
        public:
            DebugSlateWidgetToggle(dbg::var<bool>& i_var);

            ~DebugSlateWidgetToggle();

            void OnStateChanged(ECheckBoxState i_state) const;

            void OnVariableValueChanged();

            TSharedRef<SWidget> Init(const FSlateFontInfo& i_parentWindowFontInfo) override;

        private:

            std::unique_ptr<boost::signals2::scoped_connection> m_valueChangedConnection;
            std::reference_wrapper<dbg::var<bool>> m_var;
            TSharedPtr<SCheckBox> checkbox;
        };

        class CHEATSSYSTEM_API DebugSlateWidgetBoolDisplay : public DebugSlateWidget, public TSharedFromThis<DebugSlateWidgetBoolDisplay>
        {
        public:
            DebugSlateWidgetBoolDisplay(dbg::var<bool>& i_var);

            ~DebugSlateWidgetBoolDisplay();

            void OnVariableValueChanged();

            TSharedRef<SWidget> Init(const FSlateFontInfo& i_parentWindowFontInfo) override;

        private:

            std::unique_ptr<boost::signals2::scoped_connection> m_valueChangedConnection;
            std::reference_wrapper<dbg::var<bool>> m_var;
            TSharedPtr<STextBlock> valueText;
        };

        struct BoolWidgetFactory
        {
            static TSharedRef<DebugSlateWidget> create(const properties<bool>::widget::Display& i_display, var<bool>& i_var)
            {
                return MakeShared<DebugSlateWidgetBoolDisplay>(i_var);
            }

            static TSharedRef<DebugSlateWidget> create (const properties<bool>::widget::Toggle& i_display, var<bool>& i_var)
            {
                return MakeShared<DebugSlateWidgetToggle>(i_var);
            }
        };

        template<>
        inline TSharedRef<DebugSlateWidget> make_widget<var<bool>>(var<bool>& i_var)
        {
            properties<bool> props = get_properties(i_var);
            return std::visit([&i_var](auto i_display) -> TSharedRef<DebugSlateWidget> 
                {
                    return BoolWidgetFactory::create(i_display, i_var);
                }, props.display);
        }
	}
}