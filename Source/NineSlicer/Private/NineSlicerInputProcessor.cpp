// Copyright Out-of-the-Box Plugins 2018-2025. All Rights Reserved.

#include "NineSlicerInputProcessor.h"

void FNineSlicerInputProcessor::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
{
}

bool FNineSlicerInputProcessor::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	return OnMouseButtonDown.Execute(MouseEvent);
}

bool FNineSlicerInputProcessor::HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	return OnMouseButtonUp.Execute(MouseEvent);
}

bool FNineSlicerInputProcessor::HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	return OnMouseMove.Execute(MouseEvent);
}
