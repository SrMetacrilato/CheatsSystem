#include "DebugDashboardSlate.h"
#include "DebugSystem.h"
#include "DebugVar.h"
#include "DebugSignal.h"
#include "Widgets/SWidget.h"

namespace dbg
{
    namespace slate
    {
        FSlateFontInfo DebugDashboardSlate::s_textStyle;

        //A la API, amb fwd declarations
        //Leak per cicle de shareds??
        class DebugSlateWidgetToggle: public DebugSlateWidget, public TSharedFromThis<DebugSlateWidgetToggle>
        {
        public:
            DebugSlateWidgetToggle(dbg::var<bool>& i_var)
                : m_var(i_var)
            {
                
            }

            ~DebugSlateWidgetToggle()
            {

            }

            void OnStateChanged(ECheckBoxState i_state) const
            {
                dbg::set_value(m_var.get(), i_state == ECheckBoxState::Checked);
            }

            TSharedRef<SWidget> Init() override
            {
                std::filesystem::path name = get_path(m_var.get());
                bool value = dbg::value(m_var.get());
                checkbox = SNew(SCheckBox).OnCheckStateChanged(this, &DebugSlateWidgetToggle::OnStateChanged)
                    [
                        SNew(STextBlock).Text(FText::FromString(name.filename().c_str())).Font(DebugDashboardSlate::s_textStyle)
                    ];
                checkbox->SetIsChecked(value);
                return checkbox.ToSharedRef();
            }

        private:
            

            std::reference_wrapper<dbg::var<bool>> m_var;
            TSharedPtr<SCheckBox> checkbox;
        };

        class DebugSlateWidgetButton : public DebugSlateWidget, public TSharedFromThis<DebugSlateWidgetButton>
        {
        public:
            TSharedRef<SWidget> Init() override
            {
                return SNew(SButton);
            }
        };

        class DebugSlateWidgetNumberInput : public DebugSlateWidget, public TSharedFromThis<DebugSlateWidgetNumberInput>
        {
        public:
            TSharedRef<SWidget> Init() override
            {
                return SNew(SButton);
            }
        };



        TSharedRef<DebugSlateWidget> make_widget(var<bool>& i_var)
        {
            return MakeShared<DebugSlateWidgetToggle>(i_var);
        }

        TSharedRef<DebugSlateWidget> make_widget(var<float>& i_var)
        {
            return MakeShared<DebugSlateWidgetButton>();
        }

        TSharedRef<DebugSlateWidget> make_widget(signal& i_var)
        {
            return MakeShared<DebugSlateWidgetButton>();
        }


        void DebugDashboardSlate::Construct(const FArguments& i_inArgs)
        {
            const FMargin ContentPadding = FMargin(60.f, 60.f);


            s_textStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
            s_textStyle.Size = 24.0f;

            m_widgetContainer = SNew(SVerticalBox);

            m_treeView = SNew(STreeView<TSharedPtr<SWidget>>)
                .ItemHeight(24)
                .TreeItemsSource(&m_treeNodes)
                .OnGenerateRow(this, &DebugDashboardSlate::OnGenerateListRow)
                .OnGetChildren(this, &DebugDashboardSlate::OnGetChildren);

            auto overlay = SNew(SOverlay)
                + SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
                [
                    SNew(SImage).ColorAndOpacity(FColor(0, 0, 0, 200))
                ]
                + SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Top).Padding(ContentPadding)
                [
                    SNew(SScrollBox) + SScrollBox::Slot()[
                        m_treeView.ToSharedRef()
                    ]
                ];


            ChildSlot.AttachWidget(overlay);

            Initialize();
        }

        DebugDashboardSlate::DebugDashboardSlate()
        {
            
        }

        DebugDashboardSlate::~DebugDashboardSlate()
        {

        }

        void slate::DebugDashboardSlate::Tick(float DeltaTime)
        {

        }

        bool DebugDashboardSlate::SupportsKeyboardFocus() const
        {
            return true;
        }

        void DebugDashboardSlate::Initialize()
        {
            m_onVarRegisteredConnection = DebugSystem::GetInstance().onVarRegistered.connect(std::bind(&DebugDashboardSlate::OnVarRegistered, this, std::placeholders::_1));
            m_onVarUnregisteredConnection = DebugSystem::GetInstance().onVarUnregistered.connect(std::bind(&DebugDashboardSlate::OnVarUnregistered, this, std::placeholders::_1));

            for (std::reference_wrapper<detail::var> var : DebugSystem::GetInstance().ListAllVars())
            {
                OnVarRegistered(var);
            }
        }

