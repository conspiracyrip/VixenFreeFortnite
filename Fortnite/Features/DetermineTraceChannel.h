#pragma once

__int64 ( *DetermineTraceChannelOriginal )( __int64 , __int64 , char );
__int64 DetermineTraceChannel( __int64 a1 , __int64 a2 , char a3 )
{
	__int64 ReturnValue = DetermineTraceChannelOriginal( a1 , a2 , a3 );

	if ( Settings::Exploits::ShootThroughWalls )
	{
		return 27;
	}

	return ReturnValue;
}