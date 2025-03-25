#pragma once

FRotator* ( *GetControlRotationOriginal )( APlayerController* , FRotator* );
FRotator* GetControlRotation( APlayerController* PlayerController , FRotator* Rotation )
{
	GetControlRotationOriginal( PlayerController , Rotation );

	if ( Variables::GetControlRotation::bAimbotActive && Variables::GetControlRotation::AimbotRotationTarget )
	{
		*Rotation = Variables::GetControlRotation::AimbotRotationTarget;
	}

	return Rotation;
}