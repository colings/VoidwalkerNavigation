// Copyright Gunfire Games, LLC. All Rights Reserved.

#include "VoidwalkerNavDataRenderingComponent.h"

#include "VoidwalkerNavData.h"
#include "NavSvo/NavSvoSceneProxy.h"

#include "Engine/CollisionProfile.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(VoidwalkerNavDataRenderingComponent)

#if WITH_EDITOR
namespace
{
	bool AreAnyViewportsRelevant(const UWorld* World)
	{
		FWorldContext* WorldContext = GEngine->GetWorldContextFromWorld(World);
		if (WorldContext && WorldContext->GameViewport)
		{
			return true;
		}

		for (FEditorViewportClient* CurrentViewport : GEditor->GetAllViewportClients())
		{
			if (CurrentViewport && CurrentViewport->IsVisible())
			{
				return true;
			}
		}

		return false;
	}
}
#endif

UVoidwalkerNavRenderingComponent::UVoidwalkerNavRenderingComponent()
{
	SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	bIsEditorOnly = true;
	bSelectable = false;
	bCollectNavigationData = false;
}

void UVoidwalkerNavRenderingComponent::OnRegister()
{
	Super::OnRegister();

#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
	// it's a kind of HACK but there is no event or other information that show flag was changed by user => we have to check it periodically
#if WITH_EDITOR
	if (GEditor)
	{
		GEditor->GetTimerManager()->SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &UVoidwalkerNavRenderingComponent::TimerFunction), 1, true);
	}
	else
#endif //WITH_EDITOR
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &UVoidwalkerNavRenderingComponent::TimerFunction), 1, true);
	}
#endif //!UE_BUILD_SHIPPING && !UE_BUILD_TEST
}

void UVoidwalkerNavRenderingComponent::OnUnregister()
{
#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
	// it's a kind of HACK but there is no event or other information that show flag was changed by user => we have to check it periodically
#if WITH_EDITOR
	if (GEditor)
	{
		GEditor->GetTimerManager()->ClearTimer(TimerHandle);
	}
	else
#endif //WITH_EDITOR
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
#endif //!UE_BUILD_SHIPPING && !UE_BUILD_TEST

	Super::OnUnregister();
}

FPrimitiveSceneProxy* UVoidwalkerNavRenderingComponent::CreateSceneProxy()
{
#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
	const bool bShowNavigation = IsNavigationShowFlagSet(GetWorld());

	bCollectNavigationData = bShowNavigation;

	if (bCollectNavigationData && IsVisible())
	{
		const AVoidwalkerNavData* NavData = Cast<AVoidwalkerNavData>(GetOwner());
		if (NavData && NavData->IsDrawingEnabled())
		{
			return new FNavSvoSceneProxy(this, NavData);
		}
	}
#endif //!UE_BUILD_SHIPPING && !UE_BUILD_TEST

	return nullptr;
}

FBoxSphereBounds UVoidwalkerNavRenderingComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBox BoundingBox(ForceInit);

	AVoidwalkerNavData* NavData = Cast<AVoidwalkerNavData>(GetOwner());
	if (NavData)
	{
		BoundingBox = NavData->GetBounds();
	}

	return FBoxSphereBounds(BoundingBox);
}

void UVoidwalkerNavRenderingComponent::TimerFunction()
{
	const UWorld* World = GetWorld();
#if WITH_EDITOR
	if (GEditor && (AreAnyViewportsRelevant(World) == false))
	{
		// unable to tell if the flag is on or not
		return;
	}
#endif // WITH_EDITOR

	const bool bShowNavigation = (bForceUpdate || IsNavigationShowFlagSet(World));

	if (bShowNavigation != !!bCollectNavigationData && bShowNavigation == true)
	{
		bForceUpdate = false;
		bCollectNavigationData = bShowNavigation;
		MarkRenderStateDirty();
	}
}

bool UVoidwalkerNavRenderingComponent::IsNavigationShowFlagSet(const UWorld* World)
{
	bool bShowNavigation = false;

	FWorldContext* WorldContext = GEngine->GetWorldContextFromWorld(World);

#if WITH_EDITOR
	if (GEditor && WorldContext && WorldContext->WorldType != EWorldType::Game)
	{
		bShowNavigation = (WorldContext->GameViewport != nullptr && WorldContext->GameViewport->EngineShowFlags.Navigation);
		if (bShowNavigation == false)
		{
			// we have to check all viewports because we can't to distinguish between SIE and PIE at this point.
			for (FEditorViewportClient* CurrentViewport : GEditor->GetAllViewportClients())
			{
				if (CurrentViewport && CurrentViewport->IsVisible() && CurrentViewport->EngineShowFlags.Navigation)
				{
					bShowNavigation = true;
					break;
				}
			}
		}
	}
	else
#endif //WITH_EDITOR
	{
		bShowNavigation = (WorldContext && WorldContext->GameViewport && WorldContext->GameViewport->EngineShowFlags.Navigation);
	}

	return bShowNavigation;
}