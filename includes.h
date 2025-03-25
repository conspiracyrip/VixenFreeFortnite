#pragma once
//#include <iostream>
//#include <windows.h>
//#include <cstdio>
//#include <consoleapi.h>
//#include <intrin.h>
#include <Memory/Safety/Imports.h>

// Modules
uintptr_t FortniteClientWin64Shipping = 0;
uintptr_t MainModule = 0;

#include <string>

namespace Offsets
{
	int GameViewport = 0;

	// Global Arrays
	int bEnableRecoilDelay = 0x1c64;
	int GObjects = 0; // chams
	int GNames = 0; // chams
	int StaticFindObject = 0;
	int GameInstance = 0;
	// Virtual Indexs
	int GetCameraViewPoint = 90; // idk if its right on sigma !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	int FireSingle = 274;
	int TryToFire = 170; 
	int GetDamageStartLocation = 250;  // old: 249
	int GetSearchTime = 0; // OFFSET IS CORRECT
	int GetControlRotation = 237; // OFFSET IS CORRECT
	int DetermineTraceChannel = 388; // I DONT KNOW IF THIS IS CORRECT BUT NO CRASH SO IDC
	int GetWeaponTargetingTransform = 260; // OFFSET IS CORRECT
	int GetPlayerViewPoint = 256; // IDK IF ITS RIGHT -> 255
	//int HasInfiniteAmmo = 299; // OFFSET IS CORRECT

	int GetWeaponTargetingTransformTest = 0; // test
	int GetDamageStartLocationTest = 249;  // old: 249

	int GetPlayerViewPointTest = 255; // test
	int GetCameraViewPointTest = 89; // old: 89
}

namespace Offsets::ExploitOffsets
{
	int TargetedFortPawn = 0x1990;
	int bFixAimAssistDeadzoneExploit = 0x52c;
	int bADSWhileNotOnGround = 0x5869;
	int CharacterGender = 0x5c0;
	int LastFireTime = 0x1054;
	int LastFireTimeVerified = 0x1058;
	int TriggerType = 0x294;
	int CurrentWeapon = 0xa80;
	int WeaponData = 0x5e8;
	int RelativeRotation = 0x150;
	int CustomTimeDilation = 0x68;
	int GlobalAnimRateScale = 0xa38;
	int bIsReloadingWeapon = 0;
	int bIsTargeting = 0;
	int bCooldownWhileOverheated = 0x2138;
	int OnSetFirstPersonCamera = 0x1158;
	int Wireframe = 0x1d8;
	int bDisableDepthTest = 0x1c8;
	int CurrentVehicle = 0x2b50;
	int ReticleSpreadZeroingDistanceFromMuzzle = 0x1c80; 
	int FirstShotAccuracyMinWaitTime = 0x1be0;
	int BulletCountForPerBulletRecoil = 0x1be4;
	int BulletCountForPerBulletSpread = 0x1be8;
	int TimeRemainingForBulletSpread = 0x1bec;
	int bUseOverrideBaseSpread = 0x1869;
	int bUseOverrideSpreadDownsightsMultiplier = 0x1869;
	int bReticleCornerOutsideSpreadRadius = 0x249;
	int bShouldUsePerfectAimWhenTargetingMinSpread = 0x480;
	int ServerFuel = 0x128;
	int CachedFuelComponent = 0x1388;
	int DragCoefficient2 = 0x4c0;
	int FortPhysicsVehicleConfigs = 0x1c80;
	int bIsJumping = 0xa78;
	int bIsSliding = 0x71b;
	int bThankedBusDriver = 0x13d5;
	int WeaponReloadMontage = 0x15c0;
	int ReloadAnimation = 0x1590;
}

#include <Memory/Safety/SKCrypter.h>
#include <Memory/Custom/NoCRT.h>
#include <Memory/Signature.h>

#include <Memory/VTableHook.h>
Hook::VTableHook ViewportClientHook;

#include <Fortnite/Engine/Engine.h>
#include <Fortnite/Engine/Classes.h>

#include <Memory/Custom/Map.h>
#include <Memory/Custom/Vector.h>

static pair<int, UMaterialInterface*> Indexs;

namespace Variables
{
	FName MaterialParameter4;
	// Variables
	UCanvas* Canvas = nullptr;
	APlayerController* PlayerController = nullptr;
	UWorld* World = nullptr;
	APlayerPawn_Athena_C* AcknowledgedPawn = nullptr;
	UGameViewportClient* ViewportClient = nullptr;

	// Fonts
	UFont* Roboto = nullptr;
	UFont* MediumFont = nullptr;

	// Vehicle Related

	FVector VehiclePos = FVector();


	// Rendering
	static bool DrawMenu = true;
	double FramesPerSecond = 0.0;
	unsigned __int64 CurrentFrame = 0;
	FLinearColor Rainbow = FLinearColor( );
	static FVector2D ScreenCenter = FVector2D( );
	static FVector2D ScreenSize = FVector2D( );
	static FVector2D ScreenTopLeft = FVector2D();

