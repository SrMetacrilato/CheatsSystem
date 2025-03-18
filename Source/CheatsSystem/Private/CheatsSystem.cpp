// Copyright Epic Games, Inc. All Rights Reserved.

#include "CheatsSystem.h"
#include "slate/DebugDashboardSlate.h"

#define LOCTEXT_NAMESPACE "FCheatsSystemModule"

void FCheatsSystemModule::StartupModule()
{
	FEditorDelegates::BeginPIE.AddRaw(this, &FCheatsSystemModule::OnBeginPIE);
	FEditorDelegates::EndPIE.AddRaw(this, &FCheatsSystemModule::OnEndPIE);
}

void FCheatsSystemModule::ShutdownModule()
{
	FEditorDelegates::BeginPIE.RemoveAll(this);
	FEditorDelegates::EndPIE.RemoveAll(this);

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