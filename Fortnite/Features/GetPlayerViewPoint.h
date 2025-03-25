#pragma once

void (*GetPlayerViewPointOriginal)(APlayerController*, FVector*, FRotator*);
void GetPlayerViewPoint(APlayerController* PlayerController, FVector* Location, FRotator* Rotation)
{
	GetPlayerViewPointOriginal(PlayerController, Location, Rotation);

	if (Variables::GetPlayerViewPoint::bShouldStartAtLocationTarget)
	{
		Location->Z += 55;
		Location->Y += 55;
		Location->X += 55;

		Rotation->Pitch += 55;
		Rotation->Yaw += 55;
		Rotation->Roll += 55;

	}

}