// Copyright Out-of-the-Box Plugins 2018-2025. All Rights Reserved.

#include "SNineSlicerWidget.h"

#include <Components/Image.h>
#include <WidgetBlueprintEditor.h>
#include <WidgetReference.h>
#include <Widgets/SCanvas.h>

#include "NineSlicerInputProcessor.h"
#include "NineSlicerSettings.h"

#define LOCTEXT_NAMESPACE "NineSlicer"

/**
 * Helper function to truncate a double's decimals
 */
double RoundDecimal(double InNumber, int32 Decimals)
{
	const double Multiplier = FMath::Pow(10.0, Decimals);
	return FMath::CeilToDouble(InNumber * Multiplier) / Multiplier;
}

void SNineSlicerWidget::Construct(const FArguments& InArgs, const TWeakPtr<FWidgetBlueprintEditor>& InBlueprintEditor)
{
	WeakBlueprintEditor = InBlueprintEditor;

	// clang-format off
	ChildSlot
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
	];
	// clang-format on

	AddMarginHandle(EHandlePosition::Top);
	AddMarginHandle(EHandlePosition::Left);
	AddMarginHandle(EHandlePosition::Bottom);
	AddMarginHandle(EHandlePosition::Right);

	FSlateApplication& App = FSlateApplication::Get();
	App.OnPreTick().AddSP(this, &SNineSlicerWidget::OnPreTick);

	InputProcessor = MakeShared<FNineSlicerInputProcessor>();
	InputProcessor->OnMouseButtonDown.BindSP(this, &SNineSlicerWidget::OnProcessorMouseButtonDown);
	InputProcessor->OnMouseButtonUp.BindSP(this, &SNineSlicerWidget::OnProcessorMouseButtonUp);
	InputProcessor->OnMouseMove.BindSP(this, &SNineSlicerWidget::OnProcessorMouseMove);

	App.RegisterInputPreProcessor(InputProcessor);
}

SNineSlicerWidget::~SNineSlicerWidget()
{
	FSlateApplication::Get().UnregisterInputPreProcessor(InputProcessor);
}

void SNineSlicerWidget::ResetMargins()
{
	FScopedTransaction ResetTransaction(LOCTEXT("NineSlicerReset", "Nine Slicer Reset"));

	SetHandlePosition(EHandlePosition::Top, {0, 0});
	SetHandlePosition(EHandlePosition::Left, {0, 0});
	SetHandlePosition(EHandlePosition::Bottom, {1, 1});
	SetHandlePosition(EHandlePosition::Right, {1, 1});

	WeakBlueprintEditor.Pin()->RefreshPreview();
}

void SNineSlicerWidget::AddMarginHandle(EHandlePosition Handle)
{
	auto ComputeSize = [=, this]()
	{
		const UNineSlicerSettings* Settings = GetDefault<UNineSlicerSettings>();

		const FVector2D HandleSize = FVector2D::One() * Settings->HandleSize;
		return FVector2D::One() / GetCachedGeometry().Scale * HandleSize;
	};

	// clang-format off
	SCanvas::FScopedWidgetSlotArguments NewSlot = ViewCanvas->AddSlot();
	NewSlot
		.Size_Lambda(ComputeSize)
		.Position_Lambda( [=, this]()
			{
				const FVector2D Offset = ComputeSize() / 2;

				const FVector2D Coords = GetHandlePosition(Handle);
				const FVector2D CanvasSize = ViewCanvas->GetCachedGeometry().GetLocalSize();
				return FVector2D(CanvasSize.X * Coords.X, CanvasSize.Y * Coords.Y) -  Offset;
			})
		[
			SNew(SImage)
			.Image(FAppStyle::Get().GetBrush("UMGEditor.TransformHandle"))
			.ColorAndOpacity_Lambda([]()
			{
				const UNineSlicerSettings* Settings = GetDefault<UNineSlicerSettings>();
				return Settings->DrawColor;
			})
		];
	// clang-format on
}

UImage* SNineSlicerWidget::GetCurrentImage() const
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

	const FWidgetReference SelectedWidget = SelectedWidgets.Array()[0];

	UWidget* Widget = SelectedWidget.GetTemplate();
	return Cast<UImage>(Widget);
}

