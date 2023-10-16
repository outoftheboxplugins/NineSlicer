// Copyright Epic Games, Inc. All Rights Reserved.

#include "NineSlicerTabSummoner.h"

#include "SNineSlicerTab.h"
#include "StatusBarSubsystem.h"
#include "WidgetBlueprintEditor.h"

#define LOCTEXT_NAMESPACE "TabSummoner"

const FName FMVVMBindingSummoner::TabID(TEXT("MVVMTab"));

FMVVMBindingSummoner::FMVVMBindingSummoner(TSharedPtr<FWidgetBlueprintEditor> BlueprintEditor, bool bInIsDrawerTab) :
	FWorkflowTabFactory(TabID, BlueprintEditor), WeakWidgetBlueprintEditor(BlueprintEditor), bIsDrawerTab(bInIsDrawerTab)
{
	TabLabel = LOCTEXT("ViewBinding_ViewMenu_Label", "Nine Slicer");
	TabIcon = FSlateIcon(FSlateIcon("FractureEditorStyle", "FractureEditor.Slice"));

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("ViewBinding_ViewMenu_Desc", "Nine Slicer");
	ViewMenuTooltip = LOCTEXT("ViewBinding_ViewMenu_ToolTip", "Show the View Bindings tab");
}

TSharedRef<SWidget> FMVVMBindingSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	return SNew(SNineSlicerTab, WeakWidgetBlueprintEditor.Pin());
}

#undef LOCTEXT_NAMESPACE
