// Copyright Out-of-the-Box Plugins 2018-2025. All Rights Reserved.

#pragma once

#include <Widgets/SCompoundWidget.h>

class SNineSlicerWidget;
class FWidgetBlueprintEditor;

class SNineSlicerTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNineSlicerTab) {}
	SLATE_END_ARGS()

	/**
	 * Creates the tab containing the NineSlicer widget and the shortcut controls
	 */
	void Construct(const FArguments& InArgs, const TWeakPtr<FWidgetBlueprintEditor>& InBlueprintEditor);

private:
	/**
	 * Callback executed when when setting the color is set via a shortcut button
	 */
	FReply SetUserColor(FLinearColor Color);
	/**
	 * Callback executed when opening the plugin's settings
	 */
	FReply OpenSettings();
	/**
	 * Callback executed when reset the Brush's margins
	 */
	FReply ResetMargins();
	/**
	 * Inner widget displaying the mirror brush and editing handles
	 */
	TSharedPtr<SNineSlicerWidget> NineSlicerWidget;
};
