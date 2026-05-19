// Copyright Gunfire Games, LLC. All Rights Reserved.
// Copyright Colin Bonstead. All Rights Reserved.

#include "VoidwalkerNavigationCustomVersion.h"

#include "SparseVoxelOctree/SparseVoxelOctreeConfig.h"

// Initialize Statics
const FGuid FVoidwalkerNavigationCustomVersion::GUID(0xf12bd3fd, 0xf63c4121, 0x916cf40c, 0xc349229a);
FSvoConfig* FVoidwalkerNavigationCustomVersion::SvoConfig = nullptr;

// Register the custom version with core
FCustomVersionRegistration GRegisterVoidwalkerNavigationCustomVersion(FVoidwalkerNavigationCustomVersion::GUID, FVoidwalkerNavigationCustomVersion::LatestVersion, TEXT("VoidwalkerNavigationVer"));
