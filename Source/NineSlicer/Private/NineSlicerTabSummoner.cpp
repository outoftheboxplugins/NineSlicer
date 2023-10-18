// Copyright Epic Games, Inc. All Rights Reserved.

#include "NineSlicerTabSummoner.h"

#include "NineSlicerSettings.h"
#include "SNineSlicerTab.h"
#include "StatusBarSubsystem.h"
#include "WidgetBlueprintEditor.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Input/SSpinBox.h"

#define LOCTEXT_NAMESPACE "TabSummoner"

const FName FMVVMBindingSummoner::TabID(TEXT("MVVMTab"));

FMVVMBindingSummoner::FMVVMBindingSummoner(TSharedPtr<FWidgetBlueprintEditor> BlueprintEditor, bool bInIsDrawerTab) :
	FWorkflowTabFactory(TabID, BlueprintEditor), WeakWidgetBlueprintEditor(BlueprintEditor), bIsDrawerTab(bInIsDrawerTab)
{
	TabLabel = LOCTEXT("ViewBinding_ViewMenu_Label", "Nine Slicer");
	TabIcon = FSlateIcon(FSlateIcon("FractureEditorStyle", "FractureEditor.Slice"));

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("ViewBinding_ViewMenu_Desc", "Nine Slicer");
	ViewMenuTooltip = LOCTEXT("ViewBinding_ViewMenu_ToolTip", "Show the View Bindings tab");
}

FReply FMVVMBindingSummoner::SetUserColor(FLinearColor Color) const
{
	UNineSlicerSettings* Settings = GetMutableDefault<UNineSlicerSettings>();
	Settings->DrawColor = Color;
	Settings->SaveConfig();

	return FReply::Handled();
}

int32 FMVVMBindingSummoner::GetPrecision() const
{
	const UNineSlicerSettings* Settings = GetDefault<UNineSlicerSettings>();
	return Settings->DecimalPrecision;
}

void FMVVMBindingSummoner::OnPrecisionChanged(int32 InValue) const
{
	UNineSlicerSettings* Settings = GetMutableDefault<UNineSlicerSettings>();
	Settings->DecimalPrecision = InValue;
	Settings->SaveConfig();
}

TSharedRef<SWidget> FMVVMBindingSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
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
					SNew(SSpinBox<int32>)
						.MinValue(0)
						.MaxValue(10)
						.MinSliderValue(0)
						.MaxSliderValue(10)
						.Delta(1)
						.Value(this, &FMVVMBindingSummoner::GetPrecision)
						.OnValueChanged(this, &FMVVMBindingSummoner::OnPrecisionChanged)
				]
				
				+ SHorizontalBox::Slot()
				[
					SNew(SSpacer)
				]

				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.OnClicked(this, &FMVVMBindingSummoner::SetUserColor, FLinearColor::Red)
					[
						SNew(SImage)
						.ColorAndOpacity(FLinearColor::Red)
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.OnClicked(this, &FMVVMBindingSummoner::SetUserColor, FLinearColor::Green)
					[
						SNew(SImage)
						.ColorAndOpacity(FLinearColor::Green)
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.OnClicked(this, &FMVVMBindingSummoner::SetUserColor, FLinearColor::Blue)
					[
						SNew(SImage)
						.ColorAndOpacity(FLinearColor::Blue)
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
