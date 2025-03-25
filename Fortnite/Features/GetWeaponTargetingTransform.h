#pragma once

#if 0
double* ( *GetWeaponTargetingTransformOriginal )( AFortWeapon* , double* , double* );
double* GetWeaponTargetingTransform( AFortWeapon* CurrentWeapon , double* StartLocation , double* Angles )
{
	//WindowsAPI::MessageBoxA(0, "guess what.", 0, 0);
	if (CurrentWeapon)
	{
		FString Name = StaticClasses::KismetSystemLibrary->GetObjectName(CurrentWeapon);
		WindowsAPI::MessageBoxW(0, Name.c_str(), L"name", 0);
	}

	double* ReturnValue = GetWeaponTargetingTransformOriginal( CurrentWeapon , StartLocation , Angles );
	if ( Variables::GetWeaponTargetingTransform::bSilentAimActive && Variables::GetWeaponTargetingTransform::SilentLocationTarget )
	{
		Angles[ 0 ] = Variables::GetWeaponTargetingTransform::SilentLocationTarget.X - StartLocation[ 0 ];
		Angles[ 1 ] = Variables::GetWeaponTargetingTransform::SilentLocationTarget.Y - StartLocation[ 1 ];
		Angles[ 2 ] = Variables::GetWeaponTargetingTransform::SilentLocationTarget.Z - StartLocation[ 2 ];
	}

	return ReturnValue;
}
#endif

double* (*GetWeaponTargetingTransformOriginal)(AFortWeapon*, void*, void*);
double* GetWeaponTargetingTransform(AFortWeapon* CurrentWeapon, void* a2, void* a3)
{
	//WindowsAPI::MessageBoxA(0, "guess what.", 0, 0);
	__try
	{
		if (CurrentWeapon)
		{

			//if (CurrentWeapon + 0x1b30)
			//{
			//	*(EFortAbilityTargetingSource*)(CurrentWeapon + 0x1b30) = EFortAbilityTargetingSource::EFortAbilityTargetingSource__Camera;
			//}


			FString Name = StaticClasses::KismetSystemLibrary->GetObjectName(CurrentWeapon);
			WindowsAPI::MessageBoxW(0, Name.c_str(), L"a1", 0);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {};

	__try
	{
		if (a2)
		{
			FString Name = StaticClasses::KismetSystemLibrary->GetObjectName(reinterpret_cast<UObject*>(a2));
			WindowsAPI::MessageBoxW(0, Name.c_str(), L"a2", 0);
		}


		//if (CurrentWeapon && Variables::GetWeaponTargetingTransform::bSilentAimActive && Variables::GetWeaponTargetingTransform::SilentLocationTarget)
		//{
			// 0x1008 - CurrentDamageStartLocation
			//if (CurrentWeapon + 0x1008)
			//{
			//	*(FVector*)(CurrentWeapon + 0x1008) = Variables::GetWeaponTargetingTransform::SilentLocationTarget;
			//}
		//}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {};

	__try
	{
		if (a2)
		{
			WindowsAPI::MessageBoxW(0, std::to_wstring(uintptr_t(a2)).c_str(), L"a2", 0);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {};

	__try
	{
		if (a3)
		{
			FString Name = StaticClasses::KismetSystemLibrary->GetObjectName(reinterpret_cast<UObject*>(a3));
			WindowsAPI::MessageBoxW(0, Name.c_str(), L"a3", 0);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {};

	__try
	{
		if (a3)
		{
			WindowsAPI::MessageBoxW(0, std::to_wstring(uintptr_t(a3)).c_str(), L"a3", 0);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {};

	__try
	{
		double* ReturnValue = GetWeaponTargetingTransformOriginal(CurrentWeapon, a2, a3);
		
		__try
		{
			if (ReturnValue)
			{
				WindowsAPI::MessageBoxW(0, std::to_wstring(uintptr_t(ReturnValue)).c_str(), L"ReturnValue1", 0);
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {};


		__try
		{
			if (ReturnValue)
			{
				FString Name = StaticClasses::KismetSystemLibrary->GetObjectName(reinterpret_cast<UObject*>(ReturnValue));
				WindowsAPI::MessageBoxW(0, Name.c_str(), L"ReturnValue3", 0);
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {};


		/*
		if (Variables::GetWeaponTargetingTransform::bSilentAimActive && Variables::GetWeaponTargetingTransform::SilentLocationTarget)
		{
			Angles[0] = Variables::GetWeaponTargetingTransform::SilentLocationTarget.X - StartLocation[0];
			Angles[1] = Variables::GetWeaponTargetingTransform::SilentLocationTarget.Y - StartLocation[1];
			Angles[2] = Variables::GetWeaponTargetingTransform::SilentLocationTarget.Z - StartLocation[2];
		}
		*/

		return ReturnValue;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {};

	return 0;
}