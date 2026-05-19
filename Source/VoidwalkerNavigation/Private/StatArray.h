// Copyright Gunfire Games, LLC. All Rights Reserved.
// Copyright Colin Bonstead. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// TStatArray is the TArray with memory tracking.  By default, all memory will be tracked
// within the 'Misc Array Memory' category.  To add the memory to a different category,
// either pass GET_STATID(STAT_MyStatName) to the constructor of the TStatArray or call
// SetStatID(GET_STATID(STAT_MyStatName).  Either of these calls should be wrapped in '#if STATS'.

DECLARE_STATS_GROUP(TEXT("VoidwalkerNavigation"), STATGROUP_VoidwalkerNavigation, STATCAT_Advanced);
DECLARE_MEMORY_STAT(TEXT("Total Memory"), STAT_VoidwalkerNavigation_TotalMemory, STATGROUP_VoidwalkerNavigation);
DECLARE_MEMORY_STAT(TEXT("Misc Element Memory"), STAT_VoidwalkerNavigation_MiscElementMemory, STATGROUP_VoidwalkerNavigation);

namespace VoidwalkerNavigation
{
	namespace Memory
	{
#if STATS
		// @todo could be made a more generic solution
		class FStatDefaultAllocator : public FDefaultAllocator
		{
		public:
			typedef FDefaultAllocator Super;

			class ForAnyElementType : public FDefaultAllocator::ForAnyElementType
			{
			public:
				typedef FDefaultAllocator::ForAnyElementType Super;

			private:
				int32 AllocatedSize;

			public:
				TStatId StatID;

			public:
				ForAnyElementType()
					: AllocatedSize(0)
					, StatID(GET_STATID(STAT_VoidwalkerNavigation_MiscElementMemory))
				{
				}

				/** Destructor. */
				~ForAnyElementType()
				{
					if (AllocatedSize)
					{
						DEC_DWORD_STAT_FNAME_BY(StatID.GetName(), AllocatedSize);
						DEC_DWORD_STAT_BY(STAT_VoidwalkerNavigation_TotalMemory, AllocatedSize);
					}
				}

				void ResizeAllocation(int32 PreviousNumElements, int32 NumElements, int32 NumBytesPerElement, uint32 AlignmentOfElement)
				{
					const int32 NewSize = NumElements * NumBytesPerElement;
					INC_DWORD_STAT_FNAME_BY(StatID.GetName(), NewSize - AllocatedSize);
					INC_DWORD_STAT_BY(STAT_VoidwalkerNavigation_TotalMemory, NewSize - AllocatedSize);
					AllocatedSize = NewSize;

					Super::ResizeAllocation(PreviousNumElements, NumElements, NumBytesPerElement, AlignmentOfElement);
				}

			private:
				ForAnyElementType(const ForAnyElementType&);
				ForAnyElementType& operator=(const ForAnyElementType&);
			};

			template<typename ElementType>
			class ForElementType : public ForAnyElementType
			{
			public:

				ForElementType()
				{}

				ElementType* GetAllocation() const
				{
					return (ElementType*)ForAnyElementType::GetAllocation();
				}
			};
		};

		typedef FStatDefaultAllocator FStatAllocator;
#else
		typedef FDefaultAllocator FStatAllocator;
#endif
	}
}

#if STATS

template <>
struct TAllocatorTraits<VoidwalkerNavigation::Memory::FStatDefaultAllocator> : TAllocatorTraits<VoidwalkerNavigation::Memory::FStatDefaultAllocator::Super>
{
};

#endif

template<typename InElementType>
class TStatArray : public TArray<InElementType, VoidwalkerNavigation::Memory::FStatAllocator>
{
public:
	typedef TArray<InElementType, VoidwalkerNavigation::Memory::FStatAllocator> Super;

#if STATS
	TStatArray(TStatId InStatID = GET_STATID(STAT_VoidwalkerNavigation_MiscElementMemory))
	{
		SetStatID(InStatID);
	}

	void SetStatID(TStatId InStatID)
	{
		Super::AllocatorInstance.StatID = InStatID;
	}
#endif
};
