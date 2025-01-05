// Copyright Out-of-the-Box Plugins 2018-2025. All Rights Reserved.

#pragma once

#include <Engine/DeveloperSettings.h>

#include "NineSlicerSettings.generated.h"

/**
 * Holds the configurable properties for the Nine Slicer Window
 */
UCLASS(config = EditorPerProjectUserSettings)
class UNineSlicerSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/**
	 * Shortcut to open the project settings windows focused to this config
	 */
	static void OpenSettings();
	/**
	 * Color used to draw the interaction handles on top of the image
	 */
	UPROPERTY(Config, EditAnywhere, Category = "Nine Slicer")
	FLinearColor DrawColor = FLinearColor::Green;
	/**
	 * Decimal precision used when setting the Margin property
	 * @note Set at 0 to disable precision clamping
	 */
	UPROPERTY(Config, EditAnywhere, Category = "Nine Slicer", meta = (UIMin = "0", UIMax = "10"))
	int32 DecimalPrecision = 2;
	/**
	 * Changes the size of the draggable handle inside the preview window
	 */
	UPROPERTY(Config, EditAnywhere, Category = "Nine Slicer")
	float HandleSize = 10.0f;

private:
	// Begin UDeveloperSettings interface
	virtual FName GetContainerName() const override;
	virtual FName GetCategoryName() const override;
	virtual FName GetSectionName() const override;
#if WITH_EDITOR
	virtual FText GetSectionText() const override;
#endif
	// End UDeveloperSettings interface
};
