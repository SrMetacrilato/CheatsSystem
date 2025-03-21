// Copyright Epic Games, Inc. All Rights Reserved.

#include "CheatsSystem.h"
#include "slate/DebugDashboardSlate.h"

#define LOCTEXT_NAMESPACE "FCheatsSystemModule"

#if WITH_EDITOR
//In editor, plugins are dynamically linked, therefore, this operatio is needed
namespace boost {
	void throw_exception(std::exception const& e) {
		// Manejo de error personalizado (puedes hacer un log o un `check`)
		UE_LOG(LogTemp, Error, TEXT("Boost exception: %s"), *FString(e.what()));
	}
}
#endif

void FCheatsSystemModule::StartupModule()
{
#if WITH_EDITOR
	FEditorDelegates::BeginPIE.AddRaw(this, &FCheatsSystemModule::OnBeginPIE);
	FEditorDelegates::EndPIE.AddRaw(this, &FCheatsSystemModule::OnEndPIE);
#endif
}

void FCheatsSystemModule::ShutdownModule()
{
#if WITH_EDITOR
	FEditorDelegates::BeginPIE.RemoveAll(this);
	FEditorDelegates::EndPIE.RemoveAll(this);
#endif
	if (m_debugDashboard)
	{
		//FSlateApplication::Get().UnregisterInputPreProcessor(m_debugDashboard);
		m_debugDashboard = nullptr;
		m_menuWidgetContainer = nullptr;
	}
}

void FCheatsSystemModule::Tick(float DeltaTime)
{
	if (GEngine && GEngine->GameViewport && !m_debugDashboard)
	{
		m_menuWidgetContainer = SNew(SOverlay);
		m_debugDashboard = SNew(dbg::slate::DebugDashboardSlate);
		m_debugDashboard->m_parent = m_menuWidgetContainer;
		GEngine->GameViewport->AddViewportWidgetContent(m_menuWidgetContainer.ToSharedRef(), 9999);
		FSlateApplication::Get().RegisterInputPreProcessor(m_debugDashboard);
	}
	else if (GEngine && GEngine->GameViewport && m_debugDashboard)
	{
		GEngine->GameViewport->AddViewportWidgetContent(m_menuWidgetContainer.ToSharedRef(), 9999);
	}
}

bool FCheatsSystemModule::IsTickable() const
{
	return m_debugDashboard == nullptr;
}

TStatId FCheatsSystemModule::GetStatId() const
{
	return TStatId();
}

void FCheatsSystemModule::OnBeginPIE(bool bIsSimulating)
{

}

void FCheatsSystemModule::OnEndPIE(bool bIsSimulating)
{
	if (m_debugDashboard)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(m_debugDashboard);
		m_debugDashboard = nullptr;
		m_menuWidgetContainer = nullptr;
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCheatsSystemModule, CheatsSystem)