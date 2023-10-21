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

	/**
	 * Creates the slate widget that mirroring the currently selected widget with handles on top
	 */
	void Construct(const FArguments& InArgs, const TWeakPtr<FWidgetBlueprintEditor>& InBlueprintEditor);

private:
	/**
	 * Draws an interactable handle at the position of the Handle in the main canvas
	 */
	void AddMarginHandle(EHandlePosition Handle);
	/**
	 * Returns the currently selected Image (if any) in the UMG editor
	 */
	UImage* GetCurrentImage() const;
	/**
	 * Returns the position of an interactable handle as percentage
	 */
	FVector2D GetHandlePosition(EHandlePosition Handle) const;
	/**
	 * Returns the closest to an absolute position if it's within the interaction range
	 */
	TOptional<EHandlePosition> GetClosestHandle(FVector2D AbsolutePosition) const;
	/**
	 * Updates the matching margin property based on an interactable handle
	 */
	void SetHandePosition(EHandlePosition Handle, FVector2D InValue);
	/**
	 * Transforms an absolute position into a percentage
	 */
	FVector2D AbsolutePositionToPercentage(const FVector2D& AbsolutePosition) const;

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

	/**
	 * Owner UMG blueprint editor instance
	 */
	TWeakPtr<FWidgetBlueprintEditor> WeakBlueprintEditor;
	/**
	 * Current margin adjustment edit action. Needed for undo/redo
	 */
	TSharedPtr<FScopedTransaction> ScopedTransaction;
	/**
	 * Handle that's currently grabbed with the mouse
	 */
	TOptional<EHandlePosition> HandleEdited;
	/**
	 * Main canvas containing the Image edited and the draggable handles
	 */
	TSharedPtr<SCanvas> ViewCanvas;
	/**
	 * Mirror brush based on the current selection
	 */
	FSlateBrush CurrentData;
};
