// Copyright Gunfire Games, LLC. All Rights Reserved.

#include "VoidwalkerNavigationCustomVersion.h"

#include "SparseVoxelOctree/SparseVoxelOctreeConfig.h"

// Initialize Statics
const FGuid FVoidwalkerNavigationCustomVersion::GUID(0x8EE8740C, 0xE2E4451C, 0x9881C96F, 0xB03956CA);
FSvoConfig* FVoidwalkerNavigationCustomVersion::SvoConfig = nullptr;

// Register the custom version with core
FCustomVersionRegistration GRegisterVoidwalkerNavigationCustomVersion(FVoidwalkerNavigationCustomVersion::GUID, FVoidwalkerNavigationCustomVersion::LatestVersion, TEXT("VoidwalkerNavigationVer"));
