// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class FWidgetBlueprintEditor;

struct FMVVMBindingSummoner : public FWorkflowTabFactory
{
	static const FName TabID;
	static const FName DrawerID;

	FMVVMBindingSummoner(TSharedPtr<FWidgetBlueprintEditor> BlueprintEditor, bool bInIsDrawerTab = false);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<FWidgetBlueprintEditor> WeakWidgetBlueprintEditor;
	bool bIsDrawerTab;
};