FVector2D SNineSlicerWidget::GetHandlePosition(EHandlePosition Handle) const
{
	if (Handle == EHandlePosition::Top)
	{
		constexpr float X = 0.5;
		const float Y = CurrentData.Margin.Top;
		return FVector2D(X, Y);
	}
	if (Handle == EHandlePosition::Left)
	{
		const float X = CurrentData.Margin.Left;
		constexpr float Y = 0.5;
		return FVector2D(X, Y);
	}
	if (Handle == EHandlePosition::Bottom)
	{
		constexpr float X = 0.5;
		const float Y = 1.0 - CurrentData.Margin.Bottom;
		return FVector2D(X, Y);
	}
	if (Handle == EHandlePosition::Right)
	{
		const float X = 1.0 - CurrentData.Margin.Right;
		constexpr float Y = 0.5;
		return FVector2D(X, Y);
	}

	return FVector2D::ZeroVector;
}

TOptional<EHandlePosition> SNineSlicerWidget::GetClosestHandle(FVector2D AbsolutePosition) const
{
	constexpr float MaxGrabDistance = 0.001f;
	const FVector2D PercentagePosition = AbsolutePositionToPercentage(AbsolutePosition);

	TMap<EHandlePosition, FVector2D> CurrentPositions;
	CurrentPositions.Emplace(EHandlePosition::Top, GetHandlePosition(EHandlePosition::Top));
	CurrentPositions.Emplace(EHandlePosition::Left, GetHandlePosition(EHandlePosition::Left));
	CurrentPositions.Emplace(EHandlePosition::Bottom, GetHandlePosition(EHandlePosition::Bottom));
	CurrentPositions.Emplace(EHandlePosition::Right, GetHandlePosition(EHandlePosition::Right));

	double MinDistance = MaxGrabDistance;
	TOptional<EHandlePosition> Result;

	for (const TTuple<EHandlePosition, FVector2D>& Position : CurrentPositions)
	{
		const double Distance = FVector2D::DistSquared(Position.Value, PercentagePosition);
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			Result = Position.Key;
		}
	}

	return Result;
}

void SNineSlicerWidget::SetHandlePosition(EHandlePosition Handle, FVector2D InValue)
{
	UImage* Image = GetCurrentImage();
	if (!Image)
	{
		return;
	}

	auto Brush = Image->GetBrush();

	const UNineSlicerSettings* Settings = GetDefault<UNineSlicerSettings>();
	const auto Precision = Settings->DecimalPrecision;
	if (Precision > 0)
	{
		InValue = FVector2D(RoundDecimal(InValue.X, Precision), RoundDecimal(InValue.Y, Precision));
	}

	if (Handle == EHandlePosition::Top)
	{
		Brush.Margin.Top = FMath::Clamp(InValue.Y, 0, 1);
	}
	if (Handle == EHandlePosition::Left)
	{
		Brush.Margin.Left = FMath::Clamp(InValue.X, 0, 1);
	}
	if (Handle == EHandlePosition::Bottom)
	{
		Brush.Margin.Bottom = FMath::Clamp(1.0 - InValue.Y, 0, 1);
	}
	if (Handle == EHandlePosition::Right)
	{
		Brush.Margin.Right = FMath::Clamp(1.0 - InValue.X, 0, 1);
	}

	Image->SetFlags(RF_Transactional);
	Image->Modify();
	Image->SetBrush(Brush);
}

FVector2D SNineSlicerWidget::AbsolutePositionToPercentage(const FVector2D& AbsolutePosition) const
{
	const FVector2D WidgetSize = ViewCanvas->GetCachedGeometry().GetLocalSize();
	const FVector2D WidgetSpace = ViewCanvas->GetCachedGeometry().AbsoluteToLocal(AbsolutePosition);
	double PosX = WidgetSpace.X / WidgetSize.X;
	double PosY = WidgetSpace.Y / WidgetSize.Y;

	return {PosX, PosY};
}

void SNineSlicerWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	UpdateBrushAndState();
}

