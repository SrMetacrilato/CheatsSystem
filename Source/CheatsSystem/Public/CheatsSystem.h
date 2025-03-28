// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Tickable.h"

namespace dbg::slate
{
	class DebugDashboardSlate;
}

class FCheatsSystemModule : public IModuleInterface, public FTickableGameObject
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	TStatId GetStatId() const override;

private:

	void OnBeginPIE(bool bIsSimulating);

	void OnEndPIE(bool bIsSimulating);
	


	TSharedPtr<dbg::slate::DebugDashboardSlate> m_debugDashboard;
	TSharedPtr<class SOverlay> m_menuWidgetContainer;
};
