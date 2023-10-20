
#include "NineSlicerSettings.h"

#include "ISettingsModule.h"

void UNineSlicerSettings::OpenSettings()
{
	const UNineSlicerSettings* Settings = GetDefault<UNineSlicerSettings>();

	ISettingsModule& SettingsModule = FModuleManager::LoadModuleChecked<ISettingsModule>("Settings");
	SettingsModule.ShowViewer(Settings->GetContainerName(), Settings->GetCategoryName(), Settings->GetSectionName());
}