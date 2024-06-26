// © 2024 Marco Silva. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SpectreTarget : TargetRules
{
	public SpectreTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Spectre" } );
	}
}
