// Copyright Gunfire Games, LLC. All Rights Reserved.
// Copyright Colin Bonstead. All Rights Reserved.

#pragma once

// Custom serialization version for all plugin relevant classes/structs
struct FVoidwalkerNavigationCustomVersion
{
	enum Type
	{
		// Forked project from Gunfire 3D Navigation
		InitialVersion = 1,

		// -----<new versions can be added above this line>-----
		VersionPlusOne,
		LatestVersion = VersionPlusOne - 1
	};

	// The GUID for this custom version number
	const static FGuid GUID;

	// Config of the current octree being loaded
	static struct FSvoConfig* SvoConfig;
};