// Copyright Out-of-the-Box Plugins 2018-2025. All Rights Reserved.

#include "SNineSlicerTab.h"

#include <Widgets/Layout/SScaleBox.h>

#include "NineSlicerSettings.h"
#include "SNineSlicerWidget.h"

#define LOCTEXT_NAMESPACE "NineSlicer"

void SNineSlicerTab::Construct(const FArguments& InArgs, const TWeakPtr<FWidgetBlueprintEditor>& InBlueprintEditor)
{
	// clang-format off
	ChildSlot
	[
		SNew(SBox)
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
					.OnClicked(this, &SNineSlicerTab::SetUserColor, FLinearColor::Red)
					[
						SNew(SImage)
						.ColorAndOpacity(FLinearColor::Red)
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.OnClicked(this, &SNineSlicerTab::SetUserColor, FLinearColor::Green)
					[
						SNew(SImage)
						.ColorAndOpacity(FLinearColor::Green)
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.OnClicked(this, &SNineSlicerTab::SetUserColor, FLinearColor::Blue)
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
					.OnClicked(this, &SNineSlicerTab::ResetMargins)
					.ToolTipText(LOCTEXT("ResetMaringsTip", "Resets the brush's margins to 0,0,0,0"))
					[
						SNew(SImage)
						.Image(FAppStyle::GetBrush("Icons.Refresh"))
					]
				]

				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.OnClicked(this, &SNineSlicerTab::OpenSettings)
					.ToolTipText(LOCTEXT("OpenSettingsTip", "Opens the plugin's editor preferences"))
					[
						SNew(SImage)
						.Image(FAppStyle::Get().GetBrush("Icons.Settings"))
					]
				]
			]

			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text_Lambda([=, this](){ return NineSlicerWidget->GetErrorState().Get(FText::GetEmpty()); })
				.Justification(ETextJustify::Center)
				.Visibility_Lambda([=, this](){ return NineSlicerWidget->GetErrorState().IsSet() ? EVisibility::Visible : EVisibility::Collapsed; })
			]

			+ SVerticalBox::Slot()
			[
				SNew(SScaleBox)
				.Stretch(EStretch::ScaleToFit)
				[
					SAssignNew(NineSlicerWidget, SNineSlicerWidget, InBlueprintEditor)
				]
			]
		]
	];
	// clang-format on
}

FReply SNineSlicerTab::SetUserColor(FLinearColor Color)
{
	UNineSlicerSettings* Settings = GetMutableDefault<UNineSlicerSettings>();
	Settings->DrawColor = Color;
	Settings->SaveConfig();

	return FReply::Handled();
}

FReply SNineSlicerTab::OpenSettings()
{
	UNineSlicerSettings::OpenSettings();

	return FReply::Handled();
}

FReply SNineSlicerTab::ResetMargins()
{
	NineSlicerWidget->ResetMargins();

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE