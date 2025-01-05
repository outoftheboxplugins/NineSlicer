// Copyright Out-of-the-Box Plugins 2018-2025. All Rights Reserved.

#include "NineSlicerTabSummoner.h"

#include <WidgetBlueprintEditor.h>

#include "SNineSlicerTab.h"

#define LOCTEXT_NAMESPACE "NineSlicer"

FNineSlicerSummoner::FNineSlicerSummoner(const TSharedPtr<FWidgetBlueprintEditor>& BlueprintEditor) :
	FWorkflowTabFactory(TEXT("NineSlicer"), BlueprintEditor), WeakWidgetBlueprintEditor(BlueprintEditor)
{
	TabLabel = LOCTEXT("NineSlicer", "Nine Slicer");
	TabIcon = FSlateIcon(FSlateIcon("FractureEditorStyle", "FractureEditor.Slice"));
	bIsSingleton = true;
	ViewMenuDescription = LOCTEXT("NineSlicerDescription", "Nine Slicer");
	ViewMenuTooltip = LOCTEXT("NineSlicerTip", "Visual aid for viewing and adjusting the 9 slice scaling");
}

TSharedRef<SWidget> FNineSlicerSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	return SNew(SNineSlicerTab, WeakWidgetBlueprintEditor);
}

#undef LOCTEXT_NAMESPACE
