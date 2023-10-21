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

	// TODO: Check where this appear and set them to some sensible values
	ViewMenuDescription = LOCTEXT("NineSlicerDescription", "Nine Slicer Description");
	ViewMenuTooltip = LOCTEXT("NineSlicerTip", "Nine Slicer Tooltip");
}

FReply FNineSlicerSummoner::SetUserColor(FLinearColor Color) const
{
	// TODO: Check if this save works properly and where the file is actually saved on disk (this is a EditorPerProjectUserSettings, defaultconfig)
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

TSharedRef<SWidget> FNineSlicerSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	// clang-format off
	return SNew(SBox)
		.Padding(FMargin(5))
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
					.OnClicked(this, &FNineSlicerSummoner::OpenSettings)
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
					SNew(SNineSlicerTab, WeakWidgetBlueprintEditor.Pin())
				]
			]
		];
	// clang-format on
}

#undef LOCTEXT_NAMESPACE
