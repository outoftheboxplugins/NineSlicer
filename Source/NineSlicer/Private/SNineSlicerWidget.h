// Copyright Out-of-the-Box Plugins 2018-2025. All Rights Reserved.

#pragma once

#include <Widgets/SCompoundWidget.h>

class FNineSlicerInputProcessor;
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
class SNineSlicerWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNineSlicerWidget) {}
	SLATE_END_ARGS()

	/**
	 * Creates the slate widget that mirroring the currently selected widget with handles on top
	 */
	void Construct(const FArguments& InArgs, const TWeakPtr<FWidgetBlueprintEditor>& InBlueprintEditor);
	/**
	 * Destructor called when the widget is deleted
	 */
	virtual ~SNineSlicerWidget() override;
	/**
	 * Reset the current image's margins to the default value
	 */
	void ResetMargins();
	/**
	 * Returns the current error state of the nine slicer widget
	 */
	TOptional<FText> GetErrorState() const { return ErrorState; }

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
	void SetHandlePosition(EHandlePosition Handle, FVector2D InValue);
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
	// End SCompoundWidget interface

	/**
	 * Callback executed when the mouse button is down
	 */
	bool OnProcessorMouseButtonDown(const FPointerEvent& MouseEvent);
	/**
	 * Callback executed when the mouse button is up
	 */
	bool OnProcessorMouseButtonUp(const FPointerEvent& MouseEvent);
	/**
	 * Callback executed when the mouse moves
	 */
	bool OnProcessorMouseMove(const FPointerEvent& MouseEvent);
	/**
	 * Callback executed before the Slate app ticks
	 */
	void OnPreTick(float DeltaTime);
	/**
	 * Determines what type of cursor we should show depending on the widget state
	 */
	TOptional<EMouseCursor::Type> ComputeCursor() const;
	/**
	 * Checks for updates in the currently selected brush and updates the mirroring or the error state
	 */
	void UpdateBrushAndState();
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
	 * Setup error of the currently image, if any
	 */
	TOptional<FText> ErrorState;
	/**
	 * Main canvas containing the Image edited and the draggable handles
	 */
	TSharedPtr<SCanvas> ViewCanvas;
	/**
	 * Mirror brush based on the current selection
	 */
	FSlateBrush CurrentData;
	/**
	 * Input Preprocessor so we can handle events outside the widget
	 */
	TSharedPtr<FNineSlicerInputProcessor> InputProcessor;
};
