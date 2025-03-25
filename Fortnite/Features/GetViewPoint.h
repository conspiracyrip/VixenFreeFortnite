#pragma once
// https://github.com/percpopper/UE-Freecam/blob/main/dllmain.cpp
// thanks bae
// 

void(*GetViewPointOriginal)(ULocalPlayer*, FMinimalViewInfo*, void*) = nullptr; // ULocalPlayer*, FMinimalViewInfo*, EStereoscopicPass
void GetViewPoint(ULocalPlayer* LocalPlayer, FMinimalViewInfo* OutViewInfo, void* StereoPass)
{
	GetViewPointOriginal(LocalPlayer, OutViewInfo, StereoPass);

	if (Variables::GetViewPoint::bApplyModifiedData && Variables::GetViewPoint::Location)
	{
		// apply location if given and requested
		OutViewInfo->Location = Variables::GetViewPoint::Location;
	}

	if (Variables::GetViewPoint::bApplyModifiedData && !Variables::GetViewPoint::bSkipRotationForModification && Variables::GetViewPoint::Rotation)
	{
		// apply rotation if requested and given
		OutViewInfo->Rotation = Variables::GetViewPoint::Rotation;
	}


	
}