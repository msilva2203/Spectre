// © 2024 Marco Silva. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SpectreEditorTarget : TargetRules
{
	public SpectreEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Spectre" } );
	}
}
