// Copyright Out-of-the-Box Plugins 2018-2025. All Rights Reserved.

#pragma once

class FWorkflowAllowedTabSet;
class FWidgetBlueprintApplicationMode;

/**
 * Nine Slicer module is responsible for registering the tab spawner inside the UMG editor
 */
class FNineSlicerModule : public IModuleInterface
{
	/**
	 * Callback executed when a UMG blueprint editor tab is opened.
	 */
	void HandleRegisterBlueprintEditorTab(const FWidgetBlueprintApplicationMode& ApplicationMode, FWorkflowAllowedTabSet& TabFactories);

	// Begin IModuleInterface interface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface interface
};
