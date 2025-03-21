#pragma once
#include <set>
#include "Common.h"
#include "DebugDashboard.h"
#include "Framework/Application/IInputProcessor.h"

namespace dbg
{
	namespace detail
	{
		class variable;
	}

	namespace slate
	{

		class DebugDashboardSlate : public DebugDashboard, public SCompoundWidget, public IInputProcessor
		{
		public:
			SLATE_BEGIN_ARGS(DebugDashboardSlate) {}
			SLATE_END_ARGS();


			void Construct(const FArguments& i_inArgs);
			DebugDashboardSlate();
			~DebugDashboardSlate();

			
			bool SupportsKeyboardFocus() const override;

			//Not to be changed, just by Module itself
			TWeakPtr<SOverlay> m_parent;

			bool IsVisible() const;

		private:
			FSlateFontInfo m_textStyle;
			void Initialize();

			void OnVarRegistered(std::reference_wrapper<detail::variable> i_var);
			void OnVarUnregistered(std::reference_wrapper<detail::variable> i_var);

			bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
			bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override { return m_isVisible; }

			TSharedRef<ITableRow> OnGenerateListRow(TSharedPtr<SWidget> Item, const TSharedRef<STableViewBase>& OwnerTable);
			void OnGetChildren(TSharedPtr<SWidget> Item, TArray<TSharedPtr<SWidget>>& OutChildren);

			void RemoveWidget(TSharedRef<SWidget> i_widget);
			TSharedPtr<SWidget> RemoveFromParent(TSharedRef<SWidget> i_widget);

			void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;
			bool m_isVisible = false;
			boost::signals2::scoped_connection m_onVarRegisteredConnection;
			boost::signals2::scoped_connection m_onVarUnregisteredConnection;

			TSharedPtr< SVerticalBox> m_widgetContainer;
			TSharedPtr< STreeView<TSharedPtr<SWidget>>> m_treeView;
			TArray<TSharedPtr<SWidget>> m_treeNodes;
			TMap<TSharedPtr<SWidget>, TSortedMap<FString, TSharedPtr<SWidget>>> m_streeStructure;
			std::map<std::string, TSharedRef<SWidget>> m_widgetByPath;
			//std::vector<std::weak_ptr<DebugWidgetEx>> m_activeWidgets;
			TMap<TSharedRef<SWidget>, TSharedRef<DebugSlateWidget>> m_allDebugOptions;

			struct InputState
			{
				EMouseLockMode lockMode;
				bool ignoreInput;
				bool hiddenDuringCapture;
				EMouseCaptureMode captureMode;
				TSharedPtr<SWidget> focus;
				TSharedPtr<SWidget> keyboardFocus;
				bool showMouseCursor;
			};

			InputState m_oldInputState;
		};
	}
}