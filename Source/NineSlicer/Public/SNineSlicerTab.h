// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class FWidgetBlueprintEditor;
class UImage;
class SCanvas;

enum class EHandlePosition
{
	Top,
	Left,
	Bottom,
	Right,
};

/**
 *
 */
class NINESLICER_API SNineSlicerTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNineSlicerTab) {}
	SLATE_END_ARGS()

	UImage* GetCurrentImage() const;

	void AddSlot(TDelegate<FVector2D()> GetCoordinates);
	void Construct(const FArguments& InArgs, TSharedPtr<FWidgetBlueprintEditor> InBlueprintEditor);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	virtual int32 OnPaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled
	) const override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual TOptional<EMouseCursor::Type> GetCursor() const override;

	FVector2D AbsolutePositionToPercentage(const FVector2D& AbsolutePosition) const;
	FVector2D PercentageToAbsolutePosition(const FVector2D& Percentage) const;

	TWeakPtr<FWidgetBlueprintEditor> WeakBlueprintEditor;
	TSharedPtr<SCanvas> ViewCanvas;
	FSlateBrush CurrentData;
	TWeakPtr<class FAssetEditorToolkit> HostingApp;

	void SetHandePosition(EHandlePosition Handle, FVector2D InValue);
	FVector2D GetHandlePosition(EHandlePosition Handle) const;
	TOptional<EHandlePosition> GetClosestHandle(FVector2D MousePosition) const;

	TOptional<EHandlePosition> HandleEdited;
	FScopedTransaction* ScopedTransaction;
	float MaxGrabDistance = 0.001f;
};
