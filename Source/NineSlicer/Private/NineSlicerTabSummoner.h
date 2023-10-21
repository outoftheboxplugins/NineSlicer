// Copyright Out-of-the-Box Plugins 2018-2023. All Rights Reserved.

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
	/**
	 * Callback executed when when setting the color is set via a shortcut button
	 */
	FReply SetUserColor(FLinearColor Color) const;
	/**
	 * Callback executed when opening the plugin's settings
	 */
	FReply OpenSettings() const;
	/**
	 * Callback executed when reset the Brush marings
	 */
	FReply ResetMargins() const;
	// Begin FWorkflowTabFactory interface
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	// End FWorkflowTabFactory interface
	/**
	 * Weak pointer to the owning UMG blueprint editor of this tab
	 */
	TWeakPtr<FWidgetBlueprintEditor> WeakWidgetBlueprintEditor;
};
