// Copyright Epic Games, Inc. All Rights Reserved.

#include "NineSlicer.h"

#include "BlueprintModes/WidgetBlueprintApplicationModes.h"
#include "NineSlicerTabSummoner.h"
#include "ToolMenu.h"
#include "ToolMenuSection.h"
#include "ToolMenus.h"
#include "UMGEditorModule.h"
#include "WorkflowOrientedApp/ApplicationMode.h"

#define LOCTEXT_NAMESPACE "FNineSlicerModule"


void FNineSlicerModule::HandleRegisterBlueprintEditorTab(const FWidgetBlueprintApplicationMode& ApplicationMode, FWorkflowAllowedTabSet& TabFactories)
{
	if (ApplicationMode.GetModeName() == FWidgetBlueprintApplicationModes::DesignerMode)
	{
		TabFactories.RegisterFactory(MakeShared<FMVVMBindingSummoner>(ApplicationMode.GetBlueprintEditor()));
	}
}

void FNineSlicerModule::StartupModule()
{
	IUMGEditorModule& UMGEditorModule = FModuleManager::LoadModuleChecked<IUMGEditorModule>("UMGEditor");
	UMGEditorModule.OnRegisterTabsForEditor().AddRaw(this, &FNineSlicerModule::HandleRegisterBlueprintEditorTab);
}

void FNineSlicerModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FNineSlicerModule, NineSlicer)
