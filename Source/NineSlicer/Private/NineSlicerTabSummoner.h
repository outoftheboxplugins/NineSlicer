// Copyright Out-of-the-Box Plugins 2018-2025. All Rights Reserved.

#pragma once

#include <WorkflowOrientedApp/WorkflowTabFactory.h>

class FWidgetBlueprintEditor;

/**
 * Tab summoner which creates the Nine Slicer tab inside of the UMG designer editor
 */
struct FNineSlicerSummoner : FWorkflowTabFactory
{
	explicit FNineSlicerSummoner(const TSharedPtr<FWidgetBlueprintEditor>& BlueprintEditor);

private:
	// Begin FWorkflowTabFactory interface
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	// End FWorkflowTabFactory interface
	/**
	 * Weak pointer to the owning UMG blueprint editor of this tab
	 */
	TWeakPtr<FWidgetBlueprintEditor> WeakWidgetBlueprintEditor;
};