        void DebugDashboardSlate::OnVarRegistered(std::reference_wrapper<detail::var> i_var)
        {
            TSharedRef<DebugSlateWidget> holder = i_var.get().make_slate_widget();
            TSharedRef<SWidget> slateWidget = holder->Init();
            std::filesystem::path name = dbg::get_path(i_var);

            m_allDebugOptions.Emplace(std::move(holder));
            size_t i = 0;
            std::filesystem::path current;
            TSharedPtr<SWidget> parent;
            std::vector<std::pair<std::filesystem::path, TSharedPtr<SWidget>>> widgetChain;
            for (const auto& pathElement : name)
            {

                if (pathElement == "/")
                {
                    continue;
                }

                current += pathElement;
                std::string pathelementName = current.string();
                if (pathElement == name.filename()) //This is the debug option name
                {
                    widgetChain.emplace_back(name, slateWidget);
                }
                else
                {
                    auto it = m_widgetByPath.find(current.string());
                    if (it == m_widgetByPath.end())
                    {
                        parent = SNew(STextBlock).Text(FText::FromString(pathElement.c_str())).Font(s_textStyle);
                        m_widgetByPath.emplace(current.string(), parent.ToSharedRef());
                    }
                    else
                    {
                        parent = it->second;
                    }

                    widgetChain.emplace_back(current, parent);

                }
            }

            TSharedPtr<SWidget> parentWidget;
            for (const auto& widget : widgetChain)
            {
                if (parentWidget == nullptr)
                {
                    if (!m_treeNodes.Contains(widget.second))
                    {
                        m_treeNodes.Emplace(widget.second);
                    }
                }
                else
                {
                    if (!m_streeStructure.Find(parentWidget.ToSharedRef()))
                    {
                        m_streeStructure.Emplace(parent, { });
                    }
                    m_streeStructure[parentWidget.ToSharedRef()].Emplace(widget.first.c_str(), widget.second);

                }

                parentWidget = widget.second;
            }

            m_treeView->RequestTreeRefresh();
        }

        void DebugDashboardSlate::OnVarUnregistered(std::reference_wrapper<detail::var> i_var)
        {
        }

        bool DebugDashboardSlate::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
        {
            if (!GEngine->GameViewport)
            {
                return false;
            }

            if (InKeyEvent.GetKey() == EKeys::J && InKeyEvent.IsAltDown())
            {
                if (m_parent.Pin()->GetChildren()->Num() > 0)
                {
                    m_parent.Pin()->RemoveSlot(AsShared());
                    m_isVisible = false;
                    //FSlateApplication::Get().mouse(AsShared());
                }
                else
                {
                    m_parent.Pin()->AddSlot()[AsShared()];
                    GEngine->GameViewport->SetMouseLockMode(EMouseLockMode::LockOnCapture);
                    GEngine->GameViewport->SetIgnoreInput(false);
                    GEngine->GameViewport->SetHideCursorDuringCapture(false);
                    GEngine->GameViewport->SetMouseCaptureMode(EMouseCaptureMode::CaptureDuringMouseDown);
                    m_isVisible = true;
                    FSlateApplication::Get().SetKeyboardFocus(AsShared());

                }
                return true;
            }

            //for (const TSharedRef<DebugOption>& debugOption : m_allDebugOptions)
            //{
            //    if (InKeyEvent.GetKey() == debugOption->shortcut && InKeyEvent.IsAltDown())
            //    {
            //        debugOption->Interact();
            //    }
            //}

            return m_isVisible;
        }

        TSharedRef<ITableRow> DebugDashboardSlate::OnGenerateListRow(TSharedPtr<SWidget> Item, const TSharedRef<STableViewBase>& OwnerTable)
        {
            return
                SNew(STableRow< TSharedPtr<FString>>, OwnerTable)
                .Padding(2.0f)[
                    Item.ToSharedRef()
                ];
        }

        void DebugDashboardSlate::OnGetChildren(TSharedPtr<SWidget> Item, TArray<TSharedPtr<SWidget>>& OutChildren)
        {
            auto it = m_streeStructure.Find(Item);
            if (it)
            {
                it->GenerateValueArray(OutChildren);
                return;
            }
            else
            {

            }
        }

        void DebugDashboardSlate::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
        {

        }

    }
}