int32 SNineSlicerWidget::OnPaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled
) const
{
	if (ErrorState.IsSet())
	{
		return LayerId;
	}

	SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	const UNineSlicerSettings* Settings = GetDefault<UNineSlicerSettings>();

	const FVector2D UpperLeftCornerPercentage = FVector2D(GetHandlePosition(EHandlePosition::Left).X, GetHandlePosition(EHandlePosition::Top).Y);
	const FVector2D BottomRightCornerPercentage = FVector2D(GetHandlePosition(EHandlePosition::Right).X, GetHandlePosition(EHandlePosition::Bottom).Y);

	const FVector2D UpperLeftCornerWidgetSpace = AllottedGeometry.GetLocalSize() * UpperLeftCornerPercentage;
	const FVector2D BottomRightCornerWidgetSpace = AllottedGeometry.GetLocalSize() * BottomRightCornerPercentage;

	auto DrawLine = [&](FVector2D Start, FVector2D Finish)
	{
		FSlateDrawElement::MakeLines(OutDrawElements, ++LayerId, AllottedGeometry.ToPaintGeometry(), TArray({Start, Finish}), ESlateDrawEffect::None, Settings->DrawColor);
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

bool SNineSlicerWidget::OnProcessorMouseButtonUp(const FPointerEvent& MouseEvent)
{
	if (!HandleEdited.IsSet())
	{
		return false;
	}

	HandleEdited.Reset();
	if (ensure(ScopedTransaction.IsValid()))
	{
		ScopedTransaction.Reset();
	}

	WeakBlueprintEditor.Pin()->RefreshPreview();
	return true;
}

bool SNineSlicerWidget::OnProcessorMouseMove(const FPointerEvent& MouseEvent)
{
	if (!HandleEdited.IsSet())
	{
		return false;
	}

	const FVector2D MousePosition = AbsolutePositionToPercentage(FSlateApplication::Get().GetCursorPos());
	SetHandlePosition(HandleEdited.GetValue(), MousePosition);

	return true;
}

bool SNineSlicerWidget::OnProcessorMouseButtonDown(const FPointerEvent& MouseEvent)
{
	HandleEdited = GetClosestHandle(MouseEvent.GetScreenSpacePosition());

	if (!HandleEdited.IsSet())
	{
		return false;
	}

	if (ensure(!ScopedTransaction.IsValid()))
	{
		ScopedTransaction = MakeShared<FScopedTransaction>(LOCTEXT("NineSlicerAdjustement", "Nine Slicer Adjustement"));
	}

	return true;
}

void SNineSlicerWidget::OnPreTick(float DeltaTime)
{
	const TOptional<EMouseCursor::Type> CursorType = ComputeCursor();
	if (CursorType.IsSet())
	{
		FSlateApplication::Get().GetPlatformCursor()->SetType(CursorType.GetValue());
	}
	else
	{
		FSlateApplication::Get().QueryCursor();
	}
}

TOptional<EMouseCursor::Type> SNineSlicerWidget::ComputeCursor() const
{
	if (HandleEdited.IsSet())
	{
		return EMouseCursor::GrabHandClosed;
	}

	const FVector2D MousePosition = FSlateApplication::Get().GetCursorPos();
	const TOptional<EHandlePosition> NearbyHandle = GetClosestHandle(MousePosition);
	if (NearbyHandle.IsSet())
	{
		return EMouseCursor::GrabHand;
	}

	return {};
}

void SNineSlicerWidget::UpdateBrushAndState()
{
	const UImage* CurrentImage = GetCurrentImage();

	if (!CurrentImage)
	{
		ErrorState = INVTEXT("Current selection is not an image.");
		return;
	}

	const FSlateBrush Brush = CurrentImage->GetBrush();
	if (Brush.DrawAs != ESlateBrushDrawType::Box && Brush.DrawAs != ESlateBrushDrawType::Border)
	{
		ErrorState = INVTEXT("Current image is not sliceable. Change the brush's DrawAs to Box or Border");
		return;
	}

	if (!Brush.GetResourceObject())
	{
		ErrorState = INVTEXT("Current image doesn't have a valid texture assigned");
		return;
	}

	CurrentData = CurrentImage->GetBrush();
	CurrentData.DrawAs = ESlateBrushDrawType::Image;
	ErrorState.Reset();
}

#undef LOCTEXT_NAMESPACE