// Copyright Gunfire Games, LLC. All Rights Reserved.

#pragma once

#include "VoidwalkerNavQueryFilter.h"

#include "NavigationData.h"

enum class EVoidwalkerNavPathFlags : uint8
{
	SkipStringPulling	= 1 << 0,
	SkipSmoothing		= 1 << 1,
};

enum class EVoidwalkerNavPathQueryFlags : uint8
{
	// Detail flags
	PartialPath		= (uint8)EVoidwalkerNavQueryFlags::UserFlags << 0,
	CyclicalPath	= (uint8)EVoidwalkerNavQueryFlags::UserFlags << 1,
};
ENUM_CLASS_FLAGS(EVoidwalkerNavPathQueryFlags);

struct FVoidwalkerNavPathQueryResults : FVoidwalkerNavQueryResults
{
	uint32 PathNodeCount = 0;
	float PathLength = 0.f;
	float PathCost = 0.f;
	TArray<FNavPathPoint> PathPortalPoints;

	virtual void Reset() override
	{
		FVoidwalkerNavQueryResults::Reset();

		PathNodeCount = 0;
		PathLength = 0.f;
		PathCost = 0.f;
		PathPortalPoints.Reset();
	}

	bool IsPartial()
	{
		return (Status & (uint8)EVoidwalkerNavPathQueryFlags::PartialPath) != 0;
	}

	bool RanOutOfNodes()
	{
		return (Status & (uint8)EVoidwalkerNavQueryFlags::OutOfNodes) != 0;
	}
};

struct VOIDWALKERNAVIGATION_API FVoidwalkerNavPath : public FNavigationPath
{
	typedef FNavigationPath Super;

	FVoidwalkerNavPath();

	// If true, the path will be tightened up to be more direct.
	bool WantsStringPulling() const { return bStringPull; }
	void SetWantsStringPulling(bool bValue) { bStringPull = bValue; }

	// If true, the path will be smoothed to remove harsh angles
	bool WantsSmoothing() const { return bSmooth; }
	void SetWantsSmoothing(bool bValue) { bSmooth = bValue; }

	// Information about how the path was generated
	FVoidwalkerNavPathQueryResults& GetGenerationInfo() { return GenerationInfo; }
	const FVoidwalkerNavPathQueryResults& GetGenerationInfo() const { return GenerationInfo; }

	// Applies custom flags to the path
	void ApplyFlags(uint32 NavDataFlags);

	//~ Begin FNavigationPath Interface

	// Resets all variables describing generated path before attempting new path finding
	// call. This function will NOT reset setup variables like goal actor, filter,
	// observer, etc
	virtual void ResetForRepath() override;

	// This is a duplicate of FNavigationPath::DebugDraw less the
	// NavigationDebugDrawing::PathOffset which isn't needed for nav volumes
	virtual void DebugDraw(const ANavigationData* NavData, const FColor PathColor, class UCanvas* Canvas, const bool bPersistent, const float LifeTime, const uint32 NextPathPointIndex = 0) const override;

	//~ End FNavigationPath Interface

public:
	static const FNavPathType Type;

private:
	bool bStringPull = true;
	bool bSmooth = true;

	FVoidwalkerNavPathQueryResults GenerationInfo;
};