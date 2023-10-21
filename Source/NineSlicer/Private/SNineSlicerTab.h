// Copyright Out-of-the-Box Plugins 2018-2023. All Rights Reserved.

#pragma once

#include <Widgets/SCompoundWidget.h>

class FWidgetBlueprintEditor;
class UImage;
class SCanvas;

/**
 * Enum matching the FMargin variables for easier access
 */
enum class EHandlePosition
{
	Top,
	Left,
	Bottom,
	Right,
};

/**
 * Widget displaying an image with visual guides to aid the 9 slice process
 * Guides are interactable and moving them adjusts the margins
 */
class SNineSlicerTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNineSlicerTab) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<FWidgetBlueprintEditor> InBlueprintEditor);

private:
	UImage* GetCurrentImage() const;

	void AddSlot(TDelegate<FVector2D()> GetCoordinates);

	FVector2D AbsolutePositionToPercentage(const FVector2D& AbsolutePosition) const;
	FVector2D PercentageToAbsolutePosition(const FVector2D& Percentage) const;
	FVector2D GetHandlePosition(EHandlePosition Handle) const;

	void SetHandePosition(EHandlePosition Handle, FVector2D InValue);
	TOptional<EHandlePosition> GetClosestHandle(FVector2D MousePosition) const;

	// Begin SCompoundWidget interface
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
	// End SCompoundWidget interface

	TWeakPtr<FWidgetBlueprintEditor> WeakBlueprintEditor;
	TSharedPtr<SCanvas> ViewCanvas;
	FSlateBrush CurrentData;

	TOptional<EHandlePosition> HandleEdited;
	FScopedTransaction* ScopedTransaction = nullptr;
	float MaxGrabDistance = 0.001f;
};
