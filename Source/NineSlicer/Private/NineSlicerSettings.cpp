// Copyright Out-of-the-Box Plugins 2018-2025. All Rights Reserved.

#include "NineSlicerSettings.h"

#include <ISettingsModule.h>

void UNineSlicerSettings::OpenSettings()
{
	const UNineSlicerSettings* Settings = GetDefault<UNineSlicerSettings>();

	ISettingsModule& SettingsModule = FModuleManager::LoadModuleChecked<ISettingsModule>("Settings");
	SettingsModule.ShowViewer(Settings->GetContainerName(), Settings->GetCategoryName(), Settings->GetSectionName());
}

FName UNineSlicerSettings::GetContainerName() const
{
	return TEXT("Editor");
}

FName UNineSlicerSettings::GetCategoryName() const
{
	return TEXT("Out-of-the-Box Plugins");
}

FName UNineSlicerSettings::GetSectionName() const
{
	return TEXT("Nine Slicer");
}

#if WITH_EDITOR
FText UNineSlicerSettings::GetSectionText() const
{
	const FName DisplaySectionName = GetSectionName();
	return FText::FromName(DisplaySectionName);
}
#endif