	//Camera
	static FVector CameraLocation = FVector( );
	static FRotator CameraRotation = FRotator( );
	static float FieldOfView = 0.f;

	namespace GetViewPoint
	{
		static FVector Location = FVector();
		static FRotator Rotation = FRotator();
		static bool bApplyModifiedData = false;
		static bool bSkipRotationForModification = false;
	}

	namespace BulletTraces
	{
		static AFortWeapon* CurrentWeapon;
		static bool bEnableBulletTraces = false;
		static FLinearColor BulletTraceColor = FLinearColor(0.f, 1.f, 1.f, 1.f);

		static int PlacedShotsIndex = 0;

		static FVector WorldTraceLocations3D[5];
		static FVector MuzzleWorldTraceLocations3D[5];

	}

	namespace GetWeaponTargetingTransform
	{
		static bool bSilentAimActive = false;
		static FVector SilentLocationTarget = FVector( );
	}

	namespace GetCameraViewPoint
	{
		static bool bFreecamActive = false;
		static FRotator ModdedStickyRotation = FRotator();
		static FRotator LastStickyRotation = FRotator();
		static FVector StickyLocation = FVector();
		static FVector CachedFreecamLocation = FVector();
	}

	namespace GetPlayerViewPoint
	{
		static bool bSilentAimActive = false;
		static bool bShouldStartAtLocationTarget = false;
		static FVector OriginalLocation = FVector();
		static FRotator OriginalRotation = FRotator();

		static FVector SilentLocationTarget = FVector();

		static FVector LastTargetLocation = FVector();
		static FRotator LastTargetRotation = FRotator();

	}

	namespace GetControlRotation
	{
		static bool bAimbotActive = false;
		static FRotator AimbotRotationTarget = FRotator( );
	}
}

namespace Keys
{
	FKey LeftMouseButton;
	FKey RightMouseButton;
	FKey GamepadLeftTrigger;
	FKey Insert;
	FKey F8;
	FKey F7;
	FKey W;
	FKey A;
	FKey S;
	FKey D;
	FKey X;
	FKey Right;
	FKey SpaceBar;
	FKey LeftShift;
	FKey LeftAlt;
	FKey Capslock;
	FKey Tab;

	namespace Other
	{
		// main keys
		FKey B;
		FKey C;
		FKey E;
		FKey F;
		FKey G;
		FKey H;
		FKey I;
		FKey J;
		FKey K;
		FKey L;
		FKey M;
		FKey N;
		FKey O;
		FKey P;
		FKey Q;
		FKey R;
		FKey T;
		FKey U;
		FKey V;
		FKey Y;
		FKey Z;

		// mouse

		FKey ThumbMouseButton;
		FKey ThumbMouseButton2;
		FKey MiddleMouseButton;
		FKey Gamepad_LeftTrigger;
		FKey Gamepad_FaceButton_Right;
		// misc

		FKey Underscore;
		FKey Period;
		FKey Slash;
	}
}

namespace Classes
{
	UObject* MarkerClass = nullptr;
	UObject* PlayerPawn = nullptr;
	UObject* FortPickup = nullptr;
	UObject* BuildingTrap = nullptr;
	UObject* BuildingContainer = nullptr;
	UObject* BuildingWeakSpot = nullptr;
	UObject* AthenaSuperDingo = nullptr;
	UObject* FortAthenaVehicle = nullptr;
	UObject* AthenaSupplyDrop = nullptr;
	UObject* FortWeaponRanged = nullptr;
	UObject* FortWeapon = nullptr;
	UObject* WeaponPickaxeAthena = nullptr;
	UObject* BaseWeaponStats = nullptr;
	UObject* Material = nullptr;
	UObject* Texture = nullptr;
	UObject* SoundBase = nullptr;
	UObject* LocalPlayer = nullptr;
	UObject* PlayerController = nullptr;
	UObject* GameViewportClient = nullptr;
	UObject* PlayerCameraManager = nullptr;
	UObject* DynamicMaterialInstance = nullptr;
	UObject* ActorPlayerNamePrivate = nullptr;

}

#include <Fortnite/Settings.h>
#include <Fortnite/Features/GetSearchTime.h>
#include <Fortnite/Features/HasInfiniteAmmo.h>
#include <Fortnite/Features/GetControlRotation.h>
#include <Fortnite/Features/DetermineTraceChannel.h>
#include <Fortnite/Features/GetWeaponTargetingTransform.h>
#include <Fortnite/Features/GetPlayerViewPoint.h>
#include <Fortnite/Features/GetCameraViewPoint.h>
#include <Fortnite/Features/GetDamageStartLocation.h>
#include <Fortnite/Features/FireSingle.h>
#include <Fortnite/Features/TryToFire.h>
#include <Fortnite/Features/Cosmetics.h>
#include <Fortnite/Features/GetViewPoint.h>

#include <Fortnite/Framework/Wrapper.h>
#include <Fortnite/Framework/Framework.h>

#include <Fortnite/Exploits.h>
#include <Fortnite/Features/DrawTransition.h>