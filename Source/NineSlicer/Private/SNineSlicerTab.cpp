// Fill out your copyright notice in the Description page of Project Settings.


#include "SNineSlicerTab.h"

#include "Components/Image.h"
#include "ObjectEditorUtils.h"
#include "SlateOptMacros.h"
#include "WidgetBlueprintEditor.h"
#include "WidgetReference.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/SCanvas.h"

void SNineSlicerTab::AddSlot(TDelegate<FVector2D()> GetCoordinates)
{
	// clang-format off
	SCanvas::FScopedWidgetSlotArguments NewSlot = ViewCanvas->AddSlot();
	NewSlot
		.Size(FVector2D(1, 1))
		.Position_Lambda( [=, this]()
			{
				const FVector2D Offset = {0.5, 0.5};
				
				const FVector2D Coords = GetCoordinates.Execute();
				const FVector2D CanvasSize = ViewCanvas->GetCachedGeometry().GetLocalSize();
				return FVector2D(CanvasSize.X * Coords.X, CanvasSize.Y * Coords.Y) -  Offset;
			})
		[
			// TODO: This is pivoted at top right, replace image or pivot it in the middle
			SNew(SImage)
			.Image(FAppStyle::Get().GetBrush("UMGEditor.TransformHandle"))
			.ColorAndOpacity(FLinearColor::Red)
		];
	// clang-format on
}


UImage* SNineSlicerTab::GetCurrentImage() const
{
	const TSharedPtr<FWidgetBlueprintEditor> WidgetBlueprintEditor = WeakBlueprintEditor.Pin();
	if (!WidgetBlueprintEditor)
	{
		return nullptr;
	}

	const TSet<FWidgetReference>& SelectedWidgets = WidgetBlueprintEditor->GetSelectedWidgets();
	if (SelectedWidgets.Num() != 1)
	{
		return nullptr;
	}

	FWidgetReference SelectedWidget = SelectedWidgets.Array()[0];

	UWidget* Widget = SelectedWidget.GetTemplate();
	return Cast<UImage>(Widget);
}

void SNineSlicerTab::Construct(const FArguments& InArgs, TSharedPtr<FWidgetBlueprintEditor> InBlueprintEditor)
{
	WeakBlueprintEditor = InBlueprintEditor;
	TSharedPtr<FAssetEditorToolkit> AssetEditor = HostingApp.Pin();

	// clang-format off
	ChildSlot
	[
		SNew(SScaleBox)
		.Stretch(EStretch::ScaleToFit)
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(SImage)
				.Image(&CurrentData)
			]

			+SOverlay::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SAssignNew(ViewCanvas, SCanvas)
			]
		]
	];
	// clang-format on

	AddSlot(TDelegate<FVector2D()>::CreateLambda(
		[this]()
		{
			return GetHandlePosition(EHandlePosition::Top);
		}
	));
	AddSlot(TDelegate<FVector2D()>::CreateLambda(
		[this]()
		{
			return GetHandlePosition(EHandlePosition::Left);
		}
	));
	AddSlot(TDelegate<FVector2D()>::CreateLambda(
		[this]()
		{
			return GetHandlePosition(EHandlePosition::Bottom);
		}
	));
	AddSlot(TDelegate<FVector2D()>::CreateLambda(
		[this]()
		{
			return GetHandlePosition(EHandlePosition::Right);
		}
	));
}

void SNineSlicerTab::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (const UImage* Image = GetCurrentImage())
	{
		CurrentData = Image->GetBrush();
		CurrentData.DrawAs = ESlateBrushDrawType::Image;

		// Only switch when the focus switches and if the margin is 0,0,0,0 default it to 0.1, 0.1, 0.9, 0.9
	}
}

int32 SNineSlicerTab::OnPaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled
) const
{
	SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	// TODO: Find a better workaround
	LayerId = LayerId + 100000;

	const FVector2D CanvasSize = ViewCanvas->GetCachedGeometry().GetAbsoluteSize();

	const FVector2D WidgetPosition = AllottedGeometry.GetAbsolutePosition();
	const FVector2D CanvasPosition = ViewCanvas->GetPaintSpaceGeometry().GetAbsolutePosition();
	const FVector2D WidgetToCanvas = -WidgetPosition + CanvasPosition;

	const FVector2D UpperLeftCornerPercentage = FVector2D(GetHandlePosition(EHandlePosition::Left).X, GetHandlePosition(EHandlePosition::Top).Y);
	const FVector2D UpperLeftCornerCanvasSpace = FVector2D(CanvasSize.X * UpperLeftCornerPercentage.X, CanvasSize.Y * UpperLeftCornerPercentage.Y);
	const FVector2D UpperLeftCornerWidgetSpace = UpperLeftCornerCanvasSpace + WidgetToCanvas;

	const FVector2D BottomRightCornerPercentage = FVector2D(GetHandlePosition(EHandlePosition::Right).X, GetHandlePosition(EHandlePosition::Bottom).Y);
	const FVector2D BottomRightCornerCanvasSpace = FVector2D(CanvasSize.X * BottomRightCornerPercentage.X, CanvasSize.Y * BottomRightCornerPercentage.Y);
	const FVector2D BottomRightCornerWidgetSpace = BottomRightCornerCanvasSpace + WidgetToCanvas;

	auto DrawLine = [&](FVector2D Start, FVector2D Finish)
	{
		FSlateDrawElement::MakeLines(OutDrawElements, LayerId++, AllottedGeometry.ToPaintGeometry(), TArray({Start, Finish}), ESlateDrawEffect::None, FLinearColor::Red);
	};

	{
		const FVector2D Start = {UpperLeftCornerWidgetSpace.X, 0.0};
		const FVector2D Finish = {UpperLeftCornerWidgetSpace.X, AllottedGeometry.GetLocalSize().Y};
		DrawLine(Start, Finish);
	}
	{
		const FVector2D Start = {BottomRightCornerWidgetSpace.X, 0.0};
		const FVector2D Finish = {BottomRightCornerWidgetSpace.X, AllottedGeometry.GetLocalSize().Y};
		DrawLine(Start, Finish);
	}
	{
		const FVector2D Start = {0.0, UpperLeftCornerWidgetSpace.Y};
		const FVector2D Finish = {AllottedGeometry.GetLocalSize().X, UpperLeftCornerWidgetSpace.Y};
		DrawLine(Start, Finish);
	}
	{
		const FVector2D Start = {0.0, BottomRightCornerWidgetSpace.Y};
		const FVector2D Finish = {AllottedGeometry.GetLocalSize().X, BottomRightCornerWidgetSpace.Y};
		DrawLine(Start, Finish);
	}

	return LayerId;
}

FReply SNineSlicerTab::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const FVector2D MousePosition = AbsolutePositionToPercentage(MouseEvent.GetScreenSpacePosition());
	HandleEdited = GetClosestHandle(MousePosition);

	if (ScopedTransaction == nullptr)
	{
		ScopedTransaction = new FScopedTransaction(INVTEXT("Test"));
	}

	return FReply::Handled();
}

FReply SNineSlicerTab::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	HandleEdited.Reset();

	if (ScopedTransaction != nullptr)
	{
		delete ScopedTransaction;
		ScopedTransaction = nullptr;
	}

	WeakBlueprintEditor.Pin()->RefreshPreview();


	return FReply::Handled();
}

FReply SNineSlicerTab::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (HandleEdited.IsSet())
	{
		const FVector2D MousePosition = AbsolutePositionToPercentage(MouseEvent.GetScreenSpacePosition());
		SetHandePosition(HandleEdited.GetValue(), MousePosition);
	}

	UE_LOG(LogTemp, Warning, TEXT("Mouse positino: %s %s"), *LexToString(MouseEvent.GetScreenSpacePosition().X), *LexToString(MouseEvent.GetScreenSpacePosition().Y));

	return FReply::Unhandled();
}

TOptional<EMouseCursor::Type> SNineSlicerTab::GetCursor() const
{
	if (HandleEdited.IsSet())
	{
		return EMouseCursor::GrabHandClosed;
	}

	const FVector2D MousePosition = AbsolutePositionToPercentage(FSlateApplication::Get().GetCursorPos());
	const TOptional<EHandlePosition> NearbyHandle = GetClosestHandle(MousePosition);
	if(NearbyHandle.IsSet())
	{
		return EMouseCursor::GrabHand;
	}

	return SCompoundWidget::GetCursor();
}

FVector2D SNineSlicerTab::AbsolutePositionToPercentage(const FVector2D& AbsolutePosition) const
{
	const FVector2D WidgetSize = ViewCanvas->GetCachedGeometry().GetLocalSize();
	const FVector2D WidgetSpace = ViewCanvas->GetCachedGeometry().AbsoluteToLocal(AbsolutePosition);
	double PosX = WidgetSpace.X / WidgetSize.X;
	double PosY = WidgetSpace.Y / WidgetSize.Y;

	return {PosX, PosY};
}

FVector2D SNineSlicerTab::PercentageToAbsolutePosition(const FVector2D& Percentage) const
{
	const FVector2D WidgetSize = ViewCanvas->GetCachedGeometry().GetAbsoluteSize();
	double PosX = Percentage.X * WidgetSize.X;
	double PosY = Percentage.Y * WidgetSize.Y;
	const FVector2D AbsoluteSpace = ViewCanvas->GetCachedGeometry().LocalToAbsolute({PosX, PosY});

	return AbsoluteSpace;
}

void SNineSlicerTab::SetHandePosition(EHandlePosition Handle, FVector2D InValue)
{
	// TODO: Clamp everything between 0-1
	// TODO: don't let left cross top or top cross bottom
	UImage* Image = GetCurrentImage();
	if (!Image)
	{
		return;
	}

	auto Brush = Image->GetBrush();

	if (Handle == EHandlePosition::Top)
	{
		Brush.Margin.Top = InValue.Y;
	}
	if (Handle == EHandlePosition::Left)
	{
		Brush.Margin.Left = InValue.X;
	}
	if (Handle == EHandlePosition::Bottom)
	{
		Brush.Margin.Bottom = 1.0 - InValue.Y;
	}
	if (Handle == EHandlePosition::Right)
	{
		Brush.Margin.Right = 1.0 - InValue.X;
	}

	Image->SetFlags(RF_Transactional);
	Image->Modify();

	static const FName BrushPropertyName(TEXT("Brush"));
	FObjectEditorUtils::SetPropertyValue<UImage, FSlateBrush>(Image, BrushPropertyName, Brush);
	Image->SetBrush(Brush);
}

FVector2D SNineSlicerTab::GetHandlePosition(EHandlePosition Handle) const
{
	if (Handle == EHandlePosition::Top)
	{
		const float X = 0.5;
		const float Y = CurrentData.Margin.Top;
		return FVector2D(X, Y);
	}
	if (Handle == EHandlePosition::Left)
	{
		const float X = CurrentData.Margin.Left;
		const float Y = 0.5;
		return FVector2D(X, Y);
	}
	if (Handle == EHandlePosition::Bottom)
	{
		const float X = 0.5;
		const float Y = 1.0 - CurrentData.Margin.Bottom;
		return FVector2D(X, Y);
	}
	if (Handle == EHandlePosition::Right)
	{
		const float X = 1.0 - CurrentData.Margin.Right;
		const float Y = 0.5;
		return FVector2D(X, Y);
	}

	return FVector2D::ZeroVector;
}

TOptional<EHandlePosition> SNineSlicerTab::GetClosestHandle(FVector2D MousePosition) const
{
	TMap<EHandlePosition, FVector2D> CurrentPositions;
	CurrentPositions.Emplace(EHandlePosition::Top, GetHandlePosition(EHandlePosition::Top));
	CurrentPositions.Emplace(EHandlePosition::Left, GetHandlePosition(EHandlePosition::Left));
	CurrentPositions.Emplace(EHandlePosition::Bottom, GetHandlePosition(EHandlePosition::Bottom));
	CurrentPositions.Emplace(EHandlePosition::Right, GetHandlePosition(EHandlePosition::Right));

	double MinDistance = MaxGrabDistance;
	TOptional<EHandlePosition> Result;

	for (const TTuple<EHandlePosition, FVector2D>& Position : CurrentPositions)
	{
		const double Distance = FVector2D::DistSquared(Position.Value, MousePosition);
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			Result = Position.Key;
		}
	}

	return Result;
}
