// Copyright Out-of-the-Box Plugins 2018-2023. All Rights Reserved.

#include "NineSlicerTabSummoner.h"

#include <WidgetBlueprintEditor.h>
#include <Widgets/Layout/SScaleBox.h>

#include "NineSlicerSettings.h"
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

FReply FNineSlicerSummoner::SetUserColor(FLinearColor Color) const
{
	UNineSlicerSettings* Settings = GetMutableDefault<UNineSlicerSettings>();
	Settings->DrawColor = Color;
	Settings->SaveConfig();

	return FReply::Handled();
}

FReply FNineSlicerSummoner::OpenSettings() const
{
	UNineSlicerSettings::OpenSettings();

	return FReply::Handled();
}

FReply FNineSlicerSummoner::ResetMargins() const
{
	// TODO: Implement this, maybe extra to a wrapper widget?

	return FReply::Handled();
}

TSharedRef<SWidget> FNineSlicerSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	// clang-format off
	return SNew(SBox)
		.Padding(FMargin(10))
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.OnClicked(this, &FNineSlicerSummoner::SetUserColor, FLinearColor::Red)
					[
						SNew(SImage)
						.ColorAndOpacity(FLinearColor::Red)
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.OnClicked(this, &FNineSlicerSummoner::SetUserColor, FLinearColor::Green)
					[
						SNew(SImage)
						.ColorAndOpacity(FLinearColor::Green)
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.OnClicked(this, &FNineSlicerSummoner::SetUserColor, FLinearColor::Blue)
					[
						SNew(SImage)
						.ColorAndOpacity(FLinearColor::Blue)
					]
				]

				+ SHorizontalBox::Slot()
				[
					SNew(SSpacer)
				]

				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.OnClicked(this, &FNineSlicerSummoner::ResetMargins)
					.ToolTip(TEXT("ResetMaringsTip", "Resets the brush's margins to 0,0,0,0"))
					[
						SNew(SImage)
						.Image(FAppStyle::GetBrush("Icons.Refresh"))
					]
				]

				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.OnClicked(this, &FNineSlicerSummoner::OpenSettings)
					.ToolTip(TEXT("OpenSettingsTip", "Opens the plugin's editor preferences"))
					[
						SNew(SImage)
						.Image(FAppStyle::Get().GetBrush("Icons.Settings"))
					]
				]
			]

			+ SVerticalBox::Slot()
			[
				SNew(SScaleBox)
				.Stretch(EStretch::ScaleToFit)
				[
					SNew(SNineSlicerTab, WeakWidgetBlueprintEditor)
				]
			]
		];
	// clang-format on
}

#undef LOCTEXT_NAMESPACE
