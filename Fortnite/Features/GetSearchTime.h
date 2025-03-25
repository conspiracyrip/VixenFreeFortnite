#pragma once

float ( *GetSearchTimeOriginal )( ABuildingActor* );
float GetSearchTime( ABuildingActor* ContainerActor )
{
	float ReturnValue = GetSearchTimeOriginal( ContainerActor );

	if ( Settings::Exploits::InstantInteraction )
	{
		return FLT_MAX;
	}

	return ReturnValue;
}