#include "DebugDashboardSlate.h"
#include "Widgets/SWidget.h"

#include "DebugSystem.h"
#include "KeyShortcut.h"
#include "DebugVar.h"

namespace dbg
{
    namespace slate
    {

        

       


        void DebugDashboardSlate::Construct(const FArguments& i_inArgs)
        {
            const FMargin ContentPadding = FMargin(60.f, 60.f);


            m_textStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
            m_textStyle.Size = 24.0f;

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

       

        bool DebugDashboardSlate::SupportsKeyboardFocus() const
        {
            return true;
        }

        bool DebugDashboardSlate::IsVisible() const
        {
            return m_isVisible;
        }

        void DebugDashboardSlate::Initialize()
        {
            m_onVarRegisteredConnection = DebugSystem::GetInstance().onVarRegistered.connect(std::bind(&DebugDashboardSlate::OnVarRegistered, this, std::placeholders::_1));
            m_onVarUnregisteredConnection = DebugSystem::GetInstance().onVarUnregistered.connect(std::bind(&DebugDashboardSlate::OnVarUnregistered, this, std::placeholders::_1));

            for (std::reference_wrapper<detail::variable> var : DebugSystem::GetInstance().ListAllVars())
            {
                OnVarRegistered(var);
            }
        }

        void DebugDashboardSlate::OnVarRegistered(std::reference_wrapper<detail::variable> i_var)
        {
            TSharedRef<DebugSlateWidget> holder = i_var.get().make_slate_widget();
            TSharedRef<SWidget> slateWidget = holder->Init(m_textStyle);
            std::filesystem::path name = dbg::get_path(i_var);

            m_allDebugOptions.Emplace(slateWidget, std::move(holder));
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

                //current += "/";
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
                        parent = SNew(STextBlock).Text(FText::FromString(pathElement.c_str())).Font(m_textStyle);
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
                        m_streeStructure.Emplace(parentWidget.ToSharedRef(), { });
                    }
                    m_streeStructure[parentWidget.ToSharedRef()].Emplace(widget.first.c_str(), widget.second);

                }

                parentWidget = widget.second;
            }

            m_treeView->RequestTreeRefresh();
        }

        void DebugDashboardSlate::OnVarUnregistered(std::reference_wrapper<detail::variable> i_var)
        {
            std::filesystem::path path = get_path(i_var.get());
            std::string name = path.string();
            TSharedPtr<SWidget> widgetToRemove;
            for (const auto& kv : m_allDebugOptions)
            {
                if (&kv.Value->variable.get() == &i_var.get())
                {
                    widgetToRemove = kv.Key;
                    break;
                }
            }
            if (!widgetToRemove)
            {
                return;
            }

            m_allDebugOptions.Remove(widgetToRemove.ToSharedRef());
            RemoveWidget(widgetToRemove.ToSharedRef());
            
            m_treeView->RequestTreeRefresh();
            
        }

        KeyShortcut Adapt(const FKeyEvent& InKeyEvent)
        {
            KeyShortcut shortcut;
            shortcut.altDown = InKeyEvent.IsAltDown();
            FString name = InKeyEvent.GetKey().ToString();
            if (name.Len() == 1)
            {
                wcstombs(&shortcut.key, &name[0], 1);
            }
            return shortcut;

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

                    UWorld* w = GEngine->GameViewport->GetWorld();
                    if (w)
                    {
                        APlayerController* PC = w->GetFirstPlayerController();
                        if (PC)
                        {
                            PC->bShowMouseCursor = m_oldInputState.showMouseCursor;
                            //PC->SetInputMode(FInputModeUIOnly());
                        }
                    }

                    GEngine->GameViewport->SetMouseLockMode(m_oldInputState.lockMode);
                    GEngine->GameViewport->SetIgnoreInput(m_oldInputState.ignoreInput);
                    GEngine->GameViewport->SetHideCursorDuringCapture(m_oldInputState.hiddenDuringCapture);
                    GEngine->GameViewport->SetMouseCaptureMode(m_oldInputState.captureMode);
                    FSlateApplication::Get().SetUserFocus(0, m_oldInputState.focus);
                    FSlateApplication::Get().SetKeyboardFocus(m_oldInputState.keyboardFocus);

                    m_isVisible = false;
                    //FSlateApplication::Get().mouse(AsShared());
                }
                else
                {
                    m_parent.Pin()->AddSlot()[AsShared()];

                    m_oldInputState.lockMode = GEngine->GameViewport->GetMouseLockMode();
                    m_oldInputState.ignoreInput = GEngine->GameViewport->IgnoreInput();
                    m_oldInputState.hiddenDuringCapture = GEngine->GameViewport->HideCursorDuringCapture();
                    m_oldInputState.captureMode = GEngine->GameViewport->GetMouseCaptureMode();
                    m_oldInputState.focus = FSlateApplication::Get().GetUserFocusedWidget(0);
                    m_oldInputState.keyboardFocus = FSlateApplication::Get().GetKeyboardFocusedWidget();
                   
                    UWorld* w = GEngine->GameViewport->GetWorld();
                    if (w)
                    {
                        APlayerController* PC = w->GetFirstPlayerController();
                        if (PC)
                        {
                            m_oldInputState.showMouseCursor = PC->bShowMouseCursor;
                            PC->bShowMouseCursor = true;
                        }
                    }

                    GEngine->GameViewport->SetMouseLockMode(EMouseLockMode::LockOnCapture);
                    GEngine->GameViewport->SetIgnoreInput(false);
                    GEngine->GameViewport->SetHideCursorDuringCapture(false);
                    GEngine->GameViewport->SetMouseCaptureMode(EMouseCaptureMode::CaptureDuringMouseDown);
                    FSlateApplication::Get().SetUserFocus(0, AsShared());
                    FSlateApplication::Get().SetKeyboardFocus(AsShared());

                    m_isVisible = true;
                    FSlateApplication::Get().SetKeyboardFocus(AsShared());

                }
                return true;
            }
            else
            {
                KeyShortcut shortcut = Adapt(InKeyEvent);
                if (shortcut.key != 0)
                {
                    DebugSystem::GetInstance().HandleKeyPress(shortcut);
                }
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

        void DebugDashboardSlate::RemoveWidget(TSharedRef<SWidget> i_widget)
        {
            m_treeNodes.Remove(i_widget);
            for (const auto& itByPath : m_widgetByPath)
            {
                if (itByPath.second == i_widget)
                {
                    m_widgetByPath.erase(itByPath.first);
                    break;
                }
            }

            TSharedPtr<SWidget> parent = RemoveFromParent(i_widget);
            if (parent && m_streeStructure[parent].Num() == 0)
            {
                RemoveWidget(parent.ToSharedRef());
            }
        }

        TSharedPtr<SWidget> DebugDashboardSlate::RemoveFromParent(TSharedRef<SWidget> i_widget)
        {
            for (auto& kv : m_streeStructure)
            {
                TSharedPtr<SWidget> parent = kv.Key;
                TSortedMap<FString, TSharedPtr<SWidget>>& subnodes = kv.Value;
                for (auto& itsubNodes : subnodes)
                {
                    if (itsubNodes.Value == i_widget)
                    {
                        subnodes.Remove(itsubNodes.Key);
                        return parent;
                    }
                }
            }

            return nullptr;
        }

        void DebugDashboardSlate::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
        {

        }

    }
}