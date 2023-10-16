// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BlueprintModes/WidgetBlueprintApplicationMode.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "ToolMenuSection.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class SCanvas;
class UBlueprintEditorToolMenuContext;

class FNineSlicerModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void HandleRegisterBlueprintEditorTab(const FWidgetBlueprintApplicationMode& ApplicationMode, FWorkflowAllowedTabSet& TabFactories);

	FWorkflowApplicationModeExtender Extender;
	TArray<TWeakPtr<FApplicationMode>> RegisteredApplicationModes;
};
