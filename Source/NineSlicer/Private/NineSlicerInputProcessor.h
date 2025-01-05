// Copyright Out-of-the-Box Plugins 2018-2025. All Rights Reserved.

#pragma once

#include <Framework/Application/IInputProcessor.h>

/**
 * Nine Slicer Input process that allows us to process event outside the widget space
 */
class FNineSlicerInputProcessor : public IInputProcessor
{
public:
	/**
	 * Common delegate type for subscribing to mouse events
	 */
	using FMouseEvent = TDelegate<bool(const FPointerEvent& MouseEvent)>;
	/**
	 * Delegated broadcast when the mouse button is down
	 */
	FMouseEvent OnMouseButtonDown;
	/**
	 * Delegated broadcast when the mouse button is up
	 */
	FMouseEvent OnMouseButtonUp;
	/**
	 * Delegated broadcast when the mouse moves
	 */
	FMouseEvent OnMouseMove;

private:
	// Begin IInputProcessor interface
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
	virtual bool HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
	virtual const TCHAR* GetDebugName() const override { return TEXT("FNineSlicerInputProcessor"); }
	// Begin IInputProcessor interface
};
