// Copyright Gunfire Games, LLC. All Rights Reserved.
// Copyright Colin Bonstead. All Rights Reserved.

#include "VoidwalkerNavQueryFilter.h"
#include "VoidwalkerNavData.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(VoidwalkerNavQueryFilter)

//////////////////////////////////////////////////////////////////////////
// VoidwalkerNavQueryConstraints
//////////////////////////////////////////////////////////////////////////

bool FVoidwalkerNavQueryConstraints::HasConstraints() const
{
	return Bounds.Num() > 0;
}

bool FVoidwalkerNavQueryConstraints::ConstrainBounds(FBox& InOutBounds) const
{
	// Inclusion Bounds
	if (Bounds.Num() > 0)
	{
		bool bIsWithinConstraints = false;

		// Find the clipped node bounds
		for (const FBox& ConstraintBounds : Bounds)
		{
			if (InOutBounds.Intersect(ConstraintBounds))
			{
				InOutBounds = InOutBounds.Overlap(ConstraintBounds);
				bIsWithinConstraints = true;
			}
		}

		return bIsWithinConstraints;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// VoidwalkerNavQueryFilter
//////////////////////////////////////////////////////////////////////////

bool FVoidwalkerNavQueryFilter::IsEqual(const INavigationQueryFilterInterface* Other) const
{
	// TODO: This doesn't play nice with any other filter type. Epic mentions this in
	//		 FRecastQueryFilter::IsEqual which this was taken from.  If we want to address
	//		 this, we'll need to update the FRecastQueryFilter::IsEqual as well.
	return FMemory::Memcmp(this, Other, sizeof(FVoidwalkerNavQueryFilter)) == 0;
}

INavigationQueryFilterInterface* FVoidwalkerNavQueryFilter::CreateCopy() const
{
	return new FVoidwalkerNavQueryFilter(*this);
}

//////////////////////////////////////////////////////////////////////////
// VoidwalkerNavigationQueryFilter
//////////////////////////////////////////////////////////////////////////

void UVoidwalkerNavigationQueryFilter::InitializeFilter(const ANavigationData& NavData, const UObject* Querier, FNavigationQueryFilter& Filter) const
{
	if (NavData.GetClass()->IsChildOf(AVoidwalkerNavData::StaticClass()))
	{
		Filter.SetFilterType<FVoidwalkerNavQueryFilter>();
		if (FVoidwalkerNavQueryFilter* NavFilterImpl = static_cast<FVoidwalkerNavQueryFilter*>(Filter.GetImplementation()))
		{
			NavFilterImpl->SetHeuristicScale(PathHeuristicScale);
			NavFilterImpl->SetBaseTraversalCost(NodeBaseTraversalCost);
		}

		Filter.SetMaxSearchNodes(MaxPathSearchNodes);
	}

	Super::InitializeFilter(NavData, Querier, Filter);
}
