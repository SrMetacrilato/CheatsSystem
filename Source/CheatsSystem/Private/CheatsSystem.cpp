// Copyright Epic Games, Inc. All Rights Reserved.

#include "CheatsSystem.h"
#include "DebugDashboardSlate.h"

#define LOCTEXT_NAMESPACE "FCheatsSystemModule"

void FCheatsSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FCheatsSystemModule::ShutdownModule()
{
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
}

bool FCheatsSystemModule::IsTickable() const
{
	return m_debugDashboard == nullptr;
}

TStatId FCheatsSystemModule::GetStatId() const
{
	return TStatId();
}

void FCheatsSystemModule::StartPlayInEditorGameInstance()
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