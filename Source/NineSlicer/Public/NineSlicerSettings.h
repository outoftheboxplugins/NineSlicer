
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "NineSlicerSettings.generated.h"

/**
 * 
 */
UCLASS(config = EditorPerProjectUserSettings, defaultconfig)
class NINESLICER_API UNineSlicerSettings : public UDeveloperSettings 
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Nine Slicer")
	FLinearColor DrawColor = FLinearColor::Green;

	UPROPERTY(Config, EditAnywhere, Category="Nine Slicer", meta = (UIMin = "0", UIMax = "10"))
	int32 DecimalPrecision;
};
