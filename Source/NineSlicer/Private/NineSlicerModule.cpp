// Copyright Out-of-the-Box Plugins 2018-2025. All Rights Reserved.

#include "NineSlicerModule.h"

#include <BlueprintModes/WidgetBlueprintApplicationMode.h>
#include <BlueprintModes/WidgetBlueprintApplicationModes.h>
#include <UMGEditorModule.h>

#include "NineSlicerTabSummoner.h"

void FNineSlicerModule::HandleRegisterBlueprintEditorTab(const FWidgetBlueprintApplicationMode& ApplicationMode, FWorkflowAllowedTabSet& TabFactories)
{
	if (ApplicationMode.GetModeName() == FWidgetBlueprintApplicationModes::DesignerMode)
	{
		TabFactories.RegisterFactory(MakeShared<FNineSlicerSummoner>(ApplicationMode.GetBlueprintEditor()));
	}
}

void FNineSlicerModule::StartupModule()
{
	IUMGEditorModule& UMGEditorModule = FModuleManager::LoadModuleChecked<IUMGEditorModule>("UMGEditor");
	UMGEditorModule.OnRegisterTabsForEditor().AddRaw(this, &FNineSlicerModule::HandleRegisterBlueprintEditorTab);
}

void FNineSlicerModule::ShutdownModule()
{
	if (IUMGEditorModule* UMGEditorModule = FModuleManager::GetModulePtr<IUMGEditorModule>("UMGEditor"))
	{
		UMGEditorModule->OnRegisterLayoutExtensions().RemoveAll(this);
	}
}

IMPLEMENT_MODULE(FNineSlicerModule, NineSlicer)
