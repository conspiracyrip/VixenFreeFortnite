#pragma once

void (*GetCameraViewPointOriginal)(APlayerCameraManager*, FVector*, FRotator*);
void GetCameraViewPoint(APlayerCameraManager* PlayerCameraManager, FVector* OutLocation, FRotator* OutRotation)
{
	GetCameraViewPointOriginal(PlayerCameraManager, OutLocation, OutRotation);

	static bool Old_bShouldStartAtLocationTarget = false;
	static FVector StaticStickyLocation = FVector();

	FVector OldLocation = *OutLocation;

	if (Variables::GetPlayerViewPoint::bShouldStartAtLocationTarget != Old_bShouldStartAtLocationTarget)
	{
		StaticStickyLocation = *OutLocation;

		Old_bShouldStartAtLocationTarget = Variables::GetPlayerViewPoint::bShouldStartAtLocationTarget;
	}

	if (Settings::Developer::Debugging::StickySilentAimV2 && Variables::GetPlayerViewPoint::bShouldStartAtLocationTarget)
	{
		if (Settings::Aimbot::StickySilent::StickyCameraSpin && StaticStickyLocation)
		{
			*OutLocation = StaticStickyLocation;
		}
		else
		{
			*OutLocation = Variables::GetCameraViewPoint::StickyLocation;
		}

		if (!Settings::Developer::ServersideSpinbotV2)
		{
			*OutRotation = StaticClasses::KismetMathLibrary->FindLookAtRotation(*OutLocation, Variables::GetPlayerViewPoint::SilentLocationTarget);
		}
		else
		{
			*OutRotation = Variables::CameraRotation;
		}


		Variables::GetCameraViewPoint::LastStickyRotation = *OutRotation;
	}
}