#pragma once
#include <chrono>
#include <cmath> 
#include <cfloat>
#include <fstream>

#include "../DrawMenu.h"

#define GRD_TO_BOG(deg) ((deg) * M_PI / 180.0)  // Degrees to radians conversion
#define BOG_TO_GRD(rad) ((rad) * 180.0 / M_PI)  // Radians to degrees conversion

template<typename T>
inline T* Get(void* This, int32_t Offset) 
{
	return reinterpret_cast<T*>(uintptr_t(This) + Offset); 
}

inline bool FileExists(const std::string& name)
{
	if (FILE* file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

namespace VixenData
{
	FString User_Username;
}


const pair<wchar_t* , wchar_t*> BonePairs[ ]
{
	{L"neck_01", L"upperarm_r"},
	{L"neck_01", L"upperarm_l"},
	{L"upperarm_l", L"lowerarm_l"},
	{L"lowerarm_l", L"hand_l"},
	{L"upperarm_r", L"lowerarm_r"},
	{L"lowerarm_r", L"hand_r"},
	{L"neck_01", L"pelvis"},
	{L"pelvis", L"thigh_r"},
	{L"pelvis", L"thigh_l"},
	{L"thigh_r", L"calf_r"},
	{L"calf_r", L"ik_foot_r"},
	{L"thigh_l", L"calf_l"},
	{L"calf_l", L"ik_foot_l"}
};

bool is_bad_write_ptr(void* ptr, unsigned long long size)
{
	return ptr ? false : true;
}

bool valid_pointer(uintptr_t address)
{
	if (!is_bad_write_ptr((void*)address, (unsigned long long)8))
	{
		return 1;
	}

	return 0;
}


const pair<wchar_t* , wchar_t*> HitscanPairs[ ]
{
	{L"head", L"neck_01"},
	{L"neck_01", L"neck_02"},
	{L"neck_02", L"spine_01"},
	{L"neck_02", L"upperarm_r"},
	{L"neck_02", L"upperarm_l"},
	{L"upperarm_l", L"lowerarm_l"},
	{L"upperarm_r", L"lowerarm_r"},
	{L"lowerarm_l", L"hand_l"},
	{L"lowerarm_r", L"hand_r"},
	{L"pelvis", L"thigh_r"},
	{L"pelvis", L"thigh_l"},
	{L"thigh_r", L"calf_r"},
	{L"calf_r", L"ik_foot_r"},
	{L"thigh_l", L"calf_l"},
	{L"calf_l", L"ik_foot_l"}
};

#define INPUT_MOUSE    0
#define MOUSEEVENTF_MOVE 0x0001



void set_aim_mouse(APlayerController* controller, FVector aim_location, float width, float height, float smooth) {
	// Static variable to hold the last execution time
	static auto lastExecutionTime = std::chrono::steady_clock::now();

	// Get the current time
	auto currentTime = std::chrono::steady_clock::now();

	// If 10ms haven't passed since the last execution, return early
	if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastExecutionTime).count() < 10) {
		return;
	}

	// Update the last execution time
	lastExecutionTime = currentTime;

	// Check if the controller pointer is valid
	if (!controller) {
		return;
	}

	// Check if width and height are valid
	if (width <= 0 || height <= 0) {
		return;
	}

	// Validate aim_location values
	if (!std::isfinite(aim_location.X) || !std::isfinite(aim_location.Y) || !std::isfinite(aim_location.Z)) {
		return;
	}

	// Ensure smooth is valid and non-zero
	if (smooth <= 0) {
		return;
	}

	// Calculate screen center
	float ScreenCenterX = width / 2;
	float ScreenCenterY = height / 2;
	float TargetX = 0;
	float TargetY = 0;

	FVector2D aim_screen;

	// Ensure the WorldToScreen function succeeds
	if (!controller->w2s(aim_location, &aim_screen)) {
		return;
	}

	// Additional check for aim_screen validity
	if (!std::isfinite(aim_screen.X) || !std::isfinite(aim_screen.Y)) {
		return;
	}

	if (aim_screen.X == 0 && aim_screen.Y == 0) {
		return; // If both X and Y are zero, return early
	}

	// Calculate the mouse movement based on screen position
	if (aim_screen.X != 0) {
		if (aim_screen.X > ScreenCenterX) {
			TargetX = -(ScreenCenterX - aim_screen.X) / smooth;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}
		else {
			TargetX = (aim_screen.X - ScreenCenterX) / smooth;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}

	if (aim_screen.Y != 0) {
		if (aim_screen.Y > ScreenCenterY) {
			TargetY = -(ScreenCenterY - aim_screen.Y) / smooth;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}
		else {
			TargetY = (aim_screen.Y - ScreenCenterY) / smooth;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}

	if (std::isfinite(TargetX) && std::isfinite(TargetY)) {
		WindowsAPI::INPUT input = {};
		input.type = INPUT_MOUSE;
		input.mi.dx = static_cast<WindowsAPI::LONG>(TargetX);
		input.mi.dy = static_cast<WindowsAPI::LONG>(TargetY); 
		input.mi.dwFlags = MOUSEEVENTF_MOVE;

		WindowsAPI::SendInput(1, &input, sizeof(WindowsAPI::INPUT));
	}
}

vector<pair<wchar_t* , wchar_t*>> SkeletonBonePairs( BonePairs , sizeof( BonePairs ) / sizeof( pair<wchar_t* , wchar_t*> ) );
vector<pair<wchar_t* , wchar_t*>> HitscanBonePairs( HitscanPairs , sizeof( HitscanPairs ) / sizeof( pair<wchar_t* , wchar_t*> ) );

void( *DrawTransitionOriginal )( UGameViewportClient* , UCanvas* ) = 0xSKIBIDITOILET; // ud antipaste trust
void DrawTransition( UGameViewportClient* ViewPortClient, UCanvas* Canvas )
{
	if ( !ViewPortClient || !Canvas ) return DrawTransitionOriginal( ViewPortClient , Canvas );

	Variables::CurrentFrame++;

	Variables::Canvas = Canvas;
	Variables::ScreenSize = FVector2D( double( Canvas->ClipX( ) ) , double( Canvas->ClipY( ) ) );
	Variables::ScreenCenter = FVector2D( Variables::ScreenSize.X / 2.0 , Variables::ScreenSize.Y / 2.0 );
	Variables::ScreenTopLeft = FVector2D(Variables::ScreenSize.X - 40.0, Variables::ScreenSize.Y - 40.0);

	FVector LocalHead = FVector( );
	static bool ChamsWasEnabled = false;
	static UMaterial* Material = nullptr;
	static map<int, UMaterialInterface*> StoredElementIndexs;

	bool GetPlayerViewPoint_bShouldStartAtLocationTarget = false;
	FVector GetPlayerViewPoint_SilentLocationTarget = FVector();

	bool Update__bIsUsingSpinbot = false;

	bool Update__GetWeaponTargetingTransform__bSilentAimActive = false;
	FVector Update__GetWeaponTargetingTransform__SilentLocationTarget = FVector();

	bool Update__GetControlRotation__bAimbotActive = false;
	FRotator Update__GetControlRotation__AimbotRotationTarget = FRotator( );

	UWorld* World = reinterpret_cast< UWorld* >( ViewPortClient->World( ) );
	if ( !World ) return DrawTransitionOriginal( ViewPortClient , Canvas );

	

	double WorldDeltaSeconds = StaticClasses::GameplayStatics->GetWorldDeltaSeconds( World );

	UGameInstance* Gameinstance = World->OwningGameInstance( );
	if ( !Gameinstance ) return DrawTransitionOriginal( ViewPortClient , Canvas );

	

	UGameViewportClient* ViewportClient = ViewPortClient;
	if ( !ViewportClient ) return DrawTransitionOriginal( ViewPortClient , Canvas );

	APlayerController* PlayerController = StaticClasses::GameplayStatics->GetPlayerControllerFromID(World, 0);
	if (!PlayerController)
	{
		PlayerController = StaticClasses::GameplayStatics->GetPlayerController(World, 0); //LocalPlayer->PlayerController( );
	}

	if (!PlayerController) return DrawTransitionOriginal(ViewPortClient, Canvas);
	

	Variables::PlayerController = PlayerController;
	
	
	Exploits::Tick( PlayerController );

	//if ( PlayerController->IsInputKeyDown( Keys::F8 ) )
	//{
	//	ViewportClientHook.RevertHook( &DrawTransitionOriginal , ViewportClient );
	//}

	APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager();
	if ( !PlayerCameraManager ) return DrawTransitionOriginal( ViewPortClient , Canvas );

	APlayerPawn_Athena_C* AcknowledgedPawn = PlayerController->AcknowledgedPawn( );

	//Variables::CameraLocation = World->GetCameraLocation( );
	//Variables::CameraRotation = World->GetCameraRotation( );
	Variables::CameraLocation = PlayerCameraManager->GetCameraLocation(); // World->GetCameraLocation();
	Variables::CameraRotation = PlayerCameraManager->GetCameraRotation(); // World->GetCameraRotation();
	Variables::FieldOfView = PlayerCameraManager->GetFOVAngle();

	///USceneViewState* ViewState = LocalPlayer->GetViewState( );
	//float FieldOfView = StaticClasses::KismetMathLibrary->atan( 1.0 / *( double* ) ( ViewState + 0x700 ) ) * 2.f;

	double AimbotFOV = Settings::Exploits::ThreeSixtyFOV ? DBL_MAX : ( Settings::Aimbot::FOV * Variables::ScreenSize.X / Variables::FieldOfView ) / 2.0;
	//Variables::FramesPerSecond = 1.0 / WorldDeltaSeconds;
#if !NOT_STRIPPED_FROM_PROD
	if (Settings::Skins::bDefaultCharacterEnabled && !Settings::Skins::bApplySkinWithKey)
	{
		if (Settings::Skins::bCachedHasAppliedCharacter && !AcknowledgedPawn)
		{
			Settings::Skins::bCachedHasAppliedCharacter = false;
		}
		else if (!Settings::Skins::bCachedHasAppliedCharacter && AcknowledgedPawn && PlayerController)
		{
			Settings::Skins::bCachedHasAppliedCharacter = true;

			if (Settings::Skins::DefaultCharacter.c_str())
			{
				SetCosmeticMulti(
					PlayerController,
					(Settings::Skins::DefaultCharacter.c_str()),
					(Settings::Skins::Backpack.c_str()),
					(Settings::Skins::Pickaxe.c_str()),
					(Settings::Skins::Glider.c_str()),
					(Settings::Skins::Contrail.c_str())
				);
			}
		}
	} 
	
	if (Settings::Skins::bApplySkinWithKey)
	{
		if (Settings::Skins::bCachedHasAppliedCharacter && !AcknowledgedPawn)
		{
			Settings::Skins::bCachedHasAppliedCharacter = false;
		}
		else if (!Settings::Skins::bCachedHasAppliedCharacter && AcknowledgedPawn && PlayerController)
		{
			Settings::Skins::bCachedHasAppliedCharacter = true;

			if (PlayerController->WasInputKeyJustPressed(Keys::F7))
			{
				if (Settings::Skins::DefaultCharacter.c_str())
				{
					SetCosmeticMulti(
						PlayerController,
						(Settings::Skins::DefaultCharacter.c_str()),
						(Settings::Skins::Backpack.c_str()),
						(Settings::Skins::Pickaxe.c_str()),
						(Settings::Skins::Glider.c_str()),
						(Settings::Skins::Contrail.c_str())
					);
				}
			}
		}
	}
#endif

	if (Settings::Aimbot::NoReload && AcknowledgedPawn && AcknowledgedPawn->CurrentWeapon() && AcknowledgedPawn->Mesh())
	{

		auto CurrentWeapon = AcknowledgedPawn->CurrentWeapon();
		if (CurrentWeapon)
		{
			bool bIsReloadingWeapon = CurrentWeapon->IsReloading();

			if (bIsReloadingWeapon)
			{
				AcknowledgedPawn->Mesh()->SetGlobalAnimRateScale(999);
				if (CurrentWeapon->IsReloading())
				{
					bool bIsTargeting = CurrentWeapon->IsTargeting();
					
					if (bIsTargeting)
					{
						CurrentWeapon->SetTargeting(bIsTargeting);
					}

					CurrentWeapon->SetIsReloadingWeapon(false);
				}
			}
			else
			{
				if (CurrentWeapon->IsReloading())
				{
					CurrentWeapon->SetIsReloadingWeapon(false);
					
					bool bIsTargeting = CurrentWeapon->IsTargeting();

					if (bIsTargeting)
					{
						CurrentWeapon->SetTargeting(bIsTargeting);
					}
				}

				AcknowledgedPawn->Mesh()->SetGlobalAnimRateScale(1);
			}
		}
	}

	static bool bAppliedNoRecoil = false;
	if (Settings::Aimbot::NoRecoil && PlayerController && AcknowledgedPawn)
	{
		bAppliedNoRecoil = true;
		*(float*)(PlayerController + Offsets::ExploitOffsets::CustomTimeDilation) = -1;
	}
	else if (bAppliedNoRecoil && PlayerController && AcknowledgedPawn)
	{
		bAppliedNoRecoil = false;
		*(float*)(PlayerController + Offsets::ExploitOffsets::CustomTimeDilation) = 1;
	}

	if (Variables::DrawMenu)
	{
		if (AcknowledgedPawn && PlayerController)
		{
			AcknowledgedPawn->DisableInput(PlayerController);
		}
		else if (!AcknowledgedPawn && PlayerController)
		{
			PlayerController->EnableInput(PlayerController);
		}
	}
	else
	{
		if (AcknowledgedPawn && PlayerController)
		{
			AcknowledgedPawn->EnableInput(PlayerController);
		}
		else if (!AcknowledgedPawn && PlayerController)
		{
			PlayerController->EnableInput(PlayerController);
		}
	}


	if ( AcknowledgedPawn )
	{
		/*if (Variables::DrawMenu)
		{
			AcknowledgedPawn->DisableInput(PlayerController);
		}
		else
		{
			AcknowledgedPawn->EnableInput(PlayerController);
		}*/

		USkeletalMeshComponent* Mesh = AcknowledgedPawn->Mesh( );
		if ( Mesh )
		{
			LocalHead = Custom::GetSocketLocation( Mesh , FString( L"head" ) );
		}

		

		if (AcknowledgedPawn && AcknowledgedPawn->IsInVehicle() || AcknowledgedPawn->IsDrivingVehicle())
		{
			if (!AcknowledgedPawn->IsPassengerInVehicle())
			{
				AFortAthenaVehicle* CurrentVehicle = AcknowledgedPawn->CurrentVehicle();

				if (Settings::Exploits::CarFly)
				{
					if (Settings::Exploits::TeleportMethod == 0)
					{
						bool bIsAnyKeyDown = false;

						FVector CurrentLocation = CurrentVehicle->K2_GetActorLocation();
						if (PlayerController->IsInputKeyDown(Keys::W)) {
							CurrentLocation = CurrentLocation + StaticClasses::KismetMathLibrary->GetForwardVector(Variables::CameraRotation) * Settings::Exploits::CarSpeed;
							CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
							bIsAnyKeyDown = true;
						}
						if (PlayerController->IsInputKeyDown(Keys::A)) {
							CurrentLocation = CurrentLocation - StaticClasses::KismetMathLibrary->GetRightVector(Variables::CameraRotation) * Settings::Exploits::CarSpeed;
							CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
							bIsAnyKeyDown = true;
						}
						if (PlayerController->IsInputKeyDown(Keys::S)) {
							CurrentLocation = CurrentLocation - StaticClasses::KismetMathLibrary->GetForwardVector(Variables::CameraRotation) * Settings::Exploits::CarSpeed;
							CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
							bIsAnyKeyDown = true;
						}
						if (PlayerController->IsInputKeyDown(Keys::D))
						{
							CurrentLocation = CurrentLocation + StaticClasses::KismetMathLibrary->GetRightVector(Variables::CameraRotation) * Settings::Exploits::CarSpeed;
							CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
							bIsAnyKeyDown = true;
						}

						if (PlayerController->IsInputKeyDown(Keys::SpaceBar))
						{
							CurrentLocation.Z = CurrentLocation.Z + Settings::Exploits::CarSpeed;
							CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
							bIsAnyKeyDown = true;
						}

						if (PlayerController->IsInputKeyDown(Keys::LeftShift))
						{
							CurrentLocation.Z = CurrentLocation.Z - Settings::Exploits::CarSpeed;
							CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
							bIsAnyKeyDown = true;
						}

						Variables::VehiclePos = CurrentLocation;
						if (!bIsAnyKeyDown)
						{
							if (Settings::Exploits::ServersideVehicleSpinbot && AcknowledgedPawn)
							{
								static double VehicleSpin = 0;

								if (VehicleSpin >= 359)
								{
									VehicleSpin = 0;
								}

								VehicleSpin += Settings::Exploits::SpinSpeed;

								CurrentVehicle->K2_TeleportTo(CurrentLocation, FRotator(0, VehicleSpin, 0));
								CurrentVehicle->K2_SetActorRotation(FRotator(0, VehicleSpin, 0), true);
							}
							else {
								CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
							}
						}
					}
					else if (Settings::Exploits::TeleportMethod == 1 && PlayerController->WasInputKeyJustPressed(Keys::F8))
					{
						double ClosestMarker = DBL_MAX;
						AFortPlayerMarkerBase* TargetMarker = nullptr;

						TArray<UObject*> MarkerArray = StaticClasses::GameplayStatics->GetAllActorsOfClass(World, Classes::MarkerClass);
						for (int i = 0; i < MarkerArray.NumElements; i++)
						{
							AFortPlayerMarkerBase* MarkerActor = reinterpret_cast<AFortPlayerMarkerBase*>(MarkerArray[i]);
							if (!MarkerActor) continue;

							bool MarkerIsA = StaticClasses::GameplayStatics->ObjectIsA(MarkerActor, Classes::MarkerClass);
							if (!MarkerIsA) continue;

							FVector WorldLocation = MarkerActor->K2_GetActorLocation();
							double Distance = StaticClasses::KismetMathLibrary->Vector_Distance(Variables::CameraLocation, WorldLocation) * 0.01;
							if (Distance < ClosestMarker)
							{
								ClosestMarker = Distance;
								TargetMarker = MarkerActor;
							}
						}

						if (TargetMarker)
						{
							FVector WorldLocation = TargetMarker->K2_GetActorLocation();
							FVector CurrentActorLocation = AcknowledgedPawn->K2_GetActorLocation();

							if (WorldLocation && CurrentActorLocation)
							{
								WorldLocation.Z = CurrentActorLocation.Z * 2;

								CurrentVehicle->K2_TeleportTo(WorldLocation, Variables::CameraRotation);
							}
						}
					}
					else if (Settings::Exploits::TeleportMethod == 2 && PlayerController)
					{
						FVector TargetLoc = PlayerController->GetLocationUnderReticle();
						FVector2D TargetLoc2D = FVector2D();

						if (PlayerController->w2s(TargetLoc, &TargetLoc2D))
						{
							Wrapper::Circle(TargetLoc2D, FLinearColor(1.f, 0.f, 0.8f, 1.f), 10, 50, false, true);
						}

						if (PlayerController->WasInputKeyJustPressed(Keys::F8))
						{
							CurrentVehicle->K2_TeleportTo(TargetLoc, Variables::CameraRotation);
						}
					}
					else if (Settings::Exploits::TeleportMethod == 4)
					{

						static FVector TargetLocation;

						bool bIsAnyKeyDown = false;

						FVector ActualCurrentLocation = CurrentVehicle->K2_GetActorLocation();
						FVector CurrentLocation = CurrentVehicle->K2_GetActorLocation();

						if (PlayerController->IsInputKeyDown(Keys::W)) {
							CurrentLocation = CurrentLocation + StaticClasses::KismetMathLibrary->GetForwardVector(Variables::CameraRotation) * Settings::Exploits::CarSpeed;
							CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
							bIsAnyKeyDown = true;
						}
						if (PlayerController->IsInputKeyDown(Keys::A)) {
							CurrentLocation = CurrentLocation - StaticClasses::KismetMathLibrary->GetRightVector(Variables::CameraRotation) * Settings::Exploits::CarSpeed;
							CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
							bIsAnyKeyDown = true;
						}
						if (PlayerController->IsInputKeyDown(Keys::S)) {
							CurrentLocation = CurrentLocation - StaticClasses::KismetMathLibrary->GetForwardVector(Variables::CameraRotation) * Settings::Exploits::CarSpeed;
							CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
							bIsAnyKeyDown = true;
						}
						if (PlayerController->IsInputKeyDown(Keys::D))
						{
							CurrentLocation = CurrentLocation + StaticClasses::KismetMathLibrary->GetRightVector(Variables::CameraRotation) * Settings::Exploits::CarSpeed;
							CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
							bIsAnyKeyDown = true;
						}

						if (PlayerController->IsInputKeyDown(Keys::SpaceBar))
						{
							CurrentLocation.Z = CurrentLocation.Z + Settings::Exploits::CarSpeed;
							CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
							bIsAnyKeyDown = true;
						}

						if (PlayerController->IsInputKeyDown(Keys::LeftShift))
						{
							CurrentLocation.Z = CurrentLocation.Z - Settings::Exploits::CarSpeed;
							CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
							bIsAnyKeyDown = true;
						}


						FVector NewLocation = StaticClasses::KismetMathLibrary->VInterpTo(ActualCurrentLocation, CurrentLocation, WorldDeltaSeconds, Settings::Exploits::CarSpeed);
						CurrentVehicle->K2_TeleportTo(NewLocation, Variables::CameraRotation);
					}
				}

				if (Settings::Exploits::CarSpeedHack && AcknowledgedPawn)
				{
					AFortAthenaVehicle* CurrentVehicle = AcknowledgedPawn->CurrentVehicle();
					if (CurrentVehicle) {
						UFortPhysicsVehicleConfigs* FortPhysicsVehicleConfigs = CurrentVehicle->FortPhysicsVehicleConfigs();
						if (FortPhysicsVehicleConfigs)
						{
							if (PlayerController->IsInputKeyDown(Keys::W))
							{
								FortPhysicsVehicleConfigs->SetDragCoefficient2(-0.00009999999750f);
							}
							else
							{
								FortPhysicsVehicleConfigs->SetDragCoefficient2(0.00009999999747f);
							}
						}
					}

				}

				if (Settings::Exploits::InfiniteFuel && AcknowledgedPawn)
				{
					AFortAthenaVehicle* CurrentVehicle = AcknowledgedPawn->CurrentVehicle();
					if (CurrentVehicle)
					{
						UFortVehicleFuelComponent* CachedFuelComponent = CurrentVehicle->CachedFuelComponent();
						if (CachedFuelComponent)
						{
							CachedFuelComponent->SetServerFuel(3.402823466e+38F); // flt_max
							CachedFuelComponent->SetInfiniteFuel(3.402823466e+38F); // flt_max
						}
					}
				}
			}
		}

		if (!Settings::Exploits::CarFly) {
			if (Settings::Exploits::ServersideVehicleSpinbot && AcknowledgedPawn && !Settings::Exploits::CarFly)
			{
				AFortAthenaVehicle* CurrentVehicle = AcknowledgedPawn->CurrentVehicle();
				if (CurrentVehicle)
				{
					static double VehicleSpin = 0;

					VehicleSpin += Settings::Exploits::SpinSpeed;

					CurrentVehicle->K2_SetActorRelativeRotation(FRotator(0, VehicleSpin, 0), true);
				}
			}
		}

		if ( Settings::Player::Spectators )
		{
			
			APlayerState* PlayerState = AcknowledgedPawn->PlayerState( );
			if ( PlayerState )
			{
				int SpectatorCount = PlayerState->GetSpectatorCount( );

				float Red , Green = 0.f;
				if ( SpectatorCount >= 2 )
				{
					Red = StaticClasses::KismetMathLibrary->Clamp( static_cast< double >( SpectatorCount ) / 3.0 , 0.0f , 1.0f );
					Green = StaticClasses::KismetMathLibrary->Clamp( 1.0 - ( static_cast< double >( SpectatorCount ) / 3.0 ) , 0.0f , 1.0f );
				}
				else
				{
					Red = StaticClasses::KismetMathLibrary->Clamp( static_cast< double >( SpectatorCount - 3 ) / 7.0 , 0.0f , 1.0f );
					Green = StaticClasses::KismetMathLibrary->Clamp( 1.0 - ( static_cast< double >( SpectatorCount - 3 ) / 7.0 ) , 0.0f , 1.0f );
				}

				FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Int( FString( L"Spectator Count: " ) , FString( L"" ) , SpectatorCount , FString( L"" ) );
				Wrapper::Text( ConvertedText , FVector2D( Variables::ScreenCenter.X , Variables::ScreenCenter.Y - 400.0 ) , FLinearColor( Red , Green , 0.0f , 1.0f ) , 15.0 , true );
			}
		}

		if ( Settings::Environment::StormPrediction )
		{
			FVector WorldLocation = FVector( );
			FVector2D ScreenLocation = FVector2D( );
			int32_t SafeZonePhase = StaticClasses::FortKismetLibrary->GetCurrentSafeZonePhase( World );
			StaticClasses::FortKismetLibrary->GetSafeZoneLocation( World , SafeZonePhase , WorldLocation );
			if ( Custom::K2_Project( WorldLocation , &ScreenLocation ) )
			{
				double Distance = StaticClasses::KismetMathLibrary->Vector_Distance( LocalHead , WorldLocation ) * 0.01;
				double FontSize = Custom::InterpolatedValue( 150.0 , Distance , 5.0 , Settings::Misc::FontSize );
				FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double( L"SafeZone" , L" (" , int( Distance ) , L"m)" );
				Wrapper::Text( ConvertedText , ScreenLocation , FLinearColor( 0.8f , 0.52f , 0.25f , 1.0f ) , FontSize , true );
			}
		}

		if ( Settings::Environment::BuildingESP )
		{
			TArray<ABuildingActor*> HighlightedPrimaryBuildings = PlayerController->HighlightedPrimaryBuildings( );
			for ( int32_t i = 0; i < HighlightedPrimaryBuildings.NumElements; i++ )
			{
				ABuildingActor* HighlightedPrimaryBuilding = HighlightedPrimaryBuildings[ i ];
				if ( !HighlightedPrimaryBuilding ) continue;

				FVector2D ScreenLocation = FVector2D( );
				FVector WorldLocation = HighlightedPrimaryBuilding->K2_GetActorLocation( );
				if ( !Custom::K2_Project( WorldLocation , &ScreenLocation ) ) continue;
				if ( !Custom::InScreen( ScreenLocation ) ) continue;

				double Distance = StaticClasses::KismetMathLibrary->Vector_Distance( LocalHead , WorldLocation ) * 0.01;
				double FontSize = Custom::InterpolatedValue( 150.0 , Distance , 5.0 , Settings::Misc::FontSize );
				if ( Distance > Settings::Environment::MaxDistance ) continue;

				bool HasHealthLeft = HighlightedPrimaryBuilding->HasHealthLeft( );
				if ( !HasHealthLeft ) continue;

				bool IsPlayerBuilt = HighlightedPrimaryBuilding->IsPlayerBuilt( );
				if ( !IsPlayerBuilt ) continue;

				if ( Settings::Environment::InstantDestoryBuilding )
				{
					HighlightedPrimaryBuilding->SetHealth( 0.f );
					HighlightedPrimaryBuilding->ForceBuildingHealth( 0.f );
				}

				HighlightedPrimaryBuilding->SetQuestHighlight( true );
				HighlightedPrimaryBuilding->SetSuppressHealthBar( true );

				EFortBuildingType BuildingType = HighlightedPrimaryBuilding->BuildingType( );
				FString BuildName = Custom::GetBuildByType( BuildingType );
				Wrapper::Text( BuildName , FVector2D( ScreenLocation.X , ScreenLocation.Y ) , FLinearColor( 1.f , 1.f , 1.f , 1.f ) );
				double TextOffset = ( FontSize + 2.0 );

				FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Int( FString( L"Team:" ) , FString( L" " ) , HighlightedPrimaryBuilding->TeamIndex( ) , FString( L"" ) );
				Wrapper::Text( ConvertedText , FVector2D( ScreenLocation.X , ScreenLocation.Y + TextOffset ) , FLinearColor( 1.f , 1.f , 1.f , 1.f ) );
				TextOffset += ( FontSize + 2.0 );

				float CurrentHealth = HighlightedPrimaryBuilding->GetHealth( );
				float MaxHealth = HighlightedPrimaryBuilding->GetMaxHealth( );

				double CurrentPercentage = static_cast< double >( CurrentHealth / MaxHealth );
				FVector2D BarSize = FVector2D( 120.0 , 13.0 );

				float Green = 255.f * CurrentPercentage;
				float Red = 255.f - Green;
				Red /= 255.f;
				Green /= 255.f;

				FLinearColor BarColor = FLinearColor( Red , Green , 0.f , 1.f );
				FVector2D BarPosition = FVector2D( ScreenLocation.X , ScreenLocation.Y + ( TextOffset + BarSize.Y ) );

				Wrapper::Rect( BarPosition , BarSize , FromRGB( 35 , 46 , 50 ) , 1.f );
				Wrapper::RectFilled( BarPosition , FVector2D( BarSize.X * CurrentPercentage , BarSize.Y ) , BarColor );
			}
		}

		if ( Settings::Environment::Pickup )
		{
			TArray<UObject*> PickupArray = StaticClasses::GameplayStatics->GetAllActorsOfClass( World , Classes::FortPickup );
			for ( int i = 0; i < PickupArray.NumElements; i++ )
			{
				if ( !PickupArray.Valid( i ) ) continue;

				AFortPickup* PickupActor = reinterpret_cast< AFortPickup* >( PickupArray[ i ] );
				if ( !PickupActor ) continue;

				bool PickupIsA = StaticClasses::GameplayStatics->ObjectIsA( PickupActor , Classes::FortPickup );
				if ( !PickupIsA ) continue;

				FVector2D ScreenLocation = FVector2D( );
				FVector WorldLocation = PickupActor->K2_GetActorLocation( );
				if ( !Custom::K2_Project( WorldLocation , &ScreenLocation ) ) continue;
				if ( !Custom::InScreen( ScreenLocation ) ) continue;

				double Distance = StaticClasses::KismetMathLibrary->Vector_Distance( LocalHead , WorldLocation ) * 0.01;
				double FontSize = Custom::InterpolatedValue( 150.0 , Distance , 5.0 , Settings::Misc::FontSize );
				if ( Distance > Settings::Environment::MaxDistance ) continue;

				UFortItemDefinition* PickupItemDefinition = PickupActor->PrimaryPickupItemEntry( );
				if ( !PickupItemDefinition ) continue;

				EFortRarity Tier = PickupItemDefinition->Rarity( );
				if ( Tier <= Settings::Environment::MinimumTier ) continue;

		
				

				FString DistplayName = PickupItemDefinition->ItemName( ).Get( );
				FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double( DistplayName , L" (" , int( Distance ) , L"m)" );
				Wrapper::Text( ConvertedText , ScreenLocation , Custom::GetColorByTier( Tier ) , FontSize , true );

				if (PlayerController && AcknowledgedPawn) // too lazy to do menu item for this :pluhh:
				{
					if (PlayerController->IsInputKeyDown(Keys::F7))
					{
						FRotator TargetRotation = StaticClasses::KismetMathLibrary->FindLookAtRotation(Variables::CameraLocation, WorldLocation);
						
						FVector TargetTeleLoc = WorldLocation;
						TargetTeleLoc.Z -= 7;

						AcknowledgedPawn->K2_TeleportTo(TargetTeleLoc, TargetRotation);
					}
				}
			}
		}

		if ( Settings::Environment::Container )
		{
			TArray<UObject*> ContainerArray = StaticClasses::GameplayStatics->GetAllActorsOfClass( World , Classes::BuildingContainer );
			for ( int i = 0; i < ContainerArray.NumElements; i++ )
			{
				if ( !ContainerArray.Valid( i ) ) continue;

				ABuildingContainer* ContainerActor = reinterpret_cast< ABuildingContainer* >( ContainerArray[ i ] );
				if ( !ContainerActor ) continue;

				bool ContainerIsA = StaticClasses::GameplayStatics->ObjectIsA( ContainerActor , Classes::BuildingContainer );
				if ( !ContainerIsA ) continue;

				FVector2D ScreenLocation = FVector2D( );
				FVector WorldLocation = ContainerActor->K2_GetActorLocation( );
				if ( !Custom::K2_Project( WorldLocation , &ScreenLocation ) ) continue;
				if ( !Custom::InScreen( ScreenLocation ) ) continue;

				double Distance = StaticClasses::KismetMathLibrary->Vector_Distance( LocalHead , WorldLocation ) * 0.01;
				double FontSize = Custom::InterpolatedValue( 150.0 , Distance , 9.0 , Settings::Misc::FontSize );
				if ( Distance > Settings::Environment::MaxDistance ) continue;
				if ( ContainerActor->bAlreadySearched( ) ) continue;

				if ( Settings::Environment::Chams )
				{
					UMeshComponent* Mesh = ContainerActor->StaticMeshComponent( );
					if ( Mesh )
					{
						Mesh->SetCustomDepthStencilValue( 11 );
						Mesh->SetRenderCustomDepth( true );
					}
				}

				if ( Settings::Exploits::InstantInteraction )
				{
					static ABuildingContainer* OldContainerActor = nullptr;
					if ( ContainerActor != OldContainerActor )
					{
						//Hook::VTableHook().Hook( ContainerActor , GetSearchTime , Offsets::GetSearchTime , &GetSearchTimeOriginal );

						OldContainerActor = ContainerActor;
					}
				}

				FString ObjectName = StaticClasses::KismetSystemLibrary->GetObjectName(ContainerActor);

				if (Settings::Environment::Chests)
				{
					if (StaticClasses::KismetStringLibrary->Contains(ObjectName, FString(L"Tiered_Chest"), true, false) || StaticClasses::KismetStringLibrary->Contains(ObjectName, FString(L"BuriedChest"), true, false))
					{
						FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double(L"Normal Chest", L" (", int(Distance), L"m)");
						Wrapper::Text(ConvertedText, ScreenLocation, FLinearColor(1.f, 1.f, 0.f, 1.f), FontSize, true);
					}
					else if (StaticClasses::KismetStringLibrary->Contains(ObjectName, FString(L"SunRose_OlympusFactionChest"), true, false))
					{
						FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double(L"Legendary Chest", L" (", int(Distance), L"m)");
						Wrapper::Text(ConvertedText, ScreenLocation, FLinearColor(1.0f, 0.843f, 0.0f, 1.0f), FontSize, true);
					}
					else if (StaticClasses::KismetStringLibrary->Contains(ObjectName, FString(L"Tiered_Safe"), true, false))
					{
						FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double(L"Gold Safe", L" (", int(Distance), L"m)");
						Wrapper::Text(ConvertedText, ScreenLocation, FLinearColor(1.0f, 0.84f, 0.0f, 1.f), FontSize, true);
					}
				}

				if (Settings::Environment::Ammobox)
				{
					if (StaticClasses::KismetStringLibrary->Contains(ObjectName, FString(L"Tiered_Ammo"), true, false))
					{
						FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double(L"Ammo", L" (", int(Distance), L"m)");
						Wrapper::Text(ConvertedText, ScreenLocation, FLinearColor(0.5f, 0.5f, 0.5f, 1.f), FontSize, true);
					}
				}
			}

			TArray<UObject*> SupplyDroneArray = StaticClasses::GameplayStatics->GetAllActorsOfClass( World , Classes::AthenaSuperDingo );
			for ( int i = 0; i < SupplyDroneArray.NumElements; i++ )
			{
				if ( !SupplyDroneArray.Valid( i ) ) continue;

				AAthenaSuperDingo* SupplyDroneActor = reinterpret_cast< AAthenaSuperDingo* >( SupplyDroneArray[ i ] );
				if ( !SupplyDroneActor ) continue;

				bool SupplyDroneIsA = StaticClasses::GameplayStatics->ObjectIsA( SupplyDroneActor , Classes::AthenaSuperDingo );
				if ( !SupplyDroneIsA ) continue;

				FVector2D ScreenLocation = FVector2D( );
				FVector WorldLocation = SupplyDroneActor->K2_GetActorLocation( );
				if ( !Custom::K2_Project( WorldLocation , &ScreenLocation ) ) continue;
				if ( !Custom::InScreen( ScreenLocation ) ) continue;

				double Distance = StaticClasses::KismetMathLibrary->Vector_Distance( LocalHead , WorldLocation ) * 0.01;
				double FontSize = Custom::InterpolatedValue( 150.0 , Distance , 5.0 , Settings::Misc::FontSize );
				if ( Distance > Settings::Environment::MaxDistance ) continue;

				FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double( L"Supply Drone" , L" (" , int( Distance ) , L"m)" );
				Wrapper::Text( ConvertedText , ScreenLocation , FLinearColor( 0.8f , 0.52f , 0.25f , 1.0f ) , FontSize , true );
			}
		}

		if ( Settings::Environment::Traps )
		{
			TArray<UObject*> BuildingTrapArray = StaticClasses::GameplayStatics->GetAllActorsOfClass( World , Classes::BuildingTrap );
			for ( int i = 0; i < BuildingTrapArray.NumElements; i++ )
			{
				if ( !BuildingTrapArray.Valid( i ) ) continue;

				ABuildingTrap* BuildingTrap = reinterpret_cast< ABuildingTrap* >( BuildingTrapArray[ i ] );
				if ( !BuildingTrap ) continue;

				bool BuildingTrapIsA = StaticClasses::GameplayStatics->ObjectIsA( BuildingTrap , Classes::BuildingTrap );
				if ( !BuildingTrapIsA ) continue;

				FVector2D ScreenLocation = FVector2D( );
				FVector WorldLocation = BuildingTrap->K2_GetActorLocation( );
				if ( !Custom::K2_Project( WorldLocation , &ScreenLocation ) ) continue;
				if ( !Custom::InScreen( ScreenLocation ) ) continue;

				double Distance = StaticClasses::KismetMathLibrary->Vector_Distance( LocalHead , WorldLocation ) * 0.01;
				double FontSize = Custom::InterpolatedValue( 150.0 , Distance , 5.0 , Settings::Misc::FontSize );
				if ( Distance > Settings::Environment::MaxDistance ) continue;

				FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double( L"Building Trap" , L" (" , int( Distance ) , L"m)" );
				Wrapper::Text( ConvertedText , ScreenLocation , FLinearColor( 0.0f , 0.5f , 1.0f , 1.0 ) , FontSize , true );

				float ArmTime = BuildingTrap->GetArmTime( );
				FString ArmTimeText = StaticClasses::KismetStringLibrary->BuildString_Double( L"Arm Time " , L"(" , int( ArmTime ) , L")" );
				Wrapper::Text( ArmTimeText , FVector2D( ScreenLocation.X , ScreenLocation.Y + ( FontSize + 2.0 ) ) , FLinearColor( 0.f , 1.f , 0.f , 1.f ) , FontSize , true );
			}
		}

		if ( Settings::Environment::SupplyDrop )
		{
			TArray<UObject*> SupplyDropArray = StaticClasses::GameplayStatics->GetAllActorsOfClass( World , Classes::AthenaSupplyDrop );
			for ( int i = 0; i < SupplyDropArray.NumElements; i++ )
			{
				if ( !SupplyDropArray.Valid( i ) ) continue;

				AFortAthenaSupplyDrop* SupplyDropActor = reinterpret_cast< AFortAthenaSupplyDrop* >( SupplyDropArray[ i ] );
				if ( !SupplyDropActor ) continue;

				bool SupplyDropIsA = StaticClasses::GameplayStatics->ObjectIsA( SupplyDropActor , Classes::AthenaSupplyDrop );
				if ( !SupplyDropIsA ) continue;

				FVector2D ScreenLocation = FVector2D( );
				FVector WorldLocation = SupplyDropActor->K2_GetActorLocation( );
				if ( !Custom::K2_Project( WorldLocation , &ScreenLocation ) ) continue;
				if ( !Custom::InScreen( ScreenLocation ) ) continue;

				double Distance = StaticClasses::KismetMathLibrary->Vector_Distance( LocalHead , WorldLocation ) * 0.01;
				double FontSize = Custom::InterpolatedValue( 150.0 , Distance , 5.0 , Settings::Misc::FontSize );
				if ( Distance > Settings::Environment::MaxDistance ) continue;

				FString ObjectName = StaticClasses::KismetSystemLibrary->GetObjectName( SupplyDropActor );
				if ( StaticClasses::KismetStringLibrary->Contains( ObjectName , FString( L"AthenaSupplyDrop_Llama" ) , true , false ) )
				{
					FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double( L"Llama" , L" (" , int( Distance ) , L"m)" );
					Wrapper::Text( ConvertedText , ScreenLocation , FLinearColor( 0.5f , 0.0f , 1.f , 1.f ) , FontSize , true );
				}
				else if ( StaticClasses::KismetStringLibrary->Contains( ObjectName , FString( L"AthenaSupplyDrop_C" ) , true , false ) )
				{
					FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double( L"Supply Drop" , L" (" , int( Distance ) , L"m)" );
					Wrapper::Text( ConvertedText , ScreenLocation , FLinearColor( 0.1f , 0.5f , 0.9f , 1.0f ) , FontSize , true );
				}
			}
		}

		if ( Settings::Environment::WeakspotAim )
		{
			double ClosestDistanceToCenter = DBL_MAX;
			ABuildingWeakSpot* TargetWeakSpot = nullptr;

			TArray<UObject*> WeakspotArray = StaticClasses::GameplayStatics->GetAllActorsOfClass( World , Classes::BuildingWeakSpot );
			for ( int i = 0; i < WeakspotArray.NumElements; i++ )
			{
				if ( !WeakspotArray.Valid( i ) ) continue;

				ABuildingWeakSpot* WeakspotActor = reinterpret_cast< ABuildingWeakSpot* >( WeakspotArray[ i ] );
				if ( !WeakspotActor ) continue;

				bool WeakspotIsA = StaticClasses::GameplayStatics->ObjectIsA( WeakspotActor , Classes::BuildingWeakSpot );
				if ( !WeakspotIsA ) continue;

				FVector2D ScreenLocation = FVector2D( );
				FVector WorldLocation = WeakspotActor->K2_GetActorLocation( );
				if ( !Custom::K2_Project( WorldLocation , &ScreenLocation ) ) continue;
				if ( !Custom::InScreen( ScreenLocation ) ) continue;

				double Distance = StaticClasses::KismetMathLibrary->Vector_Distance( LocalHead , WorldLocation ) * 0.01;
				double FontSize = Custom::InterpolatedValue( 150.0 , Distance , 5.0 , Settings::Misc::FontSize );
				if ( Distance > 10.0 ) continue;

				if ( WeakspotActor->bHit( ) ) continue;
				if ( !WeakspotActor->bActive( ) ) continue;

				Wrapper::Text( L"WS" , ScreenLocation , FLinearColor( 1.f , 0.f , 0.f , 1.f ) , FontSize , true );

				double DistanceToCenter = StaticClasses::KismetMathLibrary->Vector_Distance2D( Variables::ScreenCenter , ScreenLocation );
				if ( DistanceToCenter < ClosestDistanceToCenter )
				{
					if ( Custom::InCircle( AimbotFOV , ScreenLocation ) )
					{
						ClosestDistanceToCenter = DistanceToCenter;
						TargetWeakSpot = WeakspotActor;
					}
				}
			}

			if ( TargetWeakSpot )
			{
				AFortWeapon* CurrentWeapon = AcknowledgedPawn->CurrentWeapon( );
				bool IsHoldingPickaxe = StaticClasses::GameplayStatics->ObjectIsA( CurrentWeapon , Classes::WeaponPickaxeAthena );
				if ( IsHoldingPickaxe )
				{
					FKey AimbotKey = Custom::GetAimKey(Settings::Aimbot::KeyType);

					FVector WorldLocation = TargetWeakSpot->K2_GetActorLocation( );
					FVector2D ScreenLocation = FVector2D( );
					if ( Custom::K2_Project( WorldLocation , &ScreenLocation ) )
					{
						if ( Settings::Aimbot::TargetLine )
						{
							Wrapper::Line( ScreenLocation , Variables::ScreenCenter , FLinearColor( 1.0f , 0.65f , 0.0f , 1.0f ) , 1.f );
						}

						if ( PlayerController->IsInputKeyDown( AimbotKey ) || PlayerController->IsInputKeyDown( Keys::GamepadLeftTrigger ) )
						{
							FRotator TargetRotation = StaticClasses::KismetMathLibrary->FindLookAtRotation( Variables::CameraLocation , WorldLocation );
							
							if ( Settings::Aimbot::SmoothingType && Settings::Aimbot::Smoothing >= 0.0 )
							{
								if ( Settings::Aimbot::SmoothingType == 1 )
									TargetRotation = StaticClasses::KismetMathLibrary->RInterpTo( Variables::CameraRotation , TargetRotation , WorldDeltaSeconds , (30.0 - Settings::Aimbot::Smoothing));
								else if ( Settings::Aimbot::SmoothingType == 2 )
									TargetRotation = StaticClasses::KismetMathLibrary->RInterpTo_Constant( Variables::CameraRotation , TargetRotation , WorldDeltaSeconds , (30.0 - Settings::Aimbot::Smoothing));

								TargetRotation += ( TargetRotation - Variables::CameraRotation );
							}
							else if (Settings::Environment::SilentWeakspot)
							{
								Update__GetWeaponTargetingTransform__bSilentAimActive = true;
								Update__GetWeaponTargetingTransform__SilentLocationTarget = WorldLocation;
							}

							if (!Settings::Environment::SilentWeakspot)
							{
								Update__GetControlRotation__bAimbotActive = true;
								Update__GetControlRotation__AimbotRotationTarget = TargetRotation;

								PlayerController->SetAim(TargetRotation);
							}
						}
					}
				}
			}
		}

		if ( Settings::Environment::Vehicle )
		{
			TArray<UObject*> VehicleArray = StaticClasses::GameplayStatics->GetAllActorsOfClass( World , Classes::FortAthenaVehicle );
			for ( int i = 0; i < VehicleArray.NumElements; i++ )
			{
				if ( !VehicleArray.Valid( i ) ) continue;

				AFortAthenaVehicle* VehicleActor = reinterpret_cast< AFortAthenaVehicle* >( VehicleArray[ i ] );
				if ( !VehicleActor ) continue;

				bool VehicleIsA = StaticClasses::GameplayStatics->ObjectIsA( VehicleActor , Classes::FortAthenaVehicle );
				if ( !VehicleIsA ) continue;

				AFortAthenaVehicle* CurrentVehicle = AcknowledgedPawn->CurrentVehicle( );
				if ( VehicleActor == CurrentVehicle ) continue;

				bool CanContainPlayers = VehicleActor->CanContainPlayers( );
				if ( !CanContainPlayers ) continue;

				FVector2D ScreenLocation = FVector2D( );
				FVector WorldLocation = VehicleActor->K2_GetActorLocation( );
				if ( !Custom::K2_Project( WorldLocation , &ScreenLocation ) ) continue;
				if ( !Custom::InScreen( ScreenLocation ) ) continue;

				double Distance = StaticClasses::KismetMathLibrary->Vector_Distance( LocalHead , WorldLocation ) * 0.01;
				double FontSize = Custom::InterpolatedValue( 150.0 , Distance , 5.0 , Settings::Misc::FontSize );
				if ( Distance > Settings::Environment::MaxDistance ) continue;

				if ( Settings::Environment::VehicleChams )
				{
					VehicleActor->SetVehicleStencilHighlighted( VehicleActor , true , 11 );
				}

				FString VehicleName = VehicleActor->GetDisplayName();
				if (VehicleName.c_str()) {

					FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double(VehicleName, L" (", int(Distance), L"m)");
					Wrapper::Text(ConvertedText, ScreenLocation, FLinearColor(0.2f, 0.3f, 0.7f, 1.f), FontSize, true);

					FString HealthText = StaticClasses::KismetStringLibrary->BuildString_Double(L"", L"(", int(VehicleActor->GetHealth()), L"/");
					HealthText = StaticClasses::KismetStringLibrary->BuildString_Double(HealthText, L"", int(VehicleActor->GetMaxHealth()), L"HP)");
					Wrapper::Text(HealthText, FVector2D(ScreenLocation.X, ScreenLocation.Y + (FontSize + 2)), FLinearColor(0.f, 1.f, 0.f, 1.f), FontSize, true);
				}
				else
				{
					FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double(FString(L"Vehicle"), L" (", int(Distance), L"m)");
					Wrapper::Text(ConvertedText, ScreenLocation, FLinearColor(0.2f, 0.3f, 0.7f, 1.f), FontSize, true);

					FString HealthText = StaticClasses::KismetStringLibrary->BuildString_Double(L"", L"(", int(VehicleActor->GetHealth()), L"/");
					HealthText = StaticClasses::KismetStringLibrary->BuildString_Double(HealthText, L"", int(VehicleActor->GetMaxHealth()), L"HP)");
					Wrapper::Text(HealthText, FVector2D(ScreenLocation.X, ScreenLocation.Y + (FontSize + 2)), FLinearColor(0.f, 1.f, 0.f, 1.f), FontSize, true);
				}
			}
		}

		if ( Settings::Environment::DebugObjects )
		{
			static int RenderedCount = 0;

			TArray <ULevel*> Levels = World->Levels( );
			for ( int32_t i = 0; i < Levels.NumElements; i++ )
			{
				ULevel* Level = Levels[ i ];
				if ( !Level ) continue;

				TArray <AActor*> Actors = Level->Actors( );
				for ( int32_t xi = 0; xi < Actors.NumElements; xi++ )
				{
					AActor* Actor = Actors[ xi ];
					if ( !Actor ) continue;

					FVector2D ScreenLocation = FVector2D( );
					FVector WorldLocation = Actor->K2_GetActorLocation( );
					if ( !Custom::K2_Project( WorldLocation , &ScreenLocation ) ) continue;
					if ( !Custom::InScreen( ScreenLocation ) ) continue;

					double Distance = StaticClasses::KismetMathLibrary->Vector_Distance( LocalHead , WorldLocation ) * 0.01;
					if ( Distance > Settings::Environment::MaxDistance ) continue;

					FString PathName = StaticClasses::KismetSystemLibrary->GetPathName( Actor );
					Wrapper::Text( PathName , ScreenLocation , FLinearColor( 1.f , 1.f , 1.f , 1.f ) , 10.0 , true );
				}
			}
		}
	}
	else if(!AcknowledgedPawn)
	{
		
	}

	double ClosestDistanceToMyself = DBL_MAX;
	double ClosestDistanceToCenter = DBL_MAX;
	APlayerPawn_Athena_C* TargetPlayerClosestToMyself = nullptr;
	APlayerPawn_Athena_C* TargetPlayerClosestToCenter = nullptr;

	TArray<UObject*> PlayerList = StaticClasses::GameplayStatics->GetAllActorsOfClass(World, Classes::PlayerPawn);
	for ( int32_t i = 0; i < PlayerList.NumElements; i++ )
	{
		if ( !PlayerList.Valid( i ) ) continue;

		APlayerPawn_Athena_C* Player = reinterpret_cast< APlayerPawn_Athena_C* >( PlayerList[ i ] );
		if ( !Player )
			continue;

		bool PlayerCheck = StaticClasses::GameplayStatics->ObjectIsA( Player , Classes::PlayerPawn );
		if ( PlayerCheck && Player != AcknowledgedPawn )
		{
			if ( Player == AcknowledgedPawn && Settings::Player::NoSelfchams) continue;

			bool Wound = Player->IsDBNO( );

			bool Dead = Player->IsDead( );
			if ( Dead ) continue;

			bool Teammate = StaticClasses::FortKismetLibrary->OnSameTeam( Player , AcknowledgedPawn );

			USkeletalMeshComponent* Mesh = Player->Mesh( );
			if ( !Mesh ) continue;

			APlayerState* PlayerState = Player->PlayerState( );
			if ( !PlayerState ) continue;
			bool IsABot = PlayerState->IsABot( );

			if (Settings::Exploits::Wireframe && Player && Player->Mesh())
			{
				int DepthStencil = 11;
				if (USkeletalMeshComponent* Mesh = Player->Mesh())
				{
					Mesh->SetCustomDepthStencilValue(DepthStencil);

					TArray<USkeletalMeshComponent*> SkeletalMeshes = Player->SkeletalMeshes();
					for (int si = 0; si < SkeletalMeshes.NumElements; si++)
					{
						USkeletalMeshComponent* SkeletalMesh = SkeletalMeshes[si];
						if (!SkeletalMesh) continue;

						SkeletalMesh->SetRenderCustomDepth(true);
						SkeletalMesh->SetCustomDepthStencilValue(DepthStencil);
					}

					TArray<UMaterialInstanceDynamic*> MaterialInstances = Player->PawnMaterials_ALL();
					for (int mi = 0; mi < MaterialInstances.NumElements; mi++)
					{
						UMaterialInstanceDynamic* MaterialInstance = MaterialInstances[mi];
						if (!MaterialInstance) continue;

						if (MaterialInstances.NumElements - 2 >= mi)
						{
							MaterialInstance->SetVectorParameterValue(Variables::MaterialParameter4, Settings::Colors::PlayerChams);

							UMaterial* Material = MaterialInstance->GetBaseMaterial(); // this is the same as the dogshit vtable :3
							if (!Material) continue;

							if (Material)
							{
								Material->SetWireframe(true);
								Material->SetDisableDepthTest(true);
							}

							Mesh->SetRenderCustomDepth(true);
						}
					}
				}
			}

			bool Visable = StaticClasses::FortKismetLibrary->CheckLineOfSightToActorWithChannel( Variables::CameraLocation , Player , ECollisionChannel::ECC_Visibility , AcknowledgedPawn );
			
			if (Settings::Misc::BulletTraces && AcknowledgedPawn && PlayerController)
			{
				for (int ri = 0; ri < 5; ri++)
				{
					FVector BulletTraceWorld3D = Variables::BulletTraces::WorldTraceLocations3D[ri];
					FVector MuzzleTraceWorld3D = Variables::BulletTraces::MuzzleWorldTraceLocations3D[ri];

					if (BulletTraceWorld3D && MuzzleTraceWorld3D)
					{
						FVector2D BulletTraceWorld2D;
						FVector2D MuzzleTraceWorld2D;

						if (PlayerController->w2s(BulletTraceWorld3D, &BulletTraceWorld2D))
						{
							if (PlayerController->w2s(MuzzleTraceWorld3D, &MuzzleTraceWorld2D))
							{
								Wrapper::Line(BulletTraceWorld2D, MuzzleTraceWorld2D, Variables::BulletTraces::BulletTraceColor, 4.f);
							}
						}
					}
				}
			}

			if (Settings::Exploits::RainbowChams && Player && AcknowledgedPawn)
			{
				static double lastChamsChangeColor = 0.00;
				
				if ((Player != AcknowledgedPawn || Settings::Player::NoSelfchams))
				{
					//int DepthStencil = 11;

					//Player->Mesh()->SetCustomDepthStencilValue(DepthStencil);
				/*	if (Settings::Exploits::ChamsThroughWalls)
					{
						Material->SetbDisableDepthTest(true);
					}
					else
					{
						Material->SetbDisableDepthTest(false);
					}*/

					if (Settings::Developer::AllowChamsThruWall)
					{
						auto PawnMaterials = Player->Mesh()->GetMaterials();

						for (int i = 0; i < PawnMaterials.NumElements; i++)
						{
							if (PawnMaterials.Valid(i))
							{
								if (Settings::Exploits::ChamsThroughWalls)
								{
									if (UMaterial* LocalMaterial = (UMaterial*)PawnMaterials[i]->GetMaterial())
									{
										LocalMaterial->SetDisableDepthTest(true);
									}
								}
								else
								{
									if (UMaterial* LocalMaterial = (UMaterial*)PawnMaterials[i]->GetMaterial())
									{
										LocalMaterial->SetDisableDepthTest(false); 
									}
								}
							}
						}
					}

					if (StaticClasses::KismetSystemLibrary->GetGameTimeInSeconds(World) - lastChamsChangeColor >= 1.00)
					{
						int32_t RandomIndexInt1 = StaticClasses::KismetMathLibrary->RandomIntegerInRange(0, 255 - 1);
						int32_t RandomIndexInt2 = StaticClasses::KismetMathLibrary->RandomIntegerInRange(0, 255 - 1);
						int32_t RandomIndexInt3 = StaticClasses::KismetMathLibrary->RandomIntegerInRange(0, 255 - 1);

						float RandomIndex1 = RandomIndexInt1 / 255.f;
						float RandomIndex2 = RandomIndexInt2 / 255.f;
						float RandomIndex3 = RandomIndexInt3 / 255.f;

						//TargetBoneIndex = BoneArray[RandomIndex];

						lastChamsChangeColor = StaticClasses::KismetSystemLibrary->GetGameTimeInSeconds(World);

						FLinearColor ChamsColor = FLinearColor(RandomIndex1, RandomIndex2, RandomIndex3, 1.f);

						Settings::Cache::bChamsWasActive = true;

						Settings::Colors::RainbowChamsColorUWU = ChamsColor;
					}

					if (Settings::Exploits::ChamsThroughWalls)
					{
						Material->SetDisableDepthTest(true);
					}
					else
					{
						Material->SetDisableDepthTest(false);
					}

					Player->ApplyPawnHighlight(Settings::Colors::RainbowChamsColorUWU, Settings::Colors::RainbowChamsColorUWU);
				}
				else if ((Player != AcknowledgedPawn || Settings::Player::NoSelfchams))
				{

					int DepthStencil = 11;

					Player->Mesh()->SetCustomDepthStencilValue(DepthStencil);
					if (Settings::Exploits::RainbowChams)
					{

						if (StaticClasses::KismetSystemLibrary->GetGameTimeInSeconds(World) - lastChamsChangeColor >= 1.00)
						{
							int32_t RandomIndexInt1 = StaticClasses::KismetMathLibrary->RandomIntegerInRange(0, 255 - 1);
							int32_t RandomIndexInt2 = StaticClasses::KismetMathLibrary->RandomIntegerInRange(0, 255 - 1);
							int32_t RandomIndexInt3 = StaticClasses::KismetMathLibrary->RandomIntegerInRange(0, 255 - 1);

							float RandomIndex1 = RandomIndexInt1 / 255.f;
							float RandomIndex2 = RandomIndexInt2 / 255.f;
							float RandomIndex3 = RandomIndexInt3 / 255.f;

							//TargetBoneIndex = BoneArray[RandomIndex];

							lastChamsChangeColor = StaticClasses::KismetSystemLibrary->GetGameTimeInSeconds(World);

							FLinearColor ChamsColor = FLinearColor(RandomIndex1, RandomIndex2, RandomIndex3, 1.f);

							Settings::Colors::RainbowChamsColorUWU = ChamsColor;
						}

						Settings::Cache::bChamsWasActive = true;

						Player->ApplyPawnHighlight(Settings::Colors::RainbowChamsColorUWU, Settings::Colors::RainbowChamsColorUWU);
					}

					Mesh->SetRenderCustomDepth(true);
				}
			}
			else if (Settings::Cache::bChamsWasActive) // reset on turning it off
			{
				Settings::Cache::bChamsWasActive = false;

				Player->ApplyPawnHighlight(FLinearColor(0.f, 0.f, 0.f, 0.f), FLinearColor(0.f, 0.f, 0.f, 0.f));
			}

			if (Settings::Exploits::RainbowChams && AcknowledgedPawn)
			{
				static double lastChamsChangeColor = 0.00;
				if (StaticClasses::KismetSystemLibrary->GetGameTimeInSeconds(World) - lastChamsChangeColor >= 1.00)
				{
					int32_t RandomIndexInt1 = StaticClasses::KismetMathLibrary->RandomIntegerInRange(0, 255 - 1);
					int32_t RandomIndexInt2 = StaticClasses::KismetMathLibrary->RandomIntegerInRange(0, 255 - 1);
					int32_t RandomIndexInt3 = StaticClasses::KismetMathLibrary->RandomIntegerInRange(0, 255 - 1);

					float RandomIndex1 = RandomIndexInt1 / 255.f;
					float RandomIndex2 = RandomIndexInt2 / 255.f;
					float RandomIndex3 = RandomIndexInt3 / 255.f;

					//TargetBoneIndex = BoneArray[RandomIndex];

					lastChamsChangeColor = StaticClasses::KismetSystemLibrary->GetGameTimeInSeconds(World);

					FLinearColor ChamsColor = FLinearColor(RandomIndex1, RandomIndex2, RandomIndex3, 1.f);

					Settings::Cache::bChamsWasActive = true;

					Settings::Colors::RainbowChamsColorUWU = ChamsColor;
				}

				AcknowledgedPawn->ApplyPawnHighlight(Settings::Colors::RainbowChamsColorUWU, Settings::Colors::RainbowChamsColorUWU);
			}
			else if (Settings::Cache::bChamsWasActive && AcknowledgedPawn) // reset on turning it off
			{
				Settings::Cache::bChamsWasActive = false;

				AcknowledgedPawn->ApplyPawnHighlight(FLinearColor(0.f, 0.f, 0.f, 0.f), FLinearColor(0.f, 0.f, 0.f, 0.f));
			}

			if (Settings::Exploits::Wireframe)
			{
				if (AcknowledgedPawn)
				{
					int DepthStencil = 11;
					if (AcknowledgedPawn->Mesh())
					{
						AcknowledgedPawn->Mesh()->SetCustomDepthStencilValue(DepthStencil);
					}

					TArray<USkeletalMeshComponent*> SkeletalMeshes = AcknowledgedPawn->SkeletalMeshes();
					for (int si = 0; si < SkeletalMeshes.Size(); si++)
					{
						USkeletalMeshComponent* SkeletalMesh = SkeletalMeshes.Data[si];
						if (!SkeletalMesh) continue;

						SkeletalMesh->SetRenderCustomDepth(true);
						SkeletalMesh->SetCustomDepthStencilValue(DepthStencil);
					}
				}

				//UMaterial* MaterialObj = (UMaterial*)UObject::FindObject(L"Material M_Elimination_DigitizeCubes.M_Elimination_DigitizeCubes");
				//*(byte*)(MaterialObj + 0x1a8) |= 1 << 0; // bDisableDepthTest
				//printf("bDisableDepthTest: %i\n", MaterialObj + 0x1a8);

				//TArray<UMaterialInterface*> Materials = Mesh->GetMaterials();
				//for (int j = 0; j < Materials.Count; j++) {
				//	Mesh->SetMaterial(j, (UMaterialInterface*)MaterialObj);
				//}
				if (AcknowledgedPawn)
				{
					TArray<UMaterialInstanceDynamic*> MaterialInstances = AcknowledgedPawn->PawnMaterials_ALL();
					if (MaterialInstances.Valid(0))
					{
						for (int mi = 0; mi < MaterialInstances.Size(); mi++)
						{
							UMaterialInstanceDynamic* MaterialInstance = MaterialInstances.Data[mi];
							if (!MaterialInstance) continue;

							if (MaterialInstances.NumElements - 2 >= mi)
							{
								UMaterial* LMaterial = (UMaterial*)MaterialInstances.Data[mi];
								if (!LMaterial) continue;

								*(BYTE*)(LMaterial + Offsets::ExploitOffsets::Wireframe) |= 1 << 6; // Wireframe
								*(BYTE*)(LMaterial + Offsets::ExploitOffsets::bDisableDepthTest) |= 1 << 0; // bDisableDepthTest
							}

							if (AcknowledgedPawn->Mesh())
							{
								AcknowledgedPawn->Mesh()->SetRenderCustomDepth(true);
							}
						}
					}
				}
			}


			if (Settings::Player::Enable)
			{

				if (Settings::Exploits::VehicleTPAura && Player && AcknowledgedPawn && PlayerController && !Teammate)
				{
					if (!Player->IsInVehicle() && !Player->IsDrivingVehicle())
					{
						AFortAthenaVehicle* CurrentVehicle = AcknowledgedPawn->CurrentVehicle();
						if (CurrentVehicle)
						{
							FVector CurrentLocation = CurrentVehicle->K2_GetActorLocation();
							FVector TargetLocationHelper = CurrentLocation;

							TargetLocationHelper.Z += 45;

							FVector TargetLocation = Player->K2_GetActorLocation();
							FRotator LocalRotation = AcknowledgedPawn->K2_GetActorRotation();
							FRotator TargetLookAtRotation = StaticClasses::KismetMathLibrary->FindLookAtRotation(CurrentLocation, TargetLocation);

							FVector2D TargetScreenLocation = FVector2D();
							FVector2D LocalCarScreenLocation = FVector2D();

							TargetLocation.Z += 45;

							if (PlayerController->w2s(TargetLocation, &TargetScreenLocation) && TargetLocation)
							{
								if (PlayerController->w2s(CurrentLocation, &LocalCarScreenLocation) && TargetLocation)
								{
									Wrapper::Line(LocalCarScreenLocation, TargetScreenLocation, FromRGB(0, 255, 255), 3.f);
									Wrapper::Line(LocalCarScreenLocation, TargetScreenLocation, FromRGB(0, 255, 255), 3.f);
								}
							}

							CurrentVehicle->K2_TeleportTo(TargetLocation, LocalRotation);
						}
					}
				}

				FLinearColor BoxColor = Settings::Colors::BoxInVisible;
				FLinearColor SkeletonColor = Settings::Colors::SkeletonInVisible;
				FLinearColor SnaplineColor = Settings::Colors::SkeletonInVisible;

				if ( Visable || !AcknowledgedPawn )
				{
					BoxColor = Settings::Colors::BoxVisible;
					SkeletonColor = Settings::Colors::SkeletonVisible;
					SnaplineColor = Settings::Colors::SnaplineVisible;
				}

				if ( Teammate )
				{
					BoxColor = Settings::Colors::TeammateColor;
					SkeletonColor = Settings::Colors::TeammateColor;
					SnaplineColor = Settings::Colors::TeammateColor;
				}

				FVector Root = Custom::GetSocketLocation( Mesh , FString( L"root" ) );
				double Distance = StaticClasses::KismetMathLibrary->Vector_Distance( Variables::CameraLocation , Root ) * 0.01;
				double BoxSize = Custom::InterpolatedValue( Distance , 100.0 , 1.0 , 3.0 );
				double FontSize = Custom::InterpolatedValue( 150.0 , Distance , 8.0 , Settings::Misc::FontSize );

			

				FVector Head = Custom::GetSocketLocation( Mesh , FString( L"head" ) );
				FVector2D HeadScreen = FVector2D( );
				if ( !Custom::K2_Project( Head , &HeadScreen ) ) continue;

				if ( Settings::Player::FOVArrows )
				{
					if ( !Custom::InScreen( HeadScreen ) && Player != AcknowledgedPawn)
					{
						double Angle = StaticClasses::KismetMathLibrary->Atan2( HeadScreen.Y - Variables::ScreenCenter.Y , HeadScreen.X - Variables::ScreenCenter.X );

						FVector2D ArrowPosition = { Variables::ScreenCenter.X + ( AimbotFOV + 10.0 ) * StaticClasses::KismetMathLibrary->cos( Angle ),
						 Variables::ScreenCenter.Y + ( AimbotFOV + 10.0 ) * StaticClasses::KismetMathLibrary->sin( Angle ) };

						double RotationAngle = Angle - WorldDeltaSeconds;

						FVector2D Vertex[ 3 ] =
						{
							FVector2D( ArrowPosition.X + StaticClasses::KismetMathLibrary->cos( RotationAngle ) * 10.0, ArrowPosition.Y + StaticClasses::KismetMathLibrary->sin( RotationAngle ) * 10.0 ),
							FVector2D( ArrowPosition.X + StaticClasses::KismetMathLibrary->cos( RotationAngle - 1.5 ) * 8.0, ArrowPosition.Y + StaticClasses::KismetMathLibrary->sin( RotationAngle - 1.5 ) * 8.0 ),
							FVector2D( ArrowPosition.X + StaticClasses::KismetMathLibrary->cos( RotationAngle + 1.5 ) * 8.0, ArrowPosition.Y + StaticClasses::KismetMathLibrary->sin( RotationAngle + 1.5 ) * 8.0 )
						};

						Wrapper::Triangle( Vertex[ 0 ] , Vertex[ 1 ] , Vertex[ 2 ] , 1.f , BoxColor );
					}
				}

				bool AimbotKnocked = ( Settings::Aimbot::SkipKnocked && !Wound ) || !Settings::Aimbot::SkipKnocked;
				bool AimbotBot = ( Settings::Aimbot::IgnoreBots && !IsABot ) || !Settings::Aimbot::IgnoreBots;
				bool AimbotValid = ( AimbotKnocked && AimbotBot && !Teammate ) && Distance < Settings::Aimbot::MaxDistance;
				if ( AimbotValid )
				{
					if ( Custom::InCircle( AimbotFOV , HeadScreen ) )
					{
						if ( Distance < ClosestDistanceToMyself )
						{
							ClosestDistanceToMyself = Distance;
							TargetPlayerClosestToMyself = Player;
						}

						double DistanceToCenter = StaticClasses::KismetMathLibrary->Vector_Distance2D( Variables::ScreenCenter , HeadScreen );
						if ( DistanceToCenter < ClosestDistanceToCenter )
						{
							ClosestDistanceToCenter = DistanceToCenter;
							TargetPlayerClosestToCenter = Player;
						}
					}
				}

				if ( !Custom::InScreen( HeadScreen ) ) continue;

				if ( Settings::Player::Enable )
				{
					FVector Origin = FVector( );
					FVector Extent = FVector( );
					float SphereRadius = 0.f;

					StaticClasses::KismetSystemLibrary->GetComponentBounds( Mesh , &Origin , &Extent , &SphereRadius );

					double MostLeft = DBL_MAX;
					double MostRight = DBL_MIN;
					double MostTop = DBL_MAX;
					double MostBottom = DBL_MIN;

					for ( int i = 0; i < 8; i++ )
					{
						FVector Corner;

						if ( i & 1 )
						{
							Corner.X = 1.0 * Extent.X;
						}
						else
						{
							Corner.X = -1.0 * Extent.X;
						}

						if ( i & 2 )
						{
							Corner.Y = 1.0 * Extent.Y;
						}
						else
						{
							Corner.Y = -1.0 * Extent.Y;
						}

						if ( i & 2 )
						{
							Corner.Z = 1.0 * Extent.Z;
						}
						else
						{
							Corner.Z = -1.0 * Extent.Z;
						}

						FVector2D Origin2D = FVector2D( );
						if ( !Custom::K2_Project( Origin + Corner , &Origin2D ) ) continue;

						if ( Origin2D.X < MostLeft )
						{
							MostLeft = Origin2D.X;
						}

						if ( Origin2D.X > MostRight )
						{
							MostRight = Origin2D.X;
						}

						if ( Origin2D.Y < MostTop )
						{
							MostTop = Origin2D.Y;
						}

						if ( Origin2D.Y > MostBottom )
						{
							MostBottom = Origin2D.Y;
						}
					}

					double ActorHeight = MostBottom - MostTop;
					double ActorWidth = MostRight - MostLeft;

					double CornerWidth = ActorWidth / 4.0;
					double CornerHeight = ActorHeight / 7.0;

					double TopTextOffset = 17.0;
					double BottomTextOffset = FontSize;

					FVector2D BottomMiddle = FVector2D( MostLeft + ( ActorWidth / 2.0 ) , MostBottom );

					float BoxThickness = ( Distance < 30.0 ) ? static_cast<float>(Settings::Visuals::BoxThiccnessClose) : static_cast<float>(Settings::Visuals::BoxThiccness);
					float CornerThickness = ( Distance < 30.0 ) ? static_cast<float>(Settings::Visuals::CornerThiccness) : static_cast<float>(Settings::Visuals::CornerThiccnessClose);


					if (Settings::Player::Dishwasher && Player && PlayerState)
					{

						if (Player->IsCharacterFemale())
						{
							Wrapper::Text((FString)L"Dishwasher: Yes", FVector2D(BottomMiddle.X, MostBottom + BottomTextOffset), Settings::Colors::TextColor, FontSize, true);
						}
						else if(Player->GetCharacterGender() == EFortCustomGender::Both)
						{
							Wrapper::Text((FString)L"Gender: cat", FVector2D(BottomMiddle.X, MostBottom + BottomTextOffset), Settings::Colors::TextColor, FontSize, true);
						}
						else
						{
							Wrapper::Text((FString)L"Dishwasher: No", FVector2D(BottomMiddle.X, MostBottom + BottomTextOffset), Settings::Colors::TextColor, FontSize, true);
						}

						BottomTextOffset += FontSize + 2;
					}

					if ( Settings::Player::Skeleton )
					{
						if (!Settings::Visuals::bOutlineESP)
						{
							for (const auto& Pair : SkeletonBonePairs)
							{
								FVector WorldLocationFirst = Custom::GetSocketLocation(Mesh, Pair.first);
								FVector WorldLocationSecond = Custom::GetSocketLocation(Mesh, Pair.second);

								FVector2D ScreenPositionFirst = FVector2D();
								FVector2D ScreenPositionSecond = FVector2D();

								if (!Custom::K2_Project(WorldLocationFirst, &ScreenPositionFirst)) continue;
								if (!Custom::K2_Project(WorldLocationSecond, &ScreenPositionSecond)) continue;

								Wrapper::Line(ScreenPositionFirst, ScreenPositionSecond, SkeletonColor, static_cast<float>(Settings::Visuals::SkeletonThiccness));
							}
						}
						else if (Settings::Visuals::bOutlineESP)
						{
							for (const auto& Pair : SkeletonBonePairs)
							{
								FVector WorldLocationFirst = Custom::GetSocketLocation(Mesh, Pair.first);
								FVector WorldLocationSecond = Custom::GetSocketLocation(Mesh, Pair.second);

								FVector2D ScreenPositionFirst = FVector2D();
								FVector2D ScreenPositionSecond = FVector2D();

								if (!Custom::K2_Project(WorldLocationFirst, &ScreenPositionFirst)) continue;
								if (!Custom::K2_Project(WorldLocationSecond, &ScreenPositionSecond)) continue;

								FLinearColor OutlineColor = FromRGB(0, 0, 0); 
								float OutlineThickness = static_cast<float>(Settings::Visuals::OutlineThiccness);
								Wrapper::Line(ScreenPositionFirst, ScreenPositionSecond, OutlineColor, Settings::Visuals::SkeletonThiccness + OutlineThickness);
								Wrapper::Line(ScreenPositionFirst, ScreenPositionSecond, SkeletonColor, static_cast<float>(Settings::Visuals::SkeletonThiccness));
							}
						}
					}

					if (Settings::Developer::Debugging::HealthTestOthers && Player && Player->Mesh() && Player->PlayerState() && PlayerController && AcknowledgedPawn)
					{
						//Wrapper::Line(FVector2D(MostLeft - BoxSize, MostTop - BoxSize), FVector2D(MostRight + BoxSize, MostTop - BoxSize), BoxColor, BoxThickness);
						//Wrapper::Line(FVector2D(MostLeft - BoxSize, MostBottom + BoxSize), FVector2D(MostRight + BoxSize, MostBottom + BoxSize), BoxColor, BoxThickness);
						//Wrapper::Line(FVector2D(MostLeft - BoxSize, MostBottom + BoxSize), FVector2D(MostLeft - BoxSize, MostTop - BoxSize), BoxColor, BoxThickness);
						//Wrapper::Line(FVector2D(MostRight + BoxSize, MostTop - BoxSize), FVector2D(MostRight + BoxSize, MostBottom + BoxSize), BoxColor, BoxThickness);

						//Wrapper::Line(BottomMiddle, Variables::ScreenCenter, OutlineColor, 3.f);

						FVector Head = Custom::GetSocketLocation(Player->Mesh(), FString(L"head"));
						FVector RightFoot = Custom::GetSocketLocation(Player->Mesh(), FString(L"ik_foot_r"));

						FVector2D Head2D = FVector2D();
						FVector2D RightFoot2D = FVector2D();

						if (PlayerController->w2s(Head, &Head2D) && PlayerController->w2s(RightFoot, &RightFoot2D))
						{
							float Health = Player->GetHealth();
							float Shield = Player->GetShield();

							Settings::Developer::Debugging::MostRecentHealth = Health;
							Settings::Developer::Debugging::MostRecentShield = Shield;

							// health bar vars
							double CurrentPercentage = double((double)(Health + Shield) / (double)200);
							double BarWidth = 40;
							double BarHeight = 120;

							FVector2D TargetRenderPosBegin = FVector2D();
							FVector2D TargetRenderPosEnd = FVector2D();
							TargetRenderPosBegin = FVector2D(BottomMiddle.X, MostBottom + BottomTextOffset);
							TargetRenderPosBegin.X = RightFoot2D; // real method
							TargetRenderPosBegin.X += Settings::Developer::Debugging::HealthOffset;
							TargetRenderPosBegin.X -= Settings::Developer::Debugging::HealthOffsetNegative;
							TargetRenderPosBegin.Y = RightFoot2D.Y - 5;

							TargetRenderPosEnd = TargetRenderPosBegin;
							TargetRenderPosEnd.Y = Head2D.Y + 40;

							// FVector2D(BottomMiddle.X, MostBottom + BottomTextOffset), FLinearColor(0.8f, 0.8f, 0.f, 1.f));
							// bottom thingy^ -> move to right based on right foot + offset (ik_foot_r) then y via head.Y + 40 (offset) as top and then foot.Y - 5 (offset)

							// health bar main
							Wrapper::RectFilled(TargetRenderPosBegin, FVector2D(BarWidth, BarHeight), FromRGB(255, 0, 0));

							// health bar slider
							Wrapper::RectFilled(TargetRenderPosEnd, FVector2D(BarWidth, BarHeight * CurrentPercentage), FromRGB(0, 255, 0));
						}
					}

					if ( Settings::Player::FilledBox )
					{
						UTexture2D* DefaultTexture = Canvas->DefaultTexture( );
						FVector2D TopLeft = FVector2D( MostLeft - BoxSize , MostTop - BoxSize );
						for ( float Y = TopLeft.Y; Y < TopLeft.Y + ( ActorHeight + 2 * BoxSize ); Y++ )
						{
							Canvas->K2_DrawTexture( DefaultTexture , FVector2D( TopLeft.X , Y ) , FVector2D( ActorWidth + 2 * BoxSize , 1.0 ) , FVector2D( ) , FVector2D( 1.0 , 1.0 ) , FLinearColor( 0.0f , 0.0f , 0.0f , 0.20f ) , EBlendMode::BLEND_Translucent , 0.f , FVector2D( ) );
						}
					}

					if ( Settings::Player::BoxType == 1 )
					{
						if (!Settings::Visuals::bOutlineESP)
						{
							Wrapper::Line(FVector2D(MostLeft - BoxSize, MostTop - BoxSize), FVector2D(MostRight + BoxSize, MostTop - BoxSize), BoxColor, BoxThickness);
							Wrapper::Line(FVector2D(MostLeft - BoxSize, MostBottom + BoxSize), FVector2D(MostRight + BoxSize, MostBottom + BoxSize), BoxColor, BoxThickness);
							Wrapper::Line(FVector2D(MostLeft - BoxSize, MostBottom + BoxSize), FVector2D(MostLeft - BoxSize, MostTop - BoxSize), BoxColor, BoxThickness);
							Wrapper::Line(FVector2D(MostRight + BoxSize, MostTop - BoxSize), FVector2D(MostRight + BoxSize, MostBottom + BoxSize), BoxColor, BoxThickness);
						}
						else if (Settings::Visuals::bOutlineESP)
						{
							FLinearColor OutlineColor = FromRGB(0, 0, 0);

							// Outline Box
							Wrapper::Line(FVector2D(MostLeft - BoxSize - Settings::Visuals::OutlineThiccness, MostTop - BoxSize - Settings::Visuals::OutlineThiccness),
								FVector2D(MostRight + BoxSize + Settings::Visuals::OutlineThiccness, MostTop - BoxSize - Settings::Visuals::OutlineThiccness),
								OutlineColor, BoxThickness + Settings::Visuals::OutlineThiccness);

							Wrapper::Line(FVector2D(MostLeft - BoxSize - Settings::Visuals::OutlineThiccness, MostBottom + BoxSize + Settings::Visuals::OutlineThiccness),
								FVector2D(MostRight + BoxSize + Settings::Visuals::OutlineThiccness, MostBottom + BoxSize + Settings::Visuals::OutlineThiccness),
								OutlineColor, BoxThickness + Settings::Visuals::OutlineThiccness);

							Wrapper::Line(FVector2D(MostLeft - BoxSize - Settings::Visuals::OutlineThiccness, MostBottom + BoxSize + Settings::Visuals::OutlineThiccness),
								FVector2D(MostLeft - BoxSize - Settings::Visuals::OutlineThiccness, MostTop - BoxSize - Settings::Visuals::OutlineThiccness),
								OutlineColor, BoxThickness + Settings::Visuals::OutlineThiccness);

							Wrapper::Line(FVector2D(MostRight + BoxSize + Settings::Visuals::OutlineThiccness, MostTop - BoxSize - Settings::Visuals::OutlineThiccness),
								FVector2D(MostRight + BoxSize + Settings::Visuals::OutlineThiccness, MostBottom + BoxSize + Settings::Visuals::OutlineThiccness),
								OutlineColor, BoxThickness + Settings::Visuals::OutlineThiccness);

							// Inner Box
							Wrapper::Line(FVector2D(MostLeft - BoxSize, MostTop - BoxSize), FVector2D(MostRight + BoxSize, MostTop - BoxSize), BoxColor, BoxThickness);
							Wrapper::Line(FVector2D(MostLeft - BoxSize, MostBottom + BoxSize), FVector2D(MostRight + BoxSize, MostBottom + BoxSize), BoxColor, BoxThickness);
							Wrapper::Line(FVector2D(MostLeft - BoxSize, MostBottom + BoxSize), FVector2D(MostLeft - BoxSize, MostTop - BoxSize), BoxColor, BoxThickness);
							Wrapper::Line(FVector2D(MostRight + BoxSize, MostTop - BoxSize), FVector2D(MostRight + BoxSize, MostBottom + BoxSize), BoxColor, BoxThickness);
						}
					}
					else if (Settings::Player::BoxType == 2)
					{
						if (!Settings::Visuals::bOutlineESP)
						{
							Wrapper::Line(FVector2D(MostLeft, MostTop), FVector2D(MostLeft, MostTop + (CornerHeight * BoxSize)), BoxColor, CornerThickness);
							Wrapper::Line(FVector2D(MostLeft, MostTop), FVector2D(MostLeft + (CornerWidth + BoxSize), MostTop), BoxColor, CornerThickness);

							Wrapper::Line(FVector2D(MostLeft + ActorWidth - (CornerWidth + BoxSize), MostTop), FVector2D(MostLeft + ActorWidth, MostTop), BoxColor, CornerThickness);
							Wrapper::Line(FVector2D(MostLeft + ActorWidth, MostTop), FVector2D(MostLeft + ActorWidth, MostTop + (CornerHeight * BoxSize)), BoxColor, CornerThickness);

							Wrapper::Line(FVector2D(MostLeft, MostTop + ActorHeight - (CornerHeight * BoxSize)), FVector2D(MostLeft, MostTop + ActorHeight), BoxColor, CornerThickness);
							Wrapper::Line(FVector2D(MostLeft, MostTop + ActorHeight), FVector2D(MostLeft + (CornerWidth + BoxSize), MostTop + ActorHeight), BoxColor, CornerThickness);

							Wrapper::Line(FVector2D(MostLeft + ActorWidth - (CornerWidth + BoxSize), MostTop + ActorHeight), FVector2D(MostLeft + ActorWidth, MostTop + ActorHeight), BoxColor, CornerThickness);
							Wrapper::Line(FVector2D(MostLeft + ActorWidth, MostTop + ActorHeight - (CornerHeight * BoxSize)), FVector2D(MostLeft + ActorWidth, MostTop + ActorHeight), BoxColor, CornerThickness);
						}
						else if (Settings::Visuals::bOutlineESP)
						{
							if (Settings::Player::BoxType == 2)
							{
								// Define outline thickness
								float OutlineThickness = Settings::Visuals::OutlineThiccness;
								
								FLinearColor OutlineColor = FromRGB(0, 0, 0);

								// Draw outline first (slightly larger than the original box)
								Wrapper::Line(FVector2D(MostLeft - OutlineThickness, MostTop - OutlineThickness),
									FVector2D(MostLeft - OutlineThickness, MostTop + (CornerHeight * BoxSize) + OutlineThickness),
									OutlineColor, CornerThickness + OutlineThickness);

								Wrapper::Line(FVector2D(MostLeft - OutlineThickness, MostTop - OutlineThickness),
									FVector2D(MostLeft + (CornerWidth + BoxSize) + OutlineThickness, MostTop - OutlineThickness),
									OutlineColor, CornerThickness + OutlineThickness);

								Wrapper::Line(FVector2D(MostLeft + ActorWidth - (CornerWidth + BoxSize) - OutlineThickness, MostTop - OutlineThickness),
									FVector2D(MostLeft + ActorWidth + OutlineThickness, MostTop - OutlineThickness),
									OutlineColor, CornerThickness + OutlineThickness);

								Wrapper::Line(FVector2D(MostLeft + ActorWidth + OutlineThickness, MostTop - OutlineThickness),
									FVector2D(MostLeft + ActorWidth + OutlineThickness, MostTop + (CornerHeight * BoxSize) + OutlineThickness),
									OutlineColor, CornerThickness + OutlineThickness);

								Wrapper::Line(FVector2D(MostLeft - OutlineThickness, MostTop + ActorHeight - (CornerHeight * BoxSize) - OutlineThickness),
									FVector2D(MostLeft - OutlineThickness, MostTop + ActorHeight + OutlineThickness),
									OutlineColor, CornerThickness + OutlineThickness);

								Wrapper::Line(FVector2D(MostLeft - OutlineThickness, MostTop + ActorHeight + OutlineThickness),
									FVector2D(MostLeft + (CornerWidth + BoxSize) + OutlineThickness, MostTop + ActorHeight + OutlineThickness),
									OutlineColor, CornerThickness + OutlineThickness);

								Wrapper::Line(FVector2D(MostLeft + ActorWidth - (CornerWidth + BoxSize) - OutlineThickness, MostTop + ActorHeight + OutlineThickness),
									FVector2D(MostLeft + ActorWidth + OutlineThickness, MostTop + ActorHeight + OutlineThickness),
									OutlineColor, CornerThickness + OutlineThickness);

								Wrapper::Line(FVector2D(MostLeft + ActorWidth + OutlineThickness, MostTop + ActorHeight - (CornerHeight * BoxSize) - OutlineThickness),
									FVector2D(MostLeft + ActorWidth + OutlineThickness, MostTop + ActorHeight + OutlineThickness),
									OutlineColor, CornerThickness + OutlineThickness);

								// Now draw the original box over the outline
								Wrapper::Line(FVector2D(MostLeft, MostTop),
									FVector2D(MostLeft, MostTop + (CornerHeight * BoxSize)),
									BoxColor, CornerThickness);

								Wrapper::Line(FVector2D(MostLeft, MostTop),
									FVector2D(MostLeft + (CornerWidth + BoxSize), MostTop),
									BoxColor, CornerThickness);

								Wrapper::Line(FVector2D(MostLeft + ActorWidth - (CornerWidth + BoxSize), MostTop),
									FVector2D(MostLeft + ActorWidth, MostTop),
									BoxColor, CornerThickness);

								Wrapper::Line(FVector2D(MostLeft + ActorWidth, MostTop),
									FVector2D(MostLeft + ActorWidth, MostTop + (CornerHeight * BoxSize)),
									BoxColor, CornerThickness);

								Wrapper::Line(FVector2D(MostLeft, MostTop + ActorHeight - (CornerHeight * BoxSize)),
									FVector2D(MostLeft, MostTop + ActorHeight),
									BoxColor, CornerThickness);

								Wrapper::Line(FVector2D(MostLeft, MostTop + ActorHeight),
									FVector2D(MostLeft + (CornerWidth + BoxSize), MostTop + ActorHeight),
									BoxColor, CornerThickness);

								Wrapper::Line(FVector2D(MostLeft + ActorWidth - (CornerWidth + BoxSize), MostTop + ActorHeight),
									FVector2D(MostLeft + ActorWidth, MostTop + ActorHeight),
									BoxColor, CornerThickness);

								Wrapper::Line(FVector2D(MostLeft + ActorWidth, MostTop + ActorHeight - (CornerHeight * BoxSize)),
									FVector2D(MostLeft + ActorWidth, MostTop + ActorHeight),
									BoxColor, CornerThickness);
							}
						}
					}

					if (Settings::Player::LineType == 1)
					{
						FLinearColor OutlineColor = FromRGB(0, 0, 0);

						Wrapper::Line(BottomMiddle, FVector2D(Variables::ScreenCenter.X, Variables::ScreenCenter.Y * 2), OutlineColor, 3.f);
						Wrapper::Line(BottomMiddle, FVector2D(Variables::ScreenCenter.X, Variables::ScreenCenter.Y * 2), BoxColor, 2.f);
					}
					else if (Settings::Player::LineType == 2)
					{
						FLinearColor OutlineColor = FromRGB(0, 0, 0);

						Wrapper::Line(BottomMiddle, Variables::ScreenCenter, OutlineColor, 3.f);
						Wrapper::Line(BottomMiddle, Variables::ScreenCenter, BoxColor, 2.f);
					}
					else if (Settings::Player::LineType == 3)
					{
						FLinearColor OutlineColor = FromRGB(0, 0, 0);

						Wrapper::Line(HeadScreen, FVector2D(Variables::ScreenCenter.X, 0.0), OutlineColor, 3.f);

						Wrapper::Line(HeadScreen, FVector2D(Variables::ScreenCenter.X, 0.0), BoxColor, 2.f);
					}

					if ( Settings::Player::Rank && PlayerState)
					{
						if (PlayerState->HabaneroComponent()) 
						{
							Wrapper::Text(PlayerState->GetRankStr(), FVector2D(BottomMiddle.X, MostBottom + BottomTextOffset), BoxColor, FontSize, true);
							BottomTextOffset += FontSize + 2;
						}
					}

					if (Settings::Player::Name && Player)
					{
						FString PlayerName = Player->GetPlayerNameSafe(Player, Classes::ActorPlayerNamePrivate);

						if (PlayerName.c_str())
						{
							Wrapper::Text(PlayerName, FVector2D(BottomMiddle.X, MostTop - TopTextOffset), Settings::Colors::TextColor, FontSize, true, false);
							TopTextOffset += 14.0;
						}
					}

					if ( Wound )
					{
						Wrapper::Text( FString( L"Player Knocked" ) , FVector2D( BottomMiddle.X , MostTop - TopTextOffset ) , BoxColor , FontSize , true );
						TopTextOffset += FontSize + 2;
					}

					if (Settings::Player::KillCount && Player && Player->PlayerState())
					{
						int32_t KillCount = Player->PlayerState()->GetKillScore();
						FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double(L"Kills: ", L"(", int(KillCount), L")");

						Wrapper::Text(ConvertedText, FVector2D(BottomMiddle.X, MostBottom - BottomTextOffset), Settings::Colors::TextColor, FontSize, true);
						BottomTextOffset += FontSize + 2;
					}

					if (Settings::Player::LevelESP && Player && Player->PlayerState())
					{
						int32_t KillCount = Player->PlayerState()->SeasonLevelUIDisplay();
						FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double(L"Level: ", L"(", int(KillCount), L")");

						Wrapper::Text(ConvertedText, FVector2D(BottomMiddle.X, MostBottom - BottomTextOffset), Settings::Colors::TextColor, FontSize, true);
						BottomTextOffset += FontSize + 2;
					}

					if ( Settings::Player::Weapon )
					{
						AFortWeapon* CurrentWeapon = Player->CurrentWeapon( );
						if ( CurrentWeapon )
						{
							if ( StaticClasses::GameplayStatics->ObjectIsA( CurrentWeapon , Classes::FortWeapon ) )
							{
								if (Settings::Player::ShowScoped && CurrentWeapon->IsTargeting())
								{
									FLinearColor RenderColor = FLinearColor(1.f, 1.f, 1.f, 1.f);

									Wrapper::Text(FString(L"Scoped"), FVector2D(BottomMiddle.X, MostBottom + BottomTextOffset), RenderColor, FontSize, true);
									BottomTextOffset += FontSize + 2;
								}

								if (Settings::Player::ShowReloading && CurrentWeapon->IsReloading())
								{
									FLinearColor RenderColor = FLinearColor(1.f, 1.f, 1.f, 1.f);

									Wrapper::Text(FString(L"Reloading"), FVector2D(BottomMiddle.X, MostTop - TopTextOffset), Settings::Colors::TextColor, FontSize, true);

									TopTextOffset += FontSize + 2;
								}

								if (Settings::Player::ShowAmmoCount)
								{
									FLinearColor RenderColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
									
									int CurrentAmmo = CurrentWeapon->GetMagazineAmmoCount();
									int MaxAmmo = CurrentWeapon->GetBulletsPerClip();
									if (CurrentAmmo && MaxAmmo)
									{
										FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double(L"Ammo: ", L"(", int(CurrentAmmo), L")");

										Wrapper::Text(ConvertedText, FVector2D(BottomMiddle.X, MostBottom - BottomTextOffset), Settings::Colors::TextColor, FontSize, true);

										BottomTextOffset += FontSize + 2;
									}
								}

								UFortItemDefinition* WeaponData = CurrentWeapon->WeaponData( );
								if ( WeaponData )
								{
									FString DisplayName = WeaponData->ItemName().Get();
									if ( DisplayName )
									{
										FLinearColor RenderColor = FLinearColor( 1.f , 1.f , 1.f , 1.f );

										if (WeaponData->Rarity())
										{
											RenderColor = Custom::GetColorByTier(WeaponData->Rarity());
										}

										bool IsHoldingSniper = CurrentWeapon->WeaponCoreAnimation() == EFortWeaponCoreAnimation::SniperRifle;

										if (IsHoldingSniper)
										{
											FString TempDisplayName = FString(L"Sniper");
										}

										Wrapper::Text( DisplayName , FVector2D( BottomMiddle.X , MostBottom + BottomTextOffset ) , RenderColor , FontSize , true );
										BottomTextOffset += FontSize + 2;
									}
								}
							}
						}
					}

					if (Settings::Player::ThankedBusDriver && AcknowledgedPawn && PlayerState)
					{

						if (PlayerState->bThankedBusDriver())
						{
							Wrapper::Text((FString)L"Thanked Bus Driver", FVector2D(BottomMiddle.X, MostBottom + BottomTextOffset), FLinearColor(0.8f, 0.8f, 0.f, 1.f), FontSize, true);
						}
						else
						{
							Wrapper::Text((FString)L"Did Not Thank Bus Driver", FVector2D(BottomMiddle.X, MostBottom + BottomTextOffset), FLinearColor(0.8f, 0.8f, 0.f, 1.f), FontSize, true);
						}

						BottomTextOffset += FontSize + 2;

					}

					if (Settings::Player::HeroTest && PlayerState && Player)
					{
						FString Result;
						if (Player->GetHeroDisplayNameString(Result))
						{
							FString ConvertedText = StaticClasses::KismetStringLibrary->Concat_StrStr((FString)L"Hero: ", Result);

							Wrapper::Text(Result, FVector2D(BottomMiddle.X, MostTop - TopTextOffset), Settings::Colors::TextColor, FontSize, true);

							TopTextOffset += FontSize + 2;
						}
						else
						{

						}
					}

					if (Settings::Player::Platform)
					{

						FString Platform = PlayerState->GetPlatformSDK();
						//FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Int( Custom::GetNameByPlatform( Platform ) , L"", L"", L"");
						//Wrapper::Text(Custom::GetNameByPlatform(Platform), FVector2D(BottomMiddle.X, MostBottom + BottomTextOffset), Settings::Colors::TextColor, FontSize, true);
						
						Wrapper::Text(Platform, FVector2D(BottomMiddle.X, MostTop - TopTextOffset), Settings::Colors::TextColor, FontSize, true);
						
						TopTextOffset += FontSize + 2;

						//BottomTextOffset += FontSize + 2;
					}

					if ( Settings::Player::Distance )
					{
						FString ConvertedText = StaticClasses::KismetStringLibrary->BuildString_Double( L"" , L"(" , int( Distance ) , L"m)" );
						//Wrapper::Text( ConvertedText , FVector2D( BottomMiddle.X , MostBottom + BottomTextOffset ) , Settings::Colors::TextColor, FontSize , true );
						Wrapper::Text(ConvertedText, FVector2D(BottomMiddle.X, MostBottom - BottomTextOffset), Settings::Colors::TextColor, FontSize, true);

						TopTextOffset += FontSize + 2;
						// BottomTextOffset += FontSize + 2;
					}

					if ( Settings::Player::EyeDirection )
					{
						USceneComponent* RootComponent = Player->RootComponent( );
						FVector FowardVector = RootComponent->GetForwardVector( );
						FowardVector *= 120.0;

						FVector2D EndScreen = FVector2D( );
						if ( Custom::K2_Project( Head + FowardVector , &EndScreen ) )
						{
							Wrapper::Line( HeadScreen , EndScreen , BoxColor , 1.f );
						}
					}
				}
			}
		}
	}


	if (PlayerController && AcknowledgedPawn && Settings::Exploits::PlayerFly)
	{
		if (AcknowledgedPawn && Settings::Exploits::PlayerFly && PlayerController)
		{
			FRootMotionMMEFinishVelocitySettings FinishVelocityParams;

			if (AcknowledgedPawn->CanJump())
				AcknowledgedPawn->Jump();

			StaticClasses::FortMovementBlueprintLibrary->ApplyRootMotionMME(World, AcknowledgedPawn, nullptr, ERootMotionMMEMode::ConstantForce, 1, 1, EMovementMode::MOVE_Flying, -1.0f, 20000.0f, FVector(), FVector(), nullptr, nullptr, FVector(), ERootMotionMMETargetOffsetType::AlignToWorldSpace, nullptr, nullptr, FLT_MAX, nullptr, nullptr, nullptr, 0, FinishVelocityParams, 0, 0, 0.0f, nullptr);
		}
	}


	if (PlayerController && AcknowledgedPawn && Settings::Exploits::YeetToOrbit)
	{
		if (AcknowledgedPawn && Settings::Exploits::PlayerFly && PlayerController)
		{
			FRootMotionMMEFinishVelocitySettings FinishVelocityParams;

			FinishVelocityParams.ClampVelocity = 45.0;
			StaticClasses::FortMovementBlueprintLibrary->ApplyRootMotionMME(World, AcknowledgedPawn, nullptr, ERootMotionMMEMode::ConstantForce, 1, 1, EMovementMode::MOVE_Swimming, -1.0f, 200.0f, FVector(), FVector(), nullptr, nullptr, FVector(), ERootMotionMMETargetOffsetType::AlignToWorldSpace, nullptr, nullptr, FLT_MAX, nullptr, nullptr, nullptr, 0, FinishVelocityParams, true, true, 4.0f, nullptr);
		}
	}

	if ( Settings::Aimbot::Aimbot )
	{
		if ( AcknowledgedPawn )
		{
			if (Settings::Exploits::BulletTPV2 && !Settings::Exploits::AirStuck)
			{
				APlayerPawn_Athena_C* Target = nullptr;

				if (Settings::Aimbot::SmartAimbot)
				{
					if (TargetPlayerClosestToMyself && TargetPlayerClosestToCenter)
					{
						if (ClosestDistanceToMyself > ClosestDistanceToCenter)
							Target = TargetPlayerClosestToCenter;
						else
							Target = TargetPlayerClosestToMyself;
					}
					else if (TargetPlayerClosestToMyself)
						Target = TargetPlayerClosestToMyself;
					else if (TargetPlayerClosestToCenter)
						Target = TargetPlayerClosestToCenter;
				}
				else
					Target = TargetPlayerClosestToCenter;

				if (Target)
				{
					USkeletalMeshComponent* Mesh = Target->Mesh();
					if (Mesh)
					{
						FString TargetBoneName = FString(L"head");

						FVector AimbotPosition = Custom::GetSocketLocation(Mesh, TargetBoneName);
						FVector2D ScreenLocation;
						if (PlayerController->w2s(AimbotPosition, &ScreenLocation))
						{
							Wrapper::Line(ScreenLocation, Variables::ScreenCenter, FromRGB(0, 255, 255), 1.f);
						}

						if (PlayerController->IsInputKeyDown(Keys::Capslock) && Target->Mesh() && AcknowledgedPawn)
						{
							FVector Root = Custom::GetSocketLocation(Target->Mesh(), FString(L"root"));
							double Distance = StaticClasses::KismetMathLibrary->Vector_Distance(Variables::CameraLocation, Root) * 0.01;


							FVector CurrentLocation = AcknowledgedPawn->K2_GetActorLocation();
							//FVector Head = Custom::GetSocketLocation(Target->Mesh(), FString(L"head"));
							FVector TargetLocation = Target->K2_GetActorLocation();
							FVector TargetForwardLoc = StaticClasses::KismetMathLibrary->GetForwardVector(Target->K2_GetActorRotation());
							FVector TargetTeleportLoc = TargetLocation - TargetForwardLoc;
							FRotator TargetLookAtRotation = StaticClasses::KismetMathLibrary->FindLookAtRotation(Variables::CameraLocation, TargetTeleportLoc);

							if (Settings::Exploits::BulletTPV2 && PlayerController && AcknowledgedPawn)
							{
								//AcknowledgedPawn->SetCustomTimeDilation(0.01f);

								if (auto Camera = PlayerController->PlayerCameraManager())
								{
									//Camera->SetCustomTimeDilation(0.01f);
								}
							}

							AcknowledgedPawn->K2_SetActorRotation(TargetLookAtRotation, true);
							AcknowledgedPawn->K2_SetActorLocation(TargetTeleportLoc, false, 0, true);
						}
						else if(PlayerController && AcknowledgedPawn)
						{
							//AcknowledgedPawn->SetCustomTimeDilation(1.f);

							if (auto Camera = PlayerController->PlayerCameraManager())
							{
								Camera->SetCustomTimeDilation(1.f);
							}
						}
					}
				}
			}

			if (Settings::Exploits::BulletTPV2_4 && !Settings::Exploits::AirStuck && Settings::Developer::FreecamTest)
			{
				APlayerPawn_Athena_C* Target = nullptr;

				if (Settings::Aimbot::SmartAimbot)
				{
					if (TargetPlayerClosestToMyself && TargetPlayerClosestToCenter)
					{
						if (ClosestDistanceToMyself > ClosestDistanceToCenter)
							Target = TargetPlayerClosestToCenter;
						else
							Target = TargetPlayerClosestToMyself;
					}
					else if (TargetPlayerClosestToMyself)
						Target = TargetPlayerClosestToMyself;
					else if (TargetPlayerClosestToCenter)
						Target = TargetPlayerClosestToCenter;
				}
				else
					Target = TargetPlayerClosestToCenter;

				if (Target)
				{
					USkeletalMeshComponent* Mesh = Target->Mesh();
					if (Mesh)
					{
						FString TargetBoneName = FString(L"head");

						FVector AimbotPosition = Custom::GetSocketLocation(Mesh, TargetBoneName);
						FVector2D ScreenLocation;
						if (PlayerController->w2s(AimbotPosition, &ScreenLocation))
						{
							Wrapper::Line(ScreenLocation, Variables::ScreenCenter, FromRGB(0, 255, 255), 1.f);
						}

						if (PlayerController->IsInputKeyDown(Keys::Capslock) && Target->Mesh() && AcknowledgedPawn)
						{
							FVector Root = Custom::GetSocketLocation(Target->Mesh(), FString(L"root"));
							double Distance = StaticClasses::KismetMathLibrary->Vector_Distance(Variables::CameraLocation, Root) * 0.01;


							FVector CurrentLocation = AcknowledgedPawn->K2_GetActorLocation();
							FVector Head = Custom::GetSocketLocation(Target->Mesh(), FString(L"head"));
							FVector TargetLocation = Target->K2_GetActorLocation();
							FVector TargetForwardLoc = StaticClasses::KismetMathLibrary->GetForwardVector(Target->K2_GetActorRotation());
							FRotator TargetLookAtRotation = StaticClasses::KismetMathLibrary->FindLookAtRotation(Variables::CameraLocation, Head);
							FVector TargetTeleportLoc = TargetLocation - TargetForwardLoc;

							Update__GetWeaponTargetingTransform__bSilentAimActive = true;
							Update__GetWeaponTargetingTransform__SilentLocationTarget = AimbotPosition;
							
							Variables::GetCameraViewPoint::StickyLocation = Variables::CameraLocation;
							Variables::GetPlayerViewPoint::bShouldStartAtLocationTarget = true; 

							if (Settings::Exploits::BulletTPV2 && PlayerController && AcknowledgedPawn)
							{
								//AcknowledgedPawn->SetCustomTimeDilation(0.01f);

								if (auto Camera = PlayerController->PlayerCameraManager())
								{
									//Camera->SetCustomTimeDilation(0.01f);
								}
							}

							//AcknowledgedPawn->K2_SetActorRotation(TargetLookAtRotation, true);
							AcknowledgedPawn->K2_SetActorLocation(TargetTeleportLoc, false, 0, true);
						}
						else if (PlayerController && AcknowledgedPawn)
						{
							//AcknowledgedPawn->SetCustomTimeDilation(1.f);

							if (auto Camera = PlayerController->PlayerCameraManager())
							{
								Camera->SetCustomTimeDilation(1.f);
							}
						}
					}
				}
			}

			static bool bIsTargetingWithBTPV3 = false;
		

			if (Settings::Developer::FreecamTest && PlayerController && AcknowledgedPawn) // cam hook v2
			{
				if (Settings::Developer::DesyncV2 && !Settings::Developer::ServersideSpinbotV2)
				{
					FKey AimbotKey = Custom::GetAimKey(Settings::Aimbot::KeyType);



					APlayerPawn_Athena_C* Target = nullptr;

					Target = TargetPlayerClosestToCenter;

					FVector CurrentPos = AcknowledgedPawn->K2_GetActorLocation();
					USkeletalMeshComponent* CurrentMesh = AcknowledgedPawn->Mesh();

					if (Target && CurrentMesh)
					{
						FVector HeadPos = Custom::GetSocketLocation(CurrentMesh, FString(L"head"));
						FVector ThighPos = Custom::GetSocketLocation(CurrentMesh, FString(L"thigh_r"));

						FVector FeetPos = HeadPos;
						FeetPos.Z = FeetPos.Z;

						FVector SkyPos = FeetPos;
						SkyPos.Z = HeadPos.Z + 20.0;

						FVector DesyncLocation3D = FVector();
						FRotator DesyncLocationRotation = FRotator();

						if (Settings::Developer::DesyncV2Angle == 0)
							DesyncLocation3D = FeetPos;
						else if (Settings::Developer::DesyncV2Angle == 1)
							DesyncLocation3D = SkyPos;

						GetPlayerViewPoint_bShouldStartAtLocationTarget = true;
						GetPlayerViewPoint_SilentLocationTarget = DesyncLocation3D;

						
						Variables::GetCameraViewPoint::StickyLocation = Variables::CameraLocation;
						Variables::GetPlayerViewPoint::bShouldStartAtLocationTarget = true;
						Variables::GetCameraViewPoint::bFreecamActive = false;
					}

				}

				if ((Settings::Developer::ServersideSpinbotV2 && !Settings::Exploits::Spinbot) && PlayerController && AcknowledgedPawn)
				{
					static int CurrentRotation = 0;

					FRotator CurrentRotationRotator = AcknowledgedPawn->K2_GetActorRotation();
					FRotator RealRotation = AcknowledgedPawn->K2_GetActorRotation();

					Update__bIsUsingSpinbot = true;
					GetPlayerViewPoint_bShouldStartAtLocationTarget = true;

					if (CurrentRotation >= 359)
					{
						CurrentRotation = 0;
					}

					CurrentRotation += Settings::Exploits::SpinSpeed;

					FRotator TargetRotation = RealRotation;
					TargetRotation.Yaw = CurrentRotation;
					

					Update__GetControlRotation__bAimbotActive = true;
					Update__GetControlRotation__AimbotRotationTarget = TargetRotation;

					if (PlayerController)
					{
						Variables::GetPlayerViewPoint::SilentLocationTarget = Variables::CameraLocation;
						Variables::GetCameraViewPoint::StickyLocation = PlayerCameraManager->GetCameraLocation();
						Update__GetWeaponTargetingTransform__bSilentAimActive = true;
						//Variables::GetWeaponTargetingTransform::SilentLocationTarget = AimbotPosition;
						// should handle itself...

						PlayerController->SetAim(TargetRotation);
					}
					 
					
					//Variables::GetPlayerViewPoint::bFreeCam
				}
			}

			AFortWeapon* CurrentWeapon = AcknowledgedPawn->CurrentWeapon( );
			if ( CurrentWeapon )
			{
				static bool bCachedWasBacktrack;
				static APlayerPawn_Athena_C* LastBacktrackTarget = nullptr;
				static double LastBacktrackTimer = 0;

				bool IsHoldingWeapon = StaticClasses::GameplayStatics->ObjectIsA( CurrentWeapon , Classes::FortWeaponRanged );
				if ( IsHoldingWeapon )
				{
					bool IsHoldingPickaxe = StaticClasses::GameplayStatics->ObjectIsA( CurrentWeapon , Classes::WeaponPickaxeAthena );

					
					if (Settings::Exploits::BulletTPV2_1 && !Settings::Exploits::BulletTPV2 && !Settings::Exploits::AirStuck) // crashes idk why
					{
						APlayerPawn_Athena_C* Target = nullptr;

						if (Settings::Aimbot::SmartAimbot)
						{
							if (TargetPlayerClosestToMyself && TargetPlayerClosestToCenter)
							{
								if (ClosestDistanceToMyself > ClosestDistanceToCenter)
									Target = TargetPlayerClosestToCenter;
								else
									Target = TargetPlayerClosestToMyself;
							}
							else if (TargetPlayerClosestToMyself)
								Target = TargetPlayerClosestToMyself;
							else if (TargetPlayerClosestToCenter)
								Target = TargetPlayerClosestToCenter;
						}
						else
							Target = TargetPlayerClosestToCenter;

						if (Target)
						{
							USkeletalMeshComponent* Mesh = Target->Mesh();
							if (Mesh)
							{
								FString TargetBoneName = FString(L"head");

								FVector AimbotPosition = Custom::GetSocketLocation(Mesh, TargetBoneName);
								FVector2D ScreenLocation;
								if (PlayerController->w2s(AimbotPosition, &ScreenLocation))
								{
									Wrapper::Line(ScreenLocation, Variables::ScreenCenter, FromRGB(0, 255, 255), 1.f);
								}

								if (PlayerController->IsInputKeyDown(Keys::Capslock) && Target && Target->Mesh() && AcknowledgedPawn)
								{
									FVector Root = Custom::GetSocketLocation(Target->Mesh(), FString(L"root"));
									double Distance = StaticClasses::KismetMathLibrary->Vector_Distance(Variables::CameraLocation, Root) * 0.01;

									FVector TargetTeleportLoc;
									FRotator TargetLookAt;
									TargetTeleportLoc = PlayerController->GetLocationUnderReticle(); // get reticle location (3d)

									
									TargetLookAt = StaticClasses::KismetMathLibrary->FindLookAtRotation(TargetTeleportLoc, Variables::CameraLocation); // look at camera :3

									FVector2D TargetTeleportLocOnScreenStart; // 2d
									FVector2D TargetTeleportLocOnScreen; // 2d

									if (PlayerController->w2s(TargetTeleportLoc, &TargetTeleportLocOnScreen))
									{
										if (PlayerController->w2s(Target->K2_GetActorLocation(), &TargetTeleportLocOnScreenStart))
										{
											Wrapper::Line(TargetTeleportLocOnScreenStart, TargetTeleportLocOnScreen, FLinearColor(1.0f, 0.55f, 0.f, 1.f), 2.f);
										}

										Wrapper::Circle(TargetTeleportLocOnScreen, FLinearColor(0.0f, 1.f, 1.f, 1.f), 10, 50, false, true);

									}
									
									Target->Mesh()->SetGlobalAnimRateScale(0.f);

									Target->K2_SetActorLocation(TargetTeleportLoc, false, 0, true); // set actor
								}
							}
						}
					}
					
					if ( !IsHoldingPickaxe )
					{
						FVector StartLocation = CurrentWeapon->GetTargetingSourceLocation( );
						bool IsHoldingShotgun = CurrentWeapon->WeaponCoreAnimation( ) == EFortWeaponCoreAnimation::Shotgun;

						FKey AimbotKey = Custom::GetAimKey(Settings::Aimbot::KeyType);


						bool AimbotKeyDown = PlayerController->IsInputKeyDown( AimbotKey ) || PlayerController->IsInputKeyDown( Keys::GamepadLeftTrigger );

						APlayerPawn_Athena_C* Target = nullptr;

						if ( Settings::Aimbot::SmartAimbot )
						{
							if ( TargetPlayerClosestToMyself && TargetPlayerClosestToCenter )
							{
								if ( ClosestDistanceToMyself > ClosestDistanceToCenter )
									Target = TargetPlayerClosestToCenter;
								else
									Target = TargetPlayerClosestToMyself;
							}
							else if ( TargetPlayerClosestToMyself )
								Target = TargetPlayerClosestToMyself;
							else if ( TargetPlayerClosestToCenter )
								Target = TargetPlayerClosestToCenter;
						}
						else
							Target = TargetPlayerClosestToCenter;

						if ( Target )
						{
							USkeletalMeshComponent* Mesh = Target->Mesh( );
							if (Mesh)
							{
								FString TargetBoneName = FString(L"head");

								if (Custom::IsHitscanBone(Settings::Aimbot::BoneType))
								{
									if (Settings::Aimbot::BoneType == 3)
									{
										double MinDistance = DBL_MAX;

										for (const auto& Pair : HitscanBonePairs)
										{
											FVector WorldLocationFirst = Custom::GetSocketLocation(Mesh, Pair.first);
											FVector WorldLocationSecond = Custom::GetSocketLocation(Mesh, Pair.second);

											FVector2D ScreenPositionFirst = FVector2D();
											FVector2D ScreenPositionSecond = FVector2D();

											if (!Custom::K2_Project(WorldLocationFirst, &ScreenPositionFirst)) continue;
											if (!Custom::K2_Project(WorldLocationSecond, &ScreenPositionSecond)) continue;

											FVector2D DeltaFirst = FVector2D(ScreenPositionFirst.X - Variables::ScreenCenter.X, ScreenPositionFirst.Y - Variables::ScreenCenter.Y);
											double DistanceFirst = StaticClasses::KismetMathLibrary->sqrt(DeltaFirst.X * DeltaFirst.X + DeltaFirst.Y * DeltaFirst.Y);

											FVector2D DeltaSecond = FVector2D(ScreenPositionSecond.X - Variables::ScreenCenter.X, ScreenPositionSecond.Y - Variables::ScreenCenter.Y);
											double DistanceSecond = StaticClasses::KismetMathLibrary->sqrt(DeltaSecond.X * DeltaSecond.X + DeltaSecond.Y * DeltaSecond.Y);

											if (!StaticClasses::FortKismetLibrary->CheckLineOfSightToActorWithChannel(Variables::CameraLocation, Target, ECollisionChannel::ECC_Visibility, AcknowledgedPawn))
											{
												if (DistanceFirst < MinDistance && ViewportClient->LineTraceSingle(World, Variables::CameraLocation, WorldLocationFirst))
												{
													MinDistance = DistanceFirst;
													TargetBoneName = Pair.first;
												}

												if (DistanceSecond < MinDistance && ViewportClient->LineTraceSingle(World, Variables::CameraLocation, WorldLocationSecond))
												{
													MinDistance = DistanceSecond;
													TargetBoneName = Pair.second;
												}
											}
										}
									}
								}
								else
								{
									TargetBoneName = Custom::GetAimBone(Settings::Aimbot::BoneType);
								}

								FVector2D ScreenLocation = FVector2D();
								FVector AimbotPosition = Custom::GetSocketLocation(Mesh, TargetBoneName);
								if (Custom::K2_Project(AimbotPosition, &ScreenLocation))
								{
									double PlayerDistance = StaticClasses::KismetMathLibrary->Vector_Distance(StartLocation, AimbotPosition);
									// bool IsVisble = (Settings::Aimbot::VisibleOnly && ViewportClient->LineTraceSingle(World, Variables::CameraLocation, AimbotPosition) ||;
									bool Visable = StaticClasses::FortKismetLibrary->CheckLineOfSightToActorWithChannel(Variables::CameraLocation, Target, ECollisionChannel::ECC_Visibility, nullptr);
									bool IsVisble = ((Settings::Aimbot::VisibleOnly && Visable) || (!Settings::Aimbot::VisibleOnly));

									if (Settings::Aimbot::Pred && Settings::Aimbot::MemoryAim)
									{
										FVector CurrentVelocity = Target->GetVelocity();
										if (Target->IsInVehicle() || Target->IsDrivingVehicle()) {
											AFortAthenaVehicle* CurrentVehicle = Target->CurrentVehicle();
											if (CurrentVehicle) {
												CurrentVelocity = CurrentVehicle->GetVelocity();
											}
										}

										FVector LocalVelocity = AcknowledgedPawn->GetVelocity();
										if (AcknowledgedPawn->IsInVehicle() || AcknowledgedPawn->IsDrivingVehicle()) {
											AFortAthenaVehicle* CurrentVehicle = AcknowledgedPawn->CurrentVehicle();
											if (CurrentVehicle) {
												LocalVelocity = CurrentVehicle->GetVelocity();
											}
										}

										FVector RelativeVelocity = CurrentVelocity - LocalVelocity;

										UFortWeaponItemDefinition* ItemDefinition = CurrentWeapon->WeaponData();
										if (ItemDefinition) {
											bool HasLightweightProjectile = ItemDefinition->HasLightweightProjectile();
											if (HasLightweightProjectile) {
												double ProjectileSpeed = CurrentWeapon->GetProjectileSpeed();
												double ProjectileGravity = CurrentWeapon->GetProjectileGravity();

												double TimeToTarget = PlayerDistance / ProjectileSpeed;

												AimbotPosition.X += RelativeVelocity.X * TimeToTarget;
												AimbotPosition.Y += RelativeVelocity.Y * TimeToTarget;

												double Difference = StartLocation.Z - AimbotPosition.Z;
												double HorizontalDistance = StaticClasses::KismetMathLibrary->sqrt(
													StaticClasses::KismetMathLibrary->Pow(StartLocation.X - AimbotPosition.X, 2.0) +
													StaticClasses::KismetMathLibrary->Pow(StartLocation.Y - AimbotPosition.Y, 2.0)
												);
												double VerticalAngle = StaticClasses::KismetMathLibrary->Atan2(
													StaticClasses::KismetMathLibrary->abs(Difference), HorizontalDistance
												);

												double Angle = VerticalAngle * (180.0 / M_PI);
												double Factor = StaticClasses::KismetMathLibrary->abs(Angle - 90.0) / 90.0;
												ProjectileGravity *= Factor;

												AimbotPosition.Z += RelativeVelocity.Z * TimeToTarget +
													(StaticClasses::KismetMathLibrary->abs(ProjectileGravity * -980.0) * (TimeToTarget * TimeToTarget)) * 0.5;
											}
										}

										if (Settings::Aimbot::PredictionDot)
										{
											FVector2D PredictionLocation = FVector2D();
											if (Custom::K2_Project(AimbotPosition, &PredictionLocation))
											{
												double CircleRaduis = Variables::ScreenSize.Y / (2.0 * PlayerDistance *
													StaticClasses::KismetMathLibrary->tan(Variables::FieldOfView * M_PI / 360.0)) * 8.0;
												Wrapper::Circle(PredictionLocation, FLinearColor(1.f, 0.f, 0.f, 1.f), CircleRaduis, 138.0, true, true);
											}
										}
									}


									if ((Settings::Aimbot::VisibleOnly && IsVisble || !Settings::Aimbot::VisibleOnly) && AcknowledgedPawn)
									{
										if (Settings::Aimbot::MouseAim)
										{
											if (Target && AcknowledgedPawn && CurrentWeapon)
											{
												if (AimbotKeyDown)
												{
													auto aim_location = AimbotPosition;
													float width = static_cast<float>(Canvas->ClipX());
													float height = static_cast<float>(Canvas->ClipY());
													if (aim_location && width && height && Target) {
														set_aim_mouse(PlayerController, aim_location, width, height, Settings::Aimbot::Speed);
													}
												}
											}
										}


										if (Settings::Aimbot::TargetLine)
										{
											FVector2D MuzzleScreen = FVector2D();
											FVector Muzzle = CurrentWeapon->GetMuzzleLocation();
											if (Custom::K2_Project(Muzzle, &MuzzleScreen))
											{
												Wrapper::Line(MuzzleScreen, ScreenLocation, Settings::Colors::TargetLine, 1.f);
											}
										}

										if (Settings::Exploits::InstantKillAll && AimbotKeyDown && AcknowledgedPawn)
										{
											FVector TeleportLocation = Target->K2_GetActorLocation() + FVector(30.0, 30.0, 30.0);
											USceneComponent* RootComponent = AcknowledgedPawn->RootComponent();
											*(FVector*)(RootComponent + 0x1E0) = TeleportLocation;
										}

										if (Settings::Exploits::BulletTP && AcknowledgedPawn)
										{
											static UObject* Class_Projectile;
											if (!Class_Projectile) {
												Class_Projectile = UObject::FindObjectSingle <UObject*>(L"FortniteGame.FortProjectileBase");
											}
											TArray<UObject*> projectile_array = StaticClasses::GameplayStatics->GetAllActorsOfClass(World, Class_Projectile);
											for (int i = 0; i < projectile_array.NumElements; i++) {
												if (!projectile_array.Valid(i)) continue;

												auto projectile = (AActor*)projectile_array[i];
												if (!projectile) continue;

												auto object_name = StaticClasses::KismetSystemLibrary->GetObjectName(projectile);

												//if (!StaticClasses::KismetStringLibrary->Contains(object_name.c_str(), L"Bullet", false, false))
													//continue;

												projectile->SetActorLocation(AimbotPosition, false, 0, false);
											}
										}


										if (Settings::Aimbot::Triggerbot && World && PlayerController && CurrentWeapon && PlayerController->TargetedFortPawn())
										{
											AFortPawn* TargetPawn = PlayerController->TargetedFortPawn();
											if (TargetPawn == Target)
											{
												//bool Visable = StaticClasses::FortKismetLibrary->CheckLineOfSightToActorWithChannel(Variables::CameraLocation, TargetPawn, ECollisionChannel::ECC_Visibility, nullptr);
												//if (Visable)
												//{
												if ((Settings::Aimbot::TriggerbotConstant) || (Settings::Aimbot::TriggerbotLeftShift && PlayerController->IsInputKeyDown(Keys::LeftShift)))
												{

													for (int i = 0; i < CurrentWeapon->GetRemainingAmmo(); i++)
													{
														PlayerController->AcknowledgedPawn()->PawnStartFire();
														PlayerController->AcknowledgedPawn()->PawnStopFire();
													}
												}
												//}
											}
										}

										if (Settings::Aimbot::Pred)
										{
											UFortWeaponItemDefinition* ItemDefinition = CurrentWeapon->WeaponData();
											bool HasLightweightProjectile = ItemDefinition->HasLightweightProjectile();
											if (HasLightweightProjectile)
											{
												FVector CurrentVelocity = Target->GetVelocity();
												if (Target->IsInVehicle() || Target->IsDrivingVehicle())
												{
													AFortAthenaVehicle* CurrentVehicle = Target->CurrentVehicle();

													CurrentVelocity = CurrentVehicle->GetVelocity();
												}

												FVector LocalVelocity = AcknowledgedPawn->GetVelocity();
												if (AcknowledgedPawn->IsInVehicle() || AcknowledgedPawn->IsDrivingVehicle())
												{
													AFortAthenaVehicle* CurrentVehicle = AcknowledgedPawn->CurrentVehicle();

													LocalVelocity = CurrentVehicle->GetVelocity();
												}

												FVector RelativeVelocity = CurrentVelocity - LocalVelocity;

												double ProjectileSpeed = static_cast<double>(10.1);
												double ProjectileGravity = static_cast<double>(31.1);

												double TimeToTarget = PlayerDistance / ProjectileSpeed;

												AimbotPosition.X += RelativeVelocity.X * TimeToTarget;
												AimbotPosition.Y += RelativeVelocity.Y * TimeToTarget;

												double Difference = StartLocation.Z - AimbotPosition.Z;
												double HorizontalDistance = StaticClasses::KismetMathLibrary->sqrt(StaticClasses::KismetMathLibrary->Pow(StartLocation.X - AimbotPosition.X, 2.0) +
													StaticClasses::KismetMathLibrary->Pow(StartLocation.Y - AimbotPosition.Y, 2.0));
												double VerticalAngle = StaticClasses::KismetMathLibrary->Atan2
												(StaticClasses::KismetMathLibrary->abs(Difference), HorizontalDistance);

												double Angle = VerticalAngle * (180.0 / M_PI);
												double Factor = StaticClasses::KismetMathLibrary->abs(Angle - 90.0) / 90.0;
												ProjectileGravity *= Factor;

												AimbotPosition.Z += RelativeVelocity.Z * TimeToTarget + (StaticClasses::KismetMathLibrary->abs
												(ProjectileGravity * -980.0) * (TimeToTarget * TimeToTarget)) * 0.5;
											}

											if (Settings::Aimbot::PredictionDot)
											{
												FVector2D PredictionLocation = FVector2D();
												if (Custom::K2_Project(AimbotPosition, &PredictionLocation))
												{
													double CircleRaduis = Variables::ScreenSize.Y / (2.0 * PlayerDistance *
														StaticClasses::KismetMathLibrary->tan(Variables::FieldOfView * M_PI / 360.0)) * 8.0;
													Wrapper::Circle(PredictionLocation, FLinearColor(1.f, 0.f, 0.f, 1.f), CircleRaduis, 138.0, true, true);
												}
											}
										}

										if (Settings::Exploits::CarFly && AcknowledgedPawn && PlayerController)
										{
											if (Settings::Exploits::TeleportMethod == 3 && PlayerController->IsInputKeyDown(Keys::F8))
											{
												if (AcknowledgedPawn && AcknowledgedPawn->IsInVehicle() || AcknowledgedPawn->IsDrivingVehicle())
												{
													AFortAthenaVehicle* CurrentVehicle = AcknowledgedPawn->CurrentVehicle();

													if (CurrentVehicle && Target)
													{
														FVector2D ScreenLocation = FVector2D();
														FVector2D TargetScreenLocation = FVector2D();
														FVector2D LocationScreenPosition = FVector2D();

														FVector TargetTPLocation = Custom::GetSocketLocation(Mesh, TargetBoneName);
														FVector TargetTPLocationAltered = TargetTPLocation;
														FVector LocalPosition = CurrentVehicle->K2_GetActorLocation();
														FRotator LocalRot = CurrentVehicle->K2_GetActorRotation();
														FVector LocalForwardVec = StaticClasses::KismetMathLibrary->GetForwardVector(LocalRot);

														LocalPosition.X += LocalForwardVec.X;
														LocalPosition.Y += LocalForwardVec.Y;
														LocalPosition.Z += LocalForwardVec.Z;


														TargetTPLocationAltered.Z += 25;
														TargetTPLocationAltered.Y += 25;

														if (PlayerController->w2s(TargetTPLocation, &ScreenLocation) &&
															PlayerController->w2s(TargetTPLocationAltered, &TargetScreenLocation) &&
															PlayerController->w2s(LocalPosition, &LocationScreenPosition))
														{
															Wrapper::Circle(ScreenLocation, FromRGB(255, 5, 0), 30, 100);
															//Wrapper::Line(LocationScreenPosition, TargetScreenLocation, FromRGB(0, 255, 10), 2.f);
															//Wrapper::Line(LocationScreenPosition, ScreenLocation, FromRGB(0, 255, 255), 3.f);

															CurrentVehicle->K2_TeleportTo(TargetTPLocationAltered, FRotator(0, 0, 0));
														}

													}
												}
											}
										}

										if (Settings::Exploits::TargetStrafe && Target && AcknowledgedPawn && Target->Mesh() && World && PlayerController && PlayerController->PlayerCameraManager())
										{
											FVector Root = Custom::GetSocketLocation(Target->Mesh(), FString(L"root"));
											double Distance = StaticClasses::KismetMathLibrary->Vector_Distance(Variables::CameraLocation, Root) * 0.01;

											if (15.0 >= Distance)
											{
												static double LastTargetStrafeActionTime = 0.0;

												if (StaticClasses::KismetSystemLibrary->GetGameTimeInSeconds(World) - LastTargetStrafeActionTime >= 0.10)
												{
													FVector CurrentLocation = AcknowledgedPawn->K2_GetActorLocation(); // save for using as a findlookatrotation
													FVector TargetLocation = Target->K2_GetActorLocation(); // this is obvious why we need
													FRotator TargetRotation = Target->K2_GetActorRotation(); // this is obvious why we need
													FRotator CameraCurrentRotation = PlayerController->PlayerCameraManager()->GetCameraRotation(); // this is obvious why we need


													double TeleportYOffset = StaticClasses::KismetMathLibrary->RandomIntegerInRange(5, 10);

													FVector TargetLocationForTeleport = TargetLocation;
													TargetLocationForTeleport += StaticClasses::KismetMathLibrary->GetRightVector(TargetRotation);
													TargetLocationForTeleport -= StaticClasses::KismetMathLibrary->GetForwardVector(TargetRotation);
													TargetLocationForTeleport.X += StaticClasses::KismetMathLibrary->RandomIntegerInRange(5, 10);
													TargetLocationForTeleport.X -= StaticClasses::KismetMathLibrary->RandomIntegerInRange(5, 10);

													if (Settings::Exploits::TargetStrafeLineTraces)
													{
														FVector2D LineStartLocation = FVector2D();
														FVector2D LineStartLocation2 = FVector2D();
														FVector2D LineStartLocation3 = FVector2D();
														FVector2D LineEndLocation = FVector2D();
														FVector2D LineEndLocation2 = FVector2D();
														FVector2D LineEndLocation3 = FVector2D();

														if (PlayerController->w2s(TargetLocationForTeleport, &LineEndLocation) && PlayerController->w2s(TargetLocation, &LineStartLocation))
														{
															Wrapper::Line(LineStartLocation, LineEndLocation, FromRGB(255, 255, 0), 4.f);
														}

														if (PlayerController->w2s(CurrentLocation, &LineStartLocation2) && PlayerController->w2s(TargetLocation, &LineEndLocation2))
														{
															Wrapper::Line(LineStartLocation2, LineEndLocation2, FromRGB(0, 255, 255), 4.f);
														}
													}


													if (!Settings::Exploits::TargetStrafeLineTracesNTP)
													{
														AcknowledgedPawn->K2_TeleportTo(TargetLocationForTeleport, CameraCurrentRotation);
													}

													LastTargetStrafeActionTime = StaticClasses::KismetSystemLibrary->GetGameTimeInSeconds(World);
												}
											}
										}

										if (Settings::Exploits::SilentAimbot && AcknowledgedPawn)
										{
											Update__GetWeaponTargetingTransform__bSilentAimActive = true;
											Update__GetWeaponTargetingTransform__SilentLocationTarget = AimbotPosition;
										}
										else if (Settings::Developer::Debugging::StickySilentAimV2 && AcknowledgedPawn)
										{
											Variables::GetPlayerViewPoint::SilentLocationTarget = AimbotPosition;
											//Variables::GetCameraViewPoint::StickyLocation = PlayerCameraManager->GetCameraLocation();
											Update__GetWeaponTargetingTransform__bSilentAimActive = true;
										}
										else if ((AimbotKeyDown && Settings::Aimbot::MemoryAim) && AcknowledgedPawn)
										{
											if (StaticClasses::KismetMathLibrary)
											{

												if (Variables::CameraLocation && Variables::CameraRotation && AimbotPosition)
												{

													/*AActor* GameState = (AActor*)World->GameState();
													*(uintptr_t*)((uintptr_t)PlayerController + 0x3d80) = (uintptr_t)GameState;
													auto vOrigin = GameState->K2_GetActorLocation();
													FVector vOffset = AimbotPosition - vOrigin;
													*(FVector*)((uintptr_t)PlayerController + 0x3d88) = vOffset;
													*(float*)((uintptr_t)PlayerController + 0x3da4) = FLT_MIN;
													*(float*)((uintptr_t)GameState + 0x1798) = Settings::Aimbot::Smoothing * 2.f;*/

													FRotator TargetRotation = StaticClasses::KismetMathLibrary->FindLookAtRotation(Variables::CameraLocation, AimbotPosition);

													TargetRotation = TargetRotation.Normalize();

													if (TargetRotation)
													{

														if (Settings::Aimbot::SmoothingType && Settings::Aimbot::Smoothing >= 0.0)
														{
															/*if (Settings::Aimbot::SmoothingType == 1)
															{*/
															TargetRotation = StaticClasses::KismetMathLibrary->RInterpTo(Variables::CameraRotation, TargetRotation, WorldDeltaSeconds, (30.0 - Settings::Aimbot::Smoothing));
															/* }
															else if (Settings::Aimbot::SmoothingType == 2)
															{
																TargetRotation = StaticClasses::KismetMathLibrary->RInterpTo_Constant(Variables::CameraRotation, TargetRotation, WorldDeltaSeconds, Settings::Aimbot::Smoothing);
															}*/

															TargetRotation += (TargetRotation - Variables::CameraRotation);
														}

														if (Settings::Aimbot::Shake)
														{
															if (Settings::Aimbot::ShakeSpeed >= 0.0)
															{
																double CurrentTime = StaticClasses::GameplayStatics->GetTimeSeconds(World);

																//double ShakeFactor = StaticClasses::KismetMathLibrary->sin(CurrentTime * Settings::Aimbot::ShakeSpeed);
																double ShakeFactor = StaticClasses::KismetMathLibrary->sin(CurrentTime * Settings::Aimbot::ShakeSpeed);

																double MaxPitchShake = 5.0 * +StaticClasses::KismetMathLibrary->sin(CurrentTime * StaticClasses::KismetMathLibrary->RandomIntegerInRange(0, static_cast<int32>((static_cast<int32>(Settings::Aimbot::ShakeSpeed) - Settings::Aimbot::ShakeFactorSmooth))));

																double PitchShake = MaxPitchShake * ShakeFactor;

																TargetRotation.Pitch += PitchShake;


																// old
																// StaticClasses::KismetMathLibrary->sin(TimeSeconds * 
																// StaticClasses::KismetMathLibrary->RandomIntegerInRange(0,
																// static_cast<int32>(Settings::Aimbot::ShakeSpeed)));

																double DistanceFactor = StaticClasses::KismetMathLibrary->Clamp(PlayerDistance, 1.0, 7.0);

																if (Settings::Aimbot::ShakeXOnly || Settings::Aimbot::ShakeYOnly)
																{
																	if (Settings::Aimbot::ShakeYOnly)
																	{
																		AimbotPosition.Y -= ShakeFactor * DistanceFactor;
																	}
																	else if (Settings::Aimbot::ShakeXOnly)
																	{
																		AimbotPosition.X += ShakeFactor * DistanceFactor;
																	}
																}
																else
																{
																	double PitchShake = MaxPitchShake * ShakeFactor;

																	TargetRotation.Pitch += PitchShake;

																}
															}
														}

														// Fix the final TargetRotation again after interpolation
														TargetRotation = TargetRotation.Normalize();

														Update__GetControlRotation__bAimbotActive = true;
														Update__GetControlRotation__AimbotRotationTarget = TargetRotation;

														// Check if PlayerController is valid before using it
														if (PlayerController)
														{
															if (Settings::Exploits::bBackTrack) // conspiracy MADE THIS SHIT PLEASE NO LEAKKKK!
															{
																bool bWantsBacktrack = (PlayerController->IsInputKeyDown(Keys::LeftMouseButton) ||
																	PlayerController->IsInputKeyDown(Keys::Tab));

																if (bWantsBacktrack)
																{
																	Target->SetCustomTimeDilation(0.001f); // if any other chink says they made this exploit i will fucking
																	// gut you -conspiracy
																}
																else
																{
																	Target->SetCustomTimeDilation(1.f);
																	// if any other chink says they made this exploit i will fucking
																	// gut you -conspiracy
																}

															}

															if ((Settings::Developer::ServersideSpinbotV3 && Settings::Developer::Debugging::StaticStickyViewpoint) && !Settings::Exploits::SilentAimbotV2)
															{
																static int Spin = 0;

																if (Spin >= 359)
																{
																	Spin = 0;
																}

																Spin += Settings::Exploits::SpinSpeed;

																Variables::GetPlayerViewPoint::SilentLocationTarget = AimbotPosition;
																Variables::GetCameraViewPoint::StickyLocation = PlayerCameraManager->GetCameraLocation();
																Update__GetWeaponTargetingTransform__bSilentAimActive = true;
																Variables::GetWeaponTargetingTransform::SilentLocationTarget = AimbotPosition;
															}
															else
															{
																PlayerController->SetAim(TargetRotation);
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if (Settings::Player::CrosshairMode)
	{
		/*if (AcknowledgedPawn)
		{
			if (AFortWeapon* CurrentWeapon = AcknowledgedPawn->CurrentWeapon())
			{
				////CurrentWeapon->EnableAltCenterReticle(false);
				///CurrentWeapon->EnableAltOuterReticle(false);
				////CurrentWeapon->EnableCornersReticle(false);
			}
		}*/

		if (Settings::Player::CrosshairMode == 1)
		{
			Wrapper::Line(FVector2D(Variables::ScreenCenter.X - 10.0, Variables::ScreenCenter.Y), FVector2D(Variables::ScreenCenter.X + 10.0, Variables::ScreenCenter.Y), FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 1.f);
			Wrapper::Line(FVector2D(Variables::ScreenCenter.X, Variables::ScreenCenter.Y - 10.0), FVector2D(Variables::ScreenCenter.X, Variables::ScreenCenter.Y + 10.0), FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 1.f);
		}
		else if (Settings::Player::CrosshairMode == 2)
		{
			Wrapper::Line(FVector2D(Variables::ScreenCenter.X - 14.0, Variables::ScreenCenter.Y), FVector2D(Variables::ScreenCenter.X - 8.0, Variables::ScreenCenter.Y), FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 2.f);
			Wrapper::Line(FVector2D(Variables::ScreenCenter.X + 8.0, Variables::ScreenCenter.Y), FVector2D(Variables::ScreenCenter.X + 14.0, Variables::ScreenCenter.Y), FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 2.f);
			Wrapper::Line(FVector2D(Variables::ScreenCenter.X, Variables::ScreenCenter.Y - 14.0), FVector2D(Variables::ScreenCenter.X, Variables::ScreenCenter.Y - 8.0), FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 2.f);
			Wrapper::Line(FVector2D(Variables::ScreenCenter.X, Variables::ScreenCenter.Y + 8.0), FVector2D(Variables::ScreenCenter.X, Variables::ScreenCenter.Y + 14.0), FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 2.f);
		}
		else if (Settings::Player::CrosshairMode == 3)
		{
			Wrapper::Circle(Variables::ScreenCenter, FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 5.0, 38.0);
		}
		else if (Settings::Player::CrosshairMode == 4)
		{
			Wrapper::Line(FVector2D(Variables::ScreenCenter.X - 10.0, Variables::ScreenCenter.Y - 10.0), FVector2D(Variables::ScreenCenter.X + 10.0, Variables::ScreenCenter.Y + 10.0), FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 1.f);
			Wrapper::Line(FVector2D(Variables::ScreenCenter.X - 10.0, Variables::ScreenCenter.Y + 10.0), FVector2D(Variables::ScreenCenter.X + 10.0, Variables::ScreenCenter.Y - 10.0), FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 1.f);

			Wrapper::Line(FVector2D(Variables::ScreenCenter.X - 10.0, Variables::ScreenCenter.Y - 10.0), FVector2D(Variables::ScreenCenter.X - 5.0, Variables::ScreenCenter.Y - 5.0), FLinearColor(0.f, 0.f, 0.f, 1.f), 2.f);
			Wrapper::Line(FVector2D(Variables::ScreenCenter.X - 10.0, Variables::ScreenCenter.Y + 10.0), FVector2D(Variables::ScreenCenter.X - 5.0, Variables::ScreenCenter.Y + 5.0), FLinearColor(0.f, 0.f, 0.f, 1.f), 2.f);
			Wrapper::Line(FVector2D(Variables::ScreenCenter.X + 10.0, Variables::ScreenCenter.Y + 10.0), FVector2D(Variables::ScreenCenter.X + 5.0, Variables::ScreenCenter.Y + 5.0), FLinearColor(0.f, 0.f, 0.f, 1.f), 2.f);
			Wrapper::Line(FVector2D(Variables::ScreenCenter.X + 10.0, Variables::ScreenCenter.Y - 10.0), FVector2D(Variables::ScreenCenter.X + 5.0, Variables::ScreenCenter.Y - 5.0), FLinearColor(0.f, 0.f, 0.f, 1.f), 2.f);
		}
		else if (Settings::Player::CrosshairMode == 5 && World && Canvas)
		{
			
			int buyukluk = 50;
			int a = (int)(buyukluk);
			float gamma = atan(static_cast<float>(a) / a);

			FVector2D CenterScreen(Canvas->ClipX() / 2, Canvas->ClipY() / 2);

			static int RotationSpeed = 14;
			RotationSpeed++;
			int RotationAngle = RotationSpeed;

			for (int i = 0; i < 4; i++)
			{
				std::vector<int> p;
				p.push_back(static_cast<int>(a * StaticClasses::KismetMathLibrary->sin(GRD_TO_BOG(RotationAngle + (i * 90)))));                                    // p[0]        p0_A.x
				p.push_back(static_cast<int>(a * StaticClasses::KismetMathLibrary->cos(GRD_TO_BOG(RotationAngle + (i * 90)))));                                    // p[1]        p0_A.y
				p.push_back(static_cast<int>((a / StaticClasses::KismetMathLibrary->cos(gamma)) * StaticClasses::KismetMathLibrary->sin(GRD_TO_BOG(RotationAngle + (i * 90) + BOG_TO_GRD(gamma))))); // p[2]        p0_B.x
				p.push_back(static_cast<int>((a / StaticClasses::KismetMathLibrary->cos(gamma)) * StaticClasses::KismetMathLibrary->cos(GRD_TO_BOG(RotationAngle + (i * 90) + BOG_TO_GRD(gamma))))); // p[3]        p0_B.y

				FVector2D p0(CenterScreen.X, CenterScreen.Y);
				FVector2D p1(CenterScreen.X + p[0], CenterScreen.Y - p[1]);
				FVector2D p2(CenterScreen.X + p[2], CenterScreen.Y - p[3]);

				// Generate radial rainbow effect
				FLinearColor lineColor = FromRGB(0, 0, 0);
				//}

				// Drawing the two lines for each arm of the crosshair
				Wrapper::Line(p0, p1, lineColor, 2.0f); // Thickness is set to 2.0 as an example
				Wrapper::Line(p1, p2, lineColor, 2.0f);
			}
		}
	}

#if !NOT_STRIPPED_FROM_PROD
	if (Settings::Developer::Debugging::UIDebugging)
	{
		if (Custom::DebugUIBegin()) // already setup by default cus I made this shit debug wrapper that prob crashes :fire:
		{
			if (Custom::DebugState(Settings::Developer::Debugging::ActorDebugging) && AcknowledgedPawn)
			{
				Wrapper::Text(FString(L"\r\nActor Debug Mode, data: \r\n"), Custom::GetDebugLoc(), FLinearColor(1.f, 1.f, 1.f, 1.f));
				if (auto CurrentWeapon = AcknowledgedPawn->CurrentWeapon())
				{
					
					FString PlatformUserStr = StaticClasses::KismetStringLibrary->Concat_StrStr(FString(L"Platform Username: "), StaticClasses::KismetSystemLibrary->GetPlatformUserName());
					Custom::WriteDebugUIStringEx(PlatformUserStr, VixenDebugMessageType::Error); // orange

					FString PlatformUserDirStr = StaticClasses::KismetStringLibrary->Concat_StrStr(FString(L"Platform User Dir: "), StaticClasses::KismetSystemLibrary->GetPlatformUserDir());
					Custom::WriteDebugUIStringEx(PlatformUserDirStr, VixenDebugMessageType::Error); // orange

					FString SlidingEnabledStr = StaticClasses::KismetStringLibrary->BuildString_Int(FString(L"IsSlidingEnabled: "), L"", AcknowledgedPawn->IsSlidingEnabled(), L"");
					Custom::WriteDebugUIString_Bool(SlidingEnabledStr, VixenDebugMessageType::Misc); // Cyan
					
					FString CanCrouchStr = StaticClasses::KismetStringLibrary->BuildString_Int(FString(L"CanCrouch: "), L"", AcknowledgedPawn->CanCrouch(), L"");
					Custom::WriteDebugUIString_Bool(CanCrouchStr, VixenDebugMessageType::Misc); // Cyan

					FString CanJumpStr = StaticClasses::KismetStringLibrary->BuildString_Int(FString(L"CanJump: "), L"", AcknowledgedPawn->CanJump(), L"");
					Custom::WriteDebugUIString_Bool(CanJumpStr, VixenDebugMessageType::Misc); // Cyan
					
					if (Settings::Developer::Debugging::UnsafeActorDebugging)
					{
						FString CanJumpStr = StaticClasses::KismetStringLibrary->BuildString_Int(FString(L"bIsJumping: "), L"", AcknowledgedPawn->bIsJumping(), L"");
						Custom::WriteDebugUIString_Bool(CanJumpStr, VixenDebugMessageType::Error); // Red

						FString SlidingEnabledStr = StaticClasses::KismetStringLibrary->BuildString_Int(FString(L"bIsSliding: "), L"", AcknowledgedPawn->bIsSliding(), L"");
						Custom::WriteDebugUIString_Bool(SlidingEnabledStr, VixenDebugMessageType::Error); // Red

						Custom::WriteDebugUIString_StringifiedTimeByFloat(FString(L"GetTimeToNextFire"), CurrentWeapon->GetTimeToNextFire());
					}

					//StaticClasses::KismetSystemLibrary->PrintText(World, Text, true, true, FLinearColor(0.f, 1.f, 1.f, 1.f), 10.f, TestID);
				}
			}

			if (AcknowledgedPawn)
			{
				if (Settings::Developer::Debugging::HealthTestOthers)
				{
					Custom::WriteDebugUIString_Bool(FString(L"bDidHealthTestFail"), Settings::Developer::Debugging::bDidHealthTestFail);
					Custom::WriteDebugUIString_Bool(FString(L"bDidShieldTestFail"), Settings::Developer::Debugging::bDidShieldTestFail);
					Custom::WriteDebugUIString_Float(FString(L"MostRecentHealth"), Settings::Developer::Debugging::MostRecentHealth);
					Custom::WriteDebugUIString_Float(FString(L"MostRecentShield"), Settings::Developer::Debugging::MostRecentShield);
				}

				Custom::WriteDebugUIString_Float(FString(L"AckPHealth"), AcknowledgedPawn->GetHealth());
				Custom::WriteDebugUIString_Float(FString(L"AckPShield"), AcknowledgedPawn->GetShield());
				Custom::WriteDebugUIString_Float(FString(L"AckPStamina"), AcknowledgedPawn->GetStamina());
				Custom::WriteDebugUIString_Float(FString(L"AckPMaxStamina"), AcknowledgedPawn->GetMaxStamina());
			}

			if(Custom::DebugState(Settings::Developer::Debugging::SpreadDebugging) && AcknowledgedPawn)
			{
				if (auto CurrentWeapon = AcknowledgedPawn->CurrentWeapon())
				{
					Custom::WriteDebugUIStringEx(FString(L"\r\nCurrentWeapon Debug"), VixenDebugMessageType::Misc); // cyan
					Custom::WriteDebugUIString_Float(FString(L"BulletCountForPerBulletSpread"), CurrentWeapon->BulletCountForPerBulletSpread());
					Custom::WriteDebugUIString_Float(FString(L"CalculatedReticleSpread"), CurrentWeapon->GetCalculatedReticleSpread());
					Custom::WriteDebugUIString_Float(FString(L"GetCurrentReticleSpreadMultiplier"), CurrentWeapon->GetCurrentReticleSpreadMultiplier());
					Custom::WriteDebugUIString_Float(FString(L"GetCurrentSpreadInDegrees"), CurrentWeapon->GetCurrentSpreadInDegrees());
					Custom::WriteDebugUIString_Float(FString(L"GetMaxAdditionalPatternSpreadDegrees"), CurrentWeapon->GetMaxAdditionalPatternSpreadDegrees());
					Custom::WriteDebugUIString_Float(FString(L"GetOverheatingMaxValue"), CurrentWeapon->GetOverheatingMaxValue());
					Custom::WriteDebugUIString_Bool(FString(L"bReticleCornerOutsideSpreadRadius"), CurrentWeapon->bReticleCornerOutsideSpreadRadius());
					Custom::WriteDebugUIString_Bool(FString(L"bShouldUsePerfectAimWhenTargetingMinSpread"), CurrentWeapon->bShouldUsePerfectAimWhenTargetingMinSpread());
					Custom::WriteDebugUIString_Bool(FString(L"IsProjectileWeapon"), CurrentWeapon->IsProjectileWeapon());
					Custom::WriteDebugUIString_Bool(FString(L"DoesWeaponOverheat"), CurrentWeapon->IsProjectileWeapon());


					//Custom::WriteDebugUIStringEx(FString(L"CurrentWeapon Test Debug"), VixenDebugMessageType::Misc); // cyan
					//Custom::WriteDebugUIString_Double(FString(L"WeaponTriggerType"), (uint8)CurrentWeapon->WeaponData()->GetTriggerType());
					//Custom::WriteDebugUIString_Bool(FString(L"bWeaponTriggerTypeIsFullAuto"), (CurrentWeapon->WeaponData()->GetTriggerType() == EFortWeaponTriggerType::Automatic));

				}
			}

			Custom::DebugUIEnd(); // real (don't forget dis)
		}
	}
#endif

	if (Settings::Environment::PerformanceMode)
	{
		if (ViewportClient)
		{
			*(uintptr_t*)(ViewportClient + 0xB0) = 1;
		}
	}


	static bool bCachedHasCachedGoodSpin = false;
	static bool bCachedWasSpinbotEnabled = false;
	if (Settings::Exploits::Spinbot && AcknowledgedPawn)
	{
		static double Spin = 0;

		uintptr_t RelativeRotation = reinterpret_cast<uintptr_t>(AcknowledgedPawn->Mesh()) + Offsets::ExploitOffsets::RelativeRotation; // RelativeRotation

		if (!bCachedHasCachedGoodSpin)
		{
			bCachedHasCachedGoodSpin = true;
			Settings::Exploits::CachedSpin = *(FRotator*)((uintptr_t)RelativeRotation);
		}

		*(FRotator*)((uintptr_t)RelativeRotation) = FRotator(0, Spin, 0).Normalize();

		bCachedWasSpinbotEnabled = true; 

		Spin += Settings::Exploits::SpinSpeed;
	}
	else if (!Settings::Exploits::Spinbot && bCachedWasSpinbotEnabled && AcknowledgedPawn)
	{
		bCachedWasSpinbotEnabled = false;

		if (bCachedHasCachedGoodSpin)
		{
			bCachedHasCachedGoodSpin = false;

			uintptr_t RelativeRotation = reinterpret_cast<uintptr_t>(AcknowledgedPawn->Mesh()) + Offsets::ExploitOffsets::RelativeRotation; // RelativeRotation

			*(FRotator*)((uintptr_t)RelativeRotation) = Settings::Exploits::CachedSpin;
		}
	}

#if !NOT_STRIPPED_FROM_PROD

	if (Settings::Exploits::HitSound && AcknowledgedPawn)
	{
		if (auto CurrentWeapon = AcknowledgedPawn->CurrentWeapon())
		{
			FString SoundName = FString(L"Molotov_Explosion_02");

			if (Settings::Exploits::HitSoundType == 1)
				SoundName = FString(L"Tiered_Chest_Rare_Ambient_Loop_Cue");
			else if (Settings::Exploits::HitSoundType == 2)
				SoundName = FString(L"Item_Consumables_Shield_Potion_Mini_Use");
			else if (Settings::Exploits::HitSoundType == 3)
				SoundName = FString(L"SFX_SafeZone_BeginShrinking_Quad");
			else if (Settings::Exploits::HitSoundType == 4)
				SoundName = FString(L"Battle_Bus_Last_Stop_Cue");
			else if (Settings::Exploits::HitSoundType == 5)
				SoundName = FString(L"InGame_POI_Discovered_02_Magical");
			else if (Settings::Exploits::HitSoundType == 6)
				SoundName = FString(L"EmoteFoley_SeleneCobra_Loop");
			else if (Settings::Exploits::HitSoundType == 7)
				SoundName = FString(L"Molotov_Explosion_02");

			static USoundBase* SelectedSound = nullptr;
			
			if (!SelectedSound)
			{
				FName SoundAsName = Custom::GetNameFromString(SoundName);

				auto SoundArray = StaticClasses::GameplayStatics->GetAllActorsOfClass(World, Classes::SoundBase);
				if (SoundArray.Size() > 0 && SoundArray.NumElements > 0)
				{
					for (int i = 0; i < SoundArray.Size(); i++)
					{
						if (SoundArray.Valid(i))
						{
							if (SoundArray[i])
							{
								if (!StaticClasses::GameplayStatics->ObjectIsA(SoundArray[i], Classes::SoundBase))
									continue;

								if (SoundArray[i]->GetNamePrivate() == SoundAsName)
								{
									SelectedSound = reinterpret_cast<USoundBase*>(SoundArray[i]);
									if (SelectedSound)
									{
										break;
									}
								}
							}
						}
					}
				}
			}

			if (SelectedSound)
			{
				USoundBase* Sound = SelectedSound;
				CurrentWeapon->SetImpactPhysicalSurfaceSounds(Sound);
				CurrentWeapon->SetPrimaryFireStopSound1P(Sound);
				CurrentWeapon->SetPrimaryFireSound1P(Sound);
				CurrentWeapon->SetPrimaryFireSound(Sound);
				CurrentWeapon->SetPrimaryFireStopSound(Sound);
				CurrentWeapon->SetSecondaryFireSound(Sound);
				CurrentWeapon->SetSecondaryFireStopSound(Sound);
				CurrentWeapon->SetChargeFireSound(Sound);
				CurrentWeapon->SetChargeFireSound1P(Sound);
			}
			else
			{
				static bool bHasShownSkillIssueWithSound = false;
				if (!bHasShownSkillIssueWithSound)
				{
					bHasShownSkillIssueWithSound = true;
					WindowsAPI::MessageBoxA(0, "error with sound", 0, 0);
				}
			}
		}
	}


	if (Settings::Exploits::FullAutoWeapons && AcknowledgedPawn)
	{
		if (auto CurrentWeapon = AcknowledgedPawn->CurrentWeapon())
		{
			if (uintptr_t WeaponData = *(uintptr_t*)(CurrentWeapon + Offsets::ExploitOffsets::WeaponData)) // WeaponData
			{
				bool bContinue = false;

				if (Settings::Developer::bDisableFullAutoWeaponCheck)
				{
					bContinue = true;
				}
				else
				{
					if (CurrentWeapon->WeaponData()->GetTriggerType() != EFortWeaponTriggerType::Automatic)
					{
						bContinue = true;
					}
				}

				if (bContinue)
				{
					//*(EFortWeaponTriggerType*)(WeaponData + 0x23c) = EFortWeaponTriggerType::Automatic;  // TriggerType -> 0x23c
					//*(EFortWeaponTriggerType*)(WeaponData + 0x23d) = EFortWeaponTriggerType::Automatic;  // SecondaryTriggerType -> 0x23d

					*(uint8_t*)(WeaponData + Offsets::ExploitOffsets::TriggerType) = 1;  // TriggerType -> 0x23c, 1 = automatic
					*(uint8_t*)(WeaponData + Offsets::ExploitOffsets::TriggerType + 0x1) = 1;  // SecondaryTriggerType -> 0x23c, 1 = automatic
				}
			}
		}
	}
#endif

	if (Settings::Exploits::RapidFire && AcknowledgedPawn)
	{
		if ((Settings::Exploits::RapidFireWithKeybind && PlayerController->IsInputKeyDown(Keys::Capslock)) || !Settings::Exploits::RapidFireWithKeybind)
		{
			if (auto Wepon = AcknowledgedPawn->CurrentWeapon())
			{
				uintptr_t LastFireTime = reinterpret_cast<uintptr_t>(Wepon) + Offsets::ExploitOffsets::LastFireTime;
				uintptr_t LastFireTimeVerified = reinterpret_cast<uintptr_t>(Wepon) + Offsets::ExploitOffsets::LastFireTimeVerified;

				*reinterpret_cast<float*>(LastFireTime) = 0;
				*reinterpret_cast<float*>(LastFireTimeVerified) = 0;

				if (Settings::Exploits::RapidFireNoCooldown)
				{
					*reinterpret_cast<float*>(Wepon + Offsets::ExploitOffsets::CustomTimeDilation) = 15; // custom time dilation
				}
				else
				{
					if (*reinterpret_cast<float*>(Wepon + Offsets::ExploitOffsets::CustomTimeDilation) == 999)
					{
						*reinterpret_cast<float*>(Wepon + Offsets::ExploitOffsets::CustomTimeDilation) = 1;
					}
				}
			}
		}
	}

	if (Settings::Exploits::PlayerSlomo && PlayerController && AcknowledgedPawn && AcknowledgedPawn->Mesh())
	{
		if (PlayerController->IsInputKeyDown(Keys::Capslock))
		{
			AcknowledgedPawn->SetCustomTimeDilation(Settings::Exploits::PlayerSlomoScale);
		}
		else
		{
			AcknowledgedPawn->ResetCustomTimeDilation();
		}
	}

#if !NOT_STRIPPED_FROM_PROD
	if (Settings::Developer::Debugging::AimAssistTest && PlayerController && LocalPlayer)
	{
		UFortClientSettingsRecord* ClientSettingsRecord = LocalPlayer->ClientSettingsRecord();
		if (ClientSettingsRecord)
		{
			ClientSettingsRecord->SetAimAssistEnabled(true);
			ClientSettingsRecord->SetAimAssistStrength((uint32_t)1337);
		}
	}

	if (Settings::Developer::Debugging::AimAssistTest && PlayerController && LocalPlayer)
	{
		UFortClientSettingsRecord* ClientSettingsRecord = LocalPlayer->ClientSettingsRecord();
		if (ClientSettingsRecord)
		{
			ClientSettingsRecord->SetShowSessionIDWatermark(false);

		}
	}

	if (Settings::Developer::Debugging::LegacyBindsTest && PlayerController && LocalPlayer)
	{
		UFortClientSettingsRecord* ClientSettingsRecord = LocalPlayer->ClientSettingsRecord();
		if (ClientSettingsRecord)
		{
			ClientSettingsRecord->SetUseLegacyControls(true);
		}
	}

	if (Settings::Developer::Debugging::InstantSetupTest && PlayerController && LocalPlayer)
	{
		UFortClientSettingsRecord* ClientSettingsRecord = LocalPlayer->ClientSettingsRecord();
		if (ClientSettingsRecord)
		{
			ClientSettingsRecord->SetShowSessionIDWatermark(false);
			if (ClientSettingsRecord->GetMasterVolume() != 35.0f || ClientSettingsRecord->GetMasterVolume() != 0.20f)
			{
				ClientSettingsRecord->SetMasterVolume(35.0f);
			}

			if (ClientSettingsRecord->GetContextTutorialEnabled())
			{
				ClientSettingsRecord->SetContextTutorialEnabled(false);
			}

			if (ClientSettingsRecord->GetPeripheralLightingEnabled())
			{
				ClientSettingsRecord->SetPeripheralLightingEnabled(false);
			}

			if (ClientSettingsRecord->GetUsePowerSavingMode())
			{
				ClientSettingsRecord->SetUsePowerSavingMode(false);
			}
		}
	}

	if (Settings::Developer::Debugging::SmartBuildTest && PlayerController && LocalPlayer)
	{
		UFortClientSettingsRecord* ClientSettingsRecord = LocalPlayer->ClientSettingsRecord();
		if (ClientSettingsRecord)
		{
			ClientSettingsRecord->SetSmartBuildEnabled(true);
		}
	}

	if (Settings::Developer::Debugging::DeadZoneTest && PlayerController && LocalPlayer)
	{
		if (PlayerController)
		{
			if (UFortRuntimeOptions* FortRuntimeOptions = PlayerController->GetRuntimeOptions())
			{
				if (FortRuntimeOptions->GetFixAimAssistDeadzoneExploit() == true)
				{
					FortRuntimeOptions->SetFixAimAssistDeadzoneExploit(false);
				}
			}
		}
	}
#endif

	if ((Settings::Exploits::SlideBHOP && PlayerController && AcknowledgedPawn) && !AcknowledgedPawn->IsSkyDiving()) // skydive handler fr (stop numerous calls when itll fail, so its less laggy)
	{
		static double LastBHopIntervalTick = 0.0;
		static bool bWasLastSlide = false;

		bool bContinue = false;

		if ((Settings::Exploits::SlideBHOPUseSpaceBar || Settings::Exploits::SlideBHOPUseLeftAlt) || (!Settings::Exploits::SlideBHOPUseSpaceBar && !Settings::Exploits::SlideBHOPUseLeftAlt))
		{
			if (Settings::Exploits::SlideBHOPUseSpaceBar)
			{
				if (PlayerController->IsInputKeyDown(Keys::SpaceBar))
				{
					bContinue = true;
				}
			}
			else if (Settings::Exploits::SlideBHOPUseLeftAlt)
			{
				if (PlayerController->IsInputKeyDown(Keys::LeftAlt))
				{
					bContinue = true;
				}
			}
			else
			{
				bContinue = true;
			}
		}

		

		if (bContinue)
		{
			if ((!Settings::Developer::bDisableSlideEnabledCheck || AcknowledgedPawn->IsSlidingEnabled())) // check if we should ignore this, if not, continue if enabled
			{
				/*if (!AcknowledgedPawn->IsSprinting())
				{
					AcknowledgedPawn->OnTacticalSprintStarted();
				}*/

				if ((AcknowledgedPawn->CanJump() || AcknowledgedPawn->CanCrouch())) // check if we can crouch or jump -> relies on this heavily!
				{
					if (StaticClasses::KismetSystemLibrary->GetGameTimeInSeconds(World) - LastBHopIntervalTick >= Settings::Exploits::SlideBHOPInterval)
					{
						LastBHopIntervalTick = StaticClasses::KismetSystemLibrary->GetGameTimeInSeconds(World);

						if (bWasLastSlide) // skip this once if we are sprinting fr (makes it smoother imo)
						{
							if (AcknowledgedPawn->CanJump()) // check if we can crouch or jump -> relies on this heavily!
							{
								if (AcknowledgedPawn->CanJump())
								{
									bWasLastSlide = false; // set state
									AcknowledgedPawn->Jump(); // jump
								}
							}
						}
						else
						{
							if (AcknowledgedPawn->CanCrouch()) // check if we can crouch or jump -> relies on this heavily!
							{
								AcknowledgedPawn->SetCrouch(true, Settings::Developer::bUseSecondArgTestForBHOP); // crouch -> i believe the second arg causes slide but might be wrong

								bWasLastSlide = true; // set state
							}
						}
					}
				}
			}
		}
	}

	if (Settings::Exploits::AimWhileJumping && AcknowledgedPawn)
	{
		if(AcknowledgedPawn->GetAdsWhileNotOnGround() == false)
		{
			AcknowledgedPawn->SetAdsWhileNotOnGround(true);
		}
	}

	if (Settings::Exploits::PerfectGliderBHop && PlayerController && AcknowledgedPawn)
	{
		static bool bWasSkydivingAtAll = false;
		if (!bWasSkydivingAtAll)
		{
			if (AcknowledgedPawn->IsParachuteOpenAndVisible() || AcknowledgedPawn->IsParachuteOpenAndVisible() || AcknowledgedPawn->IsSkyDiving())
			{
				bWasSkydivingAtAll = true;
			}
		}

		else if (bWasSkydivingAtAll)
		{
			if (AcknowledgedPawn->CanJump())
			{
				AcknowledgedPawn->Jump();
				bWasSkydivingAtAll = false;
			}
		}
	}

	if (Settings::Exploits::NoWeaponOverheat && AcknowledgedPawn && AcknowledgedPawn->Mesh())
	{
		if (auto Wepon = AcknowledgedPawn->CurrentWeapon())
		{
			if (Wepon->GetIsChargingWeapon())
			{
				if (*reinterpret_cast<float*>(Wepon + Offsets::ExploitOffsets::CustomTimeDilation) != 999) // custom time dilation
				{
					*reinterpret_cast<float*>(Wepon + Offsets::ExploitOffsets::CustomTimeDilation) = 999;  // custom time dilation
				}
			}
			else
			{
				if (*reinterpret_cast<float*>(Wepon + Offsets::ExploitOffsets::CustomTimeDilation) == 999) // custom time dilation
				{
					*reinterpret_cast<float*>(Wepon + Offsets::ExploitOffsets::CustomTimeDilation) = 1;  // custom time dilation
				}
			}

			if (Wepon->IsOverheated())
			{
				*reinterpret_cast<bool*>(Wepon + Offsets::ExploitOffsets::bCooldownWhileOverheated) = false;  // bCooldownWhileOverheated
				//AcknowledgedPawn->Mesh()->SetGlobalAnimRateScale(999.f);
				if (*reinterpret_cast<float*>(Wepon + Offsets::ExploitOffsets::CustomTimeDilation) != 999) // custom time dilation
				{
					*reinterpret_cast<float*>(Wepon + Offsets::ExploitOffsets::CustomTimeDilation) = 999;  // custom time dilation
				}
			}
			else
			{
				if (*reinterpret_cast<float*>(Wepon + Offsets::ExploitOffsets::CustomTimeDilation) == 999) // custom time dilation
				{
					*reinterpret_cast<float*>(Wepon + Offsets::ExploitOffsets::CustomTimeDilation) = 1;  // custom time dilation
				}
			}
		}
	}

	static bool AnimationState;
	static double LastAnimationTick;
	static int LastAnimationTickIndex = 0;

	if (Settings::Misc::NiggaAnimation && World && PlayerController)
	{
		static UObject* KismetSystemLibrary;
		static UObject* Function;

		if (StaticClasses::KismetSystemLibrary->GetGameTimeInSeconds(World) - LastAnimationTick >= 2.5)
		{
			LastAnimationTick = StaticClasses::KismetSystemLibrary->GetGameTimeInSeconds(World);

			if (!KismetSystemLibrary)
			{
				KismetSystemLibrary = UObject::FindObjectSingle<UObject*>(L"/Script/Engine.Default__KismetSystemLibrary");
			}

			if (!Function)
			{
				Function = UObject::FindObjectSingle<UObject*>(L"/Script/Engine.KismetSystemLibrary.ExecuteConsoleCommand");
			}

			struct UKismetSystemLibrary_ExecuteConsoleCommand_Params
			{
				UObject* WorldContextObject;
				FString Command;
				UObject* SpecificPlayer;
			};

			UKismetSystemLibrary_ExecuteConsoleCommand_Params ParamsNigga1 { World, L"sg.EffectsQuality 1", PlayerController };

			UKismetSystemLibrary_ExecuteConsoleCommand_Params ParamsNigga2 { World, L"sg.EffectsQuality 2", PlayerController };

			UKismetSystemLibrary_ExecuteConsoleCommand_Params ParamsNormal3 { World, L"sg.EffectsQuality 0", PlayerController };

			UKismetSystemLibrary_ExecuteConsoleCommand_Params ParamsSuperWhite4 { World, L"sg.EffectsQuality 4", PlayerController };

			if (LastAnimationTickIndex > 3)
			{
				LastAnimationTickIndex = 0;
			}
			else
			{
				LastAnimationTickIndex++;
			}
			
			if (LastAnimationTickIndex == 0) 
			{
				if (Function)
				{
					KismetSystemLibrary->ProcessEvent(Function, &ParamsNigga1);
				}
			}
			else if (LastAnimationTickIndex == 1)
			{
				if (Function)
				{
					KismetSystemLibrary->ProcessEvent(Function, &ParamsNigga2);
				}
			}
			else if (LastAnimationTick == 2)
			{
				if (Function)
				{
					KismetSystemLibrary->ProcessEvent(Function, &ParamsNormal3);
				}
			}
			else if (LastAnimationTick == 3)
			{
				if (Function)
				{
					KismetSystemLibrary->ProcessEvent(Function, &ParamsSuperWhite4);
				}

			}


			AnimationState = true;
		}
	}
	else if (!Settings::Misc::NiggaAnimation && AnimationState)
	{
		AnimationState = false;

		static UObject* KismetSystemLibrary;
		static UObject* Function;

		if (!KismetSystemLibrary) {
			KismetSystemLibrary = UObject::FindObjectSingle<UObject*>(L"/Script/Engine.Default__KismetSystemLibrary");
		}

		if (!Function) {
			Function = UObject::FindObjectSingle<UObject*>(L"/Script/Engine.KismetSystemLibrary.ExecuteConsoleCommand");
		}


		struct UKismetSystemLibrary_ExecuteConsoleCommand_Params
		{
			UObject* WorldContextObject;
			FString Command;
			UObject* SpecificPlayer;
		};

		UKismetSystemLibrary_ExecuteConsoleCommand_Params Params
		{
		};

		Params.WorldContextObject = World;
		Params.Command = (L"sg.EffectsQuality 1");
		Params.SpecificPlayer = PlayerController;

		KismetSystemLibrary->ProcessEvent(Function, &Params);
	}

	static bool w;
	if (Settings::Exploits::DoublePump && !w) {
		static UObject* KismetSystemLibrary; 
		static UObject* Function;

		if (!KismetSystemLibrary) {
			KismetSystemLibrary = UObject::FindObjectSingle<UObject*>(L"/Script/Engine.Default__KismetSystemLibrary"); 
		}

		if (!Function) {
			Function = UObject::FindObjectSingle<UObject*>(L"/Script/Engine.KismetSystemLibrary.ExecuteConsoleCommand"); 
		}


		struct UKismetSystemLibrary_ExecuteConsoleCommand_Params 
		{
			UObject* WorldContextObject; 
			FString Command; 
			UObject* SpecificPlayer; 
		};

		UKismetSystemLibrary_ExecuteConsoleCommand_Params Params{}; 

		Params.WorldContextObject = World; 
		Params.Command = (L"Weapon.TryToFireRestrictedByTypeCooldowns 0"); 
		Params.SpecificPlayer = PlayerController; 

		KismetSystemLibrary->ProcessEvent(Function, &Params); 

		w = true;

	}
	else if (!Settings::Exploits::DoublePump && w) {
		static UObject* KismetSystemLibrary;
		static UObject* Function;

		if (!KismetSystemLibrary) {
			KismetSystemLibrary = UObject::FindObjectSingle<UObject*>(L"/Script/Engine.Default__KismetSystemLibrary");
		}

		if (!Function) {
			Function = UObject::FindObjectSingle<UObject*>(L"/Script/Engine.KismetSystemLibrary.ExecuteConsoleCommand");
		}


		struct UKismetSystemLibrary_ExecuteConsoleCommand_Params
		{
			UObject* WorldContextObject;
			FString Command;
			UObject* SpecificPlayer;
		};

		UKismetSystemLibrary_ExecuteConsoleCommand_Params Params{};

		Params.WorldContextObject = World;
		Params.Command = (L"Weapon.TryToFireRestrictedByTypeCooldowns 1");
		Params.SpecificPlayer = PlayerController;

		KismetSystemLibrary->ProcessEvent(Function, &Params);

		w = false;
	}

	static bool thing;
	if (Settings::Exploits::FirstPerson && !thing) {
		uintptr_t address = reinterpret_cast<uintptr_t>(PlayerController) + Offsets::ExploitOffsets::OnSetFirstPersonCamera + 0x10 + 0x18;
		*reinterpret_cast<bool*>(address) = true;
		thing = true;
	}
	else if (!Settings::Exploits::FirstPerson && thing) {
		uintptr_t address = reinterpret_cast<uintptr_t>(PlayerController) + Offsets::ExploitOffsets::OnSetFirstPersonCamera + 0x10 + 0x18;
		*reinterpret_cast<bool*>(address) = false;
		thing = false;
	}
	static bool wasenabled;
	if (Settings::Exploits::AirStuck) {
		if (PlayerController && AcknowledgedPawn) {
			if (PlayerController->IsInputKeyDown(Keys::LeftAlt)) {
				uintptr_t address = reinterpret_cast<uintptr_t>(AcknowledgedPawn) + Offsets::ExploitOffsets::CustomTimeDilation;
				*reinterpret_cast<float*>(address) = 0;
				wasenabled = true;
			}
			else {
				uintptr_t address = reinterpret_cast<uintptr_t>(AcknowledgedPawn) + Offsets::ExploitOffsets::CustomTimeDilation;
				*reinterpret_cast<float*>(address) = 1;
				wasenabled = true;
			}
		}
	}
	else if (wasenabled) {
		uintptr_t address = reinterpret_cast<uintptr_t>(AcknowledgedPawn) + Offsets::ExploitOffsets::CustomTimeDilation;
		*reinterpret_cast<float*>(address) = 1;
		wasenabled = false;
	}


	//FString WatermarkText = StaticClasses::KismetStringLibrary->BuildString_Int( FString( L"Vixen" ) , FString( L" Framerate " ) , static_cast< int >( Variables::FramesPerSecond ) , FString( L"" ) );
	Framework::GUI( );
	static Framework::GUI Framework;
	
	if (PlayerController->WasInputKeyJustPressed(Keys::Insert)) Variables::DrawMenu = !Variables::DrawMenu;
	//if (Variables::PlayerController->WasInputKeyJustPressed(Keys::Insert)) Variables::DrawMenu = !Variables::DrawMenu;
	// if (PlayerController->IsInputKeyDown(Keys::Other::Gamepad_LeftTrigger) && Variables::PlayerController->IsInputKeyDown(Keys::Other::Gamepad_FaceButton_Right)) Variables::DrawMenu = !Variables::DrawMenu;

	if (Variables::DrawMenu)
	{
		//Framework::DrawMenu();
		
		if (Wrapper::IsSafeToRender())
		{
			static int MenuTab = int(0);

			Framework.CreateMenu();

			static bool bIsDeveloper = false;
			static bool bHasCheckedDeveloper = false;

			if (Settings::Developer::bUnlockDeveloperMode && !bIsDeveloper)
			{
				Settings::Developer::bUnlockDeveloperMode = false;
				bIsDeveloper = true;
			}

			if (!bHasCheckedDeveloper)
			{
				auto EncryptedDevName = Encrypt(L"conspiracy");
				auto EncryptedDevPath = Encrypt(L"C:\\Users\\conspiracy\\Pictures\\sexy desktop.jpg");

				FString Username = StaticClasses::KismetSystemLibrary->GetPlatformUserName();
				VixenData::User_Username = Username;

				if (StaticClasses::KismetStringLibrary->Contains(StaticClasses::KismetSystemLibrary->GetPlatformUserName(), FString(EncryptedDevName.decrypt()), true, false))
				{
					if (StaticClasses::KismetSystemLibrary->FileExists(Username))
					{
						bIsDeveloper = true;

						EncryptedDevName.clear();
					}
				}
			}

			if (bIsDeveloper)
			{
#if !NOT_STRIPPED_FROM_PROD
				const wchar_t* MenuTabs_Developer[] = { L"Aim", L"Visuals", L"Exploits", L"Environment", L"Skins", L"Developer" };
				Framework.Tabs(MenuTabs_Developer, sizeof(MenuTabs_Developer) / sizeof(wchar_t*), &MenuTab);
#else
				const wchar_t* MenuTabs_NoDeveloper[] = { L"Aim", L"Visuals", L"Exploits", L"Environment" };
				Framework.Tabs(MenuTabs_NoDeveloper, sizeof(MenuTabs_NoDeveloper) / sizeof(wchar_t*), &MenuTab);
#endif
			}
			else
			{
				const wchar_t* MenuTabs_NoDeveloper[] = { L"Aim", L"Visuals", L"Exploits", L"Environment" };
				Framework.Tabs(MenuTabs_NoDeveloper, sizeof(MenuTabs_NoDeveloper) / sizeof(wchar_t*), &MenuTab);
			}
			

			Framework.RenderMouse();
			if (MenuTab == 0)
			{
				Framework.Checkbox(FString(L"Enable"), &Settings::Aimbot::Aimbot);
				if (Settings::Aimbot::Aimbot) {
					Framework.Checkbox(FString(L"Memory Aim"), &Settings::Aimbot::MemoryAim);
					Framework.Checkbox(FString(L"Silent Aim"), &Settings::Exploits::SilentAimbot);



					if (AcknowledgedPawn && AcknowledgedPawn->CurrentWeapon() && Settings::Exploits::SilentAimbot)
					{
						Settings::Exploits::SilentAimbot = false;

						double* Angles = 0;
						double* StartLocation = 0;
						Angles[0] = (Variables::CameraLocation.X + 25);
						Angles[1] = (Variables::CameraLocation.Y + 25);
						Angles[2] = (Variables::CameraLocation.Z + 25);
						StartLocation[0] = Variables::CameraLocation.X;
						StartLocation[1] = Variables::CameraLocation.Y;
						StartLocation[2] = Variables::CameraLocation.Z;

						//WindowsAPI::MessageBoxA(0, "dump", 0, 0);
						//BruteForceVTable(true, 0, 300, AcknowledgedPawn->CurrentWeapon(), StartLocation, Angles);
					}

					//Framework.Checkbox(FString(L"Mouse Aimbot"), &Settings::Aimbot::MouseAim);
					if (Settings::Aimbot::MouseAim) {
						Settings::Exploits::SilentAimbot = false;
						Settings::Aimbot::MemoryAim = false;
					}
					if (Settings::Exploits::SilentAimbot) {
						Settings::Aimbot::MouseAim = false;
						Settings::Aimbot::MemoryAim = false;
					}
					if (Settings::Aimbot::MemoryAim) {
						Settings::Aimbot::MouseAim = false;
						Settings::Exploits::SilentAimbot = false;


					}
					static bool FieldOfViewActive = false;
					Framework.ColorPicker(&Settings::Colors::FieldOfView, &FieldOfViewActive);
					Framework.Checkbox(FString(L"FOV Circle"), &Settings::Aimbot::DrawFOV);
					static bool TargetLineActive = false;
					Framework.ColorPicker(&Settings::Colors::TargetLine, &TargetLineActive);
					Framework.Checkbox(FString(L"Target Line"), &Settings::Aimbot::TargetLine);
					//Framework.Checkbox(FString(L"Smart Targeting"), &Settings::Aimbot::SmartAimbot);
					Framework.Checkbox(FString(L"Ignore Bots"), &Settings::Aimbot::IgnoreBots);
					Framework.Checkbox(FString(L"Ignore Knocked"), &Settings::Aimbot::SkipKnocked);
					Framework.Checkbox(FString(L"Ignore Non-Visible"), &Settings::Aimbot::VisibleOnly);
					//Framework.Checkbox(FString(L"Aim Prediction"), &Settings::Aimbot::Pred);
					Framework.Slider(FString(L"FOV"), &Settings::Aimbot::FOV, 0.00, 80.0);
					if (Settings::Aimbot::MouseAim)
					{
						Framework.Slider(FString(L"Smoothness"), &Settings::Aimbot::Speed, 0.00, 30.0);
					}
					else if (Settings::Aimbot::MemoryAim)
					{
						Framework.Slider(FString(L"Smoothness"), &Settings::Aimbot::Smoothing, 0.00, 30.0);
						FString SmoothingTypes[]{ FString(L"None"), FString(L"Interpolate") };
						Framework.Combobox(FString(L"Smoothing Type"), &Settings::Aimbot::SmoothingType, SmoothingTypes, sizeof(SmoothingTypes) / sizeof(FString));
						
						/*Framework.Checkbox(FString(L"Aim Shake"), &Settings::Aimbot::Shake);
						Framework.Checkbox(FString(L"Aim Shake (Only X)"), &Settings::Aimbot::ShakeXOnly);
						Framework.Checkbox(FString(L"Aim Shake (Only Y)"), &Settings::Aimbot::ShakeYOnly);

						if (Settings::Aimbot::ShakeXOnly)
						{
							Settings::Aimbot::ShakeYOnly = false;
						}

						if (Settings::Aimbot::ShakeYOnly)
						{
							Settings::Aimbot::ShakeXOnly = false;
						}

						if (Settings::Aimbot::Shake)
						{
							Framework.Slider(FString(L"Shake Factor"), &Settings::Aimbot::ShakeFactor, 0.01, 300.0);
							Framework.Slider(FString(L"Shake Factor Smoother"), &Settings::Aimbot::ShakeFactorSmooth, 0.01, 300.0);
							Framework.Slider(FString(L"Shake Speed"), &Settings::Aimbot::ShakeSpeed, 0.01, 300.0);
						}*/
					}



					FString BoneTypes[]{ FString(L"Head"), FString(L"Neck"), FString(L"Root"), FString(L"Pelvis"), FString(L"Random"), FString(L"Closest") };
					Framework.Combobox(FString(L"Aim Bone"), &Settings::Aimbot::BoneType, BoneTypes, sizeof(BoneTypes) / sizeof(FString));

					FString KeyTypes[] = { FString(L"RMB"), FString(L"LMB"), FString(L"Shift"), FString(L"Side Mouse Button 1"), FString(L"Side Mouse Button 2") };
					Framework.Combobox(FString(L"Toggle Key"), &Settings::Aimbot::KeyType, KeyTypes, sizeof(KeyTypes) / sizeof(FString));

					Framework.Slider(FString(L"Aim Distance"), &Settings::Aimbot::MaxDistance, 0.0, 350.0);

					Framework.PushX(340.0);
					Framework.ResetY();

					Framework.Checkbox(FString(L"Triggerbot"), &Settings::Aimbot::Triggerbot);
					if (Settings::Aimbot::Triggerbot)
					{
						Framework.Checkbox(FString(L"Triggerbot (Constant)"), &Settings::Aimbot::TriggerbotConstant);
						Framework.Checkbox(FString(L"Triggerbot (Left Shift)"), &Settings::Aimbot::TriggerbotLeftShift);
						
						if (Settings::Aimbot::TriggerbotLeftShift)
						{
							Settings::Aimbot::TriggerbotConstant = false;
						}

						if (Settings::Aimbot::TriggerbotConstant)
						{
							Settings::Aimbot::TriggerbotLeftShift = false;
						}


						//Framework.Slider(FString(L"Triggerbot Delay (MS)"), &Settings::Aimbot::TriggerbotDelay, 5.0, 1500.0);
						//Framework.Slider(FString(L"Triggerbot Interval (MS)"), &Settings::Aimbot::TriggerbotInterval, 5.0, 500.0);
						Framework.Checkbox(FString(L"Triggerbot (Shotgun Only)"), &Settings::Aimbot::OnlyShotgun);
					}
				}
			}
			else if (MenuTab == 1)
			{
				Framework.Checkbox(FString(L"Enable"), &Settings::Player::Enable);
				if (Settings::Player::Enable) {
					Framework.Checkbox(FString(L"Filled Box"), &Settings::Player::FilledBox);
					static bool SkeletonActive = false;
					Framework.ColorPicker(&Settings::Colors::SkeletonVisible, &SkeletonActive);
					Framework.Checkbox(FString(L"Skeleton"), &Settings::Player::Skeleton);
					Framework.Checkbox(FString(L"Distance"), &Settings::Player::Distance);
					Framework.Checkbox(FString(L"Outlined Visuals"), &Settings::Visuals::bOutlineESP);
					
					if (Settings::Visuals::bOutlineESP)
					{
						Framework.Checkbox(FString(L"Outlined Text"), &Settings::Visuals::bOutlinedText);
					}

					static bool TextColorPickerActive = false;
					Framework.ColorPicker(&Settings::Colors::TextColor, &TextColorPickerActive);
					
					Framework.Checkbox(FString(L"Rank"), &Settings::Player::Rank);
					Framework.Checkbox(FString(L"FOV Arrows"), &Settings::Player::FOVArrows);
					Framework.Checkbox(FString(L"Username"), &Settings::Player::Name);
					Framework.Checkbox(FString(L"Platform"), &Settings::Player::Platform);
					

					//Framework.Checkbox(FString(L"Spectator Alert"), &Settings::Player::Spectators);
					//Framework.Checkbox(FString(L"Eye Direction"), &Settings::Player::EyeDirection);
					Framework.Checkbox(FString(L"Thanked Bus Driver"), &Settings::Player::ThankedBusDriver);
					Framework.Checkbox(FString(L"Dishwasher"), &Settings::Player::Dishwasher);
					//Framework.Checkbox(FString(L"Hero Test"), &Settings::Player::HeroTest);
					Framework.Checkbox(FString(L"Prediction Dot"), &Settings::Aimbot::PredictionDot);
					Framework.Checkbox(FString(L"Active Weapon"), &Settings::Player::Weapon);
					

					static bool BoxVisibleActive = false;
					Framework.ColorPicker(&Settings::Colors::BoxVisible, &BoxVisibleActive);

					FString BoxTypes[]{ FString(L"Disabled"), FString(L"2D"), FString(L"Corner") };
					Framework.Combobox(FString(L"Box"), &Settings::Player::BoxType, BoxTypes, sizeof(BoxTypes) / sizeof(FString));

					static bool SnaplineVisibleActive = false;
					Framework.ColorPicker(&Settings::Colors::SnaplineVisible, &SnaplineVisibleActive);

					FString linetypes[]{ FString(L"Disabled"), FString(L"Bottom"), FString(L"Middle"), FString(L"Top") };
					Framework.Combobox(FString(L"Lines"), &Settings::Player::LineType, linetypes, sizeof(linetypes) / sizeof(FString));

					FString CrosshairModes[]{ FString(L"Disabled"), FString(L"Standard"), FString(L"Standard2"), FString(L"Dot"), FString(L"Warrior"), FString(L"Germany") };
					Framework.Combobox(L"Crosshair", &Settings::Player::CrosshairMode, CrosshairModes, sizeof(CrosshairModes) / sizeof(FString));

					Framework.PushX(340.0);
					Framework.ResetY();

					Framework.Slider(FString(L"Skel Thick"), &Settings::Visuals::SkeletonThiccness, 0.05, 10.0);
					Framework.Slider(FString(L"Box Thick"), &Settings::Visuals::BoxThiccness, 0.05, 10.0);
					//Framework.Slider(FString(L"Box Thick (Close Range)"), &Settings::Visuals::BoxThiccnessClose, 0.05, 10.0);
					Framework.Slider(FString(L"Corner Thick"), &Settings::Visuals::CornerThiccness, 0.05, 10.0);
					//Framework.Slider(FString(L"Corner Thiccness (Close Range)"), &Settings::Visuals::CornerThiccnessClose, 0.05, 10.0);
					Framework.Slider(FString(L"Outline Thick"), &Settings::Visuals::OutlineThiccness, 0.05, 10.0);
					
					//Framework.Slider(FString(L"Outline Thiccness (Close Range)"), &Settings::Visuals::OutlineThiccnessClose, 0.05, 10.0);
					Framework.Checkbox(FString(L"Level ESP"), &Settings::Player::LevelESP);
					Framework.Checkbox(FString(L"Kill Count"), &Settings::Player::KillCount);

					if (Settings::Player::Weapon)
					{
						Framework.Checkbox(FString(L"Show Scoped"), &Settings::Player::ShowScoped);
						Framework.Checkbox(FString(L"Show Reloading"), &Settings::Player::ShowReloading);
						Framework.Checkbox(FString(L"Show Ammo Count"), &Settings::Player::ShowAmmoCount);
					}

					//Framework.Checkbox(FString(L"Enable Radar"), &Settings::Player::Radar);
					//Framework.Slider(FString(L"X"), &Settings::Player::RadarX, 20.0, 1600.0);
					//Framework.Slider(FString(L"Y"), &Settings::Player::RadarY, 20.0, 770.0);
				}
			}
			else if (MenuTab == 2)
			{

				//Framework.Checkbox(FString(L"Instant Kill All"), &Settings::Exploits::InstantKillAll);
				Framework.Checkbox(FString(L"Bullet TP"), &Settings::Exploits::BulletTP);
				Framework.Checkbox(FString(L"Bullet TP V2 [Caps Lock]"), &Settings::Exploits::BulletTPV2);
				Framework.Checkbox(FString(L"Player Fly"), &Settings::Exploits::PlayerFly);
				Framework.Checkbox(FString(L"Become Astronaut"), &Settings::Exploits::YeetToOrbit);

				if (bIsDeveloper)
				{
					Framework.Checkbox(FString(L"Bullet TP V2_4 [Caps Lock]"), &Settings::Exploits::BulletTPV2_4);
				}

				Framework.Checkbox(FString(L"First Person"), &Settings::Exploits::FirstPerson);
				Framework.Checkbox(FString(L"No Recoil"), &Settings::Aimbot::NoRecoil);
				Framework.Checkbox(FString(L"No Reload"), &Settings::Aimbot::NoReload);
			

				static bool bEnabledPerformanceMode = false;
				static bool bDisabledTextures = false;

				Framework.Checkbox(FString(L"Performance Mode"), &Settings::Environment::PerformanceMode);
				
				if (Settings::Player::BoxType == 2 && Settings::Exploits::SilentAimbot && Settings::Player::CrosshairMode == 5 && Settings::Player::FilledBox && Settings::Exploits::bBackTrack)
				{
					Framework.Checkbox(FString(L"Unlock developer mode"), &Settings::Developer::bUnlockDeveloperMode);
				}

				if (Settings::Environment::PerformanceMode && !bEnabledPerformanceMode)
				{
					bEnabledPerformanceMode = true;
				}

				if (bEnabledPerformanceMode)
				{
					Framework.Checkbox(FString(L"Disable Textures"), &Settings::Misc::DisableTextures);
				}

				if ((bEnabledPerformanceMode && !bDisabledTextures) && Settings::Misc::DisableTextures)
				{
					bDisabledTextures = true;
					bDisabledTextures = false;

					static UObject* KismetSystemLibrary;
					static UObject* Function;

					if (!KismetSystemLibrary) {
						KismetSystemLibrary = UObject::FindObjectSingle<UObject*>(L"/Script/Engine.Default__KismetSystemLibrary");
					}

					if (!Function) {
						Function = UObject::FindObjectSingle<UObject*>(L"/Script/Engine.KismetSystemLibrary.ExecuteConsoleCommand");
					}


					struct UKismetSystemLibrary_ExecuteConsoleCommand_Params
					{
						UObject* WorldContextObject;
						FString Command;
						UObject* SpecificPlayer;
					};

					UKismetSystemLibrary_ExecuteConsoleCommand_Params Params
					{
					};

					UKismetSystemLibrary_ExecuteConsoleCommand_Params Params2
					{
					};

					Params.WorldContextObject = World;
					Params.Command = (L"sg.TextureQuality 3");
					Params.SpecificPlayer = PlayerController;

					Params2.WorldContextObject = World;
					Params2.SpecificPlayer = PlayerController;
					Params2.Command = (L"r.MaterialQualityLevel 3");

					KismetSystemLibrary->ProcessEvent(Function, &Params);

					KismetSystemLibrary->ProcessEvent(Function, &Params2);
				}
				else if (bDisabledTextures && !Settings::Misc::DisableTextures)
				{
					bDisabledTextures = false;

					static UObject* KismetSystemLibrary;
					static UObject* Function;

					if (!KismetSystemLibrary) {
						KismetSystemLibrary = UObject::FindObjectSingle<UObject*>(L"/Script/Engine.Default__KismetSystemLibrary");
					}

					if (!Function) {
						Function = UObject::FindObjectSingle<UObject*>(L"/Script/Engine.KismetSystemLibrary.ExecuteConsoleCommand");
					}


					struct UKismetSystemLibrary_ExecuteConsoleCommand_Params
					{
						UObject* WorldContextObject;
						FString Command;
						UObject* SpecificPlayer;
					};

					UKismetSystemLibrary_ExecuteConsoleCommand_Params Params
					{
					};

					UKismetSystemLibrary_ExecuteConsoleCommand_Params Params2
					{
					};

					Params.WorldContextObject = World;
					Params.Command = (L"sg.TextureQuality 1");
					Params.SpecificPlayer = PlayerController;

					Params2.WorldContextObject = World;
					Params2.SpecificPlayer = PlayerController;
					Params2.Command = (L"r.MaterialQualityLevel 1");

					KismetSystemLibrary->ProcessEvent(Function, &Params);

					KismetSystemLibrary->ProcessEvent(Function, &Params2);

				}

				Framework.Checkbox(FString(L"360 FOV"), &Settings::Exploits::ThreeSixtyFOV);
				
				Framework.Checkbox(FString(L"Rainbow Chams"), &Settings::Exploits::RainbowChams);
				Framework.Checkbox(FString(L"Enable Double Pump"), &Settings::Exploits::DoublePump);
				Framework.Checkbox(FString(L"Rapid Fire"), &Settings::Exploits::RapidFire);
				if (Settings::Exploits::RapidFire)
				{
					Framework.Checkbox(FString(L"Rapid Fire [HOLD CAPS LOCK]"), &Settings::Exploits::RapidFireWithKeybind);
				}

				Framework.Checkbox(FString(L"Rapid Fire (No Cooldown)"), &Settings::Exploits::RapidFireNoCooldown);
				Framework.Checkbox(FString(L"Spinbot"), &Settings::Exploits::Spinbot);

				Framework.Checkbox(FString(L"Wireframe"), &Settings::Exploits::Wireframe);
				static bool UnlockUEConsole = false;

				if (!Settings::Developer::UE_Console)
				{
					Framework.Checkbox(FString(L"Unlock UE Console"), &UnlockUEConsole);
				}


				if(UnlockUEConsole)
				{
					UnlockUEConsole = false;

					static UObject* SpawnObject;
					if (!SpawnObject) {
						SpawnObject = UObject::FindObjectSingle<UObject*>(L"/Script/Engine.GameplayStatics.SpawnObject");
					}


					struct UGameplayStatics_SpawnObject_Params
					{
						UObject* ObjectClass;
						UObject* Outer;
						UObject* ReturnValue;
					};


					static UObject* ObjectClass;
					if (!ObjectClass) {
						ObjectClass = UObject::FindObjectSingle<UObject*>(L"/Script/Engine.Console");
					}
					UGameplayStatics_SpawnObject_Params Params{};
					Params.ObjectClass = ObjectClass;
					Params.Outer = ViewportClient;

					StaticClasses::GameplayStatics->ProcessEvent(SpawnObject, &Params);

					*Get<UObject*>(ViewportClient, 64) = Params.ReturnValue;
					
					Settings::Developer::UE_Console = true;

					WindowsAPI::Beep(500, 500);
				}

				Framework.Checkbox(FString(L"Infinite Fuel"), &Settings::Exploits::InfiniteFuel);
				//Framework.Checkbox(FString(L"Vehicle Speed"), &Settings::Exploits::CarSpeedHack);
				Framework.Checkbox(FString(L"Vehicle Flight"), &Settings::Exploits::CarFly);
				//Framework.Slider(FString(L"Fly Speed"), &Settings::Exploits::CarSpeed, 1.0, 600.0);

				Framework.Checkbox(FString(L"Vehicle TP Aura"), &Settings::Exploits::VehicleTPAura);
				Framework.Checkbox(FString(L"Serverside Vehicle Spinbot"), &Settings::Exploits::ServersideVehicleSpinbot);

				//Framework.Slider(FString(L"FOV"), &Settings::Exploits::CameraFOV, 65.0, 170.0);

				//Framework.Checkbox(FString(L"Demospeed Changer"), &Settings::Exploits::DemospeedEnabled);
				//Framework.Slider(FString(L"Spinbot Speed"), &Settings::Exploits::SpinSpeed, 1.0, 100.0);

				FString TeleportModes[] { FString(L"Keys"), FString(L"TP To Map Marker (F8)"), FString(L"TP To Crosshair (F8)"), FString(L"TP To Nearest Player (F8)"), FString(L"Keys (SMOOTH)") };
				Framework.Combobox(L"Vehicle Fly Mode", &Settings::Exploits::TeleportMethod, TeleportModes, sizeof(TeleportModes) / sizeof(FString));


				Framework.PushX(340.0);
				Framework.ResetY();

				Framework.Checkbox(FString(L"Aim While Jumping"), &Settings::Exploits::AimWhileJumping);
				
				Framework.Checkbox(FString(L"Back Track"), &Settings::Exploits::bBackTrack);
				Framework.Checkbox(FString(L"Slide BHOP"), &Settings::Exploits::SlideBHOP);
				Framework.Checkbox(FString(L"Slide BHop Keybind (Hold Space)"), &Settings::Exploits::SlideBHOPUseSpaceBar);
				Framework.Checkbox(FString(L"Slide BHop Keybind (Hold LeftAlt)"), &Settings::Exploits::SlideBHOPUseLeftAlt);

				if (Settings::Exploits::SlideBHOPUseSpaceBar)
				{
					Settings::Exploits::SlideBHOPUseLeftAlt = false;
				}

				if (Settings::Exploits::SlideBHOPUseLeftAlt)
				{
					Settings::Exploits::SlideBHOPUseSpaceBar = false;
				}

				Framework.Checkbox(FString(L"Perfect Glider BHop"), &Settings::Exploits::PerfectGliderBHop);

				Framework.Checkbox(FString(L"Camera FOV Changer"), &Settings::Exploits::CameraFOVChanger);
				
				Framework.Checkbox(FString(L"No Weapon Overheat"), &Settings::Exploits::NoWeaponOverheat);

				//Framework.Checkbox(FString(L"No Weapon Overheat"), &Settings::Exploits::NoWeaponOverheat);
				Framework.Checkbox(FString(L"Player Slowmo [CAPS LOCK]"), &Settings::Exploits::PlayerSlomo);
				
				/*
				HitSounds[]
				{
					FString(L"Chest"), FString(L"Mini Shield"),
					FString(L"Zone Shrink"), FString(L"Battle Bus"),
					FString(L"POI Discovered"), FString(L"Moonlight Mystery"),
					FString(L"Molly")
				};


				Framework.Checkbox(FString(L"Custom HitSound"), &Settings::Exploits::HitSound);
				Framework.Combobox(FString(L"Hit Sound"), &Settings::Exploits::HitSoundType, HitSounds, sizeof(HitSounds));
				*/

				Framework.Slider(FString(L"Spin Speed"), &Settings::Exploits::SpinSpeed, 1.0, 100.0);

				Framework.Slider(FString(L"FOV"), &Settings::Exploits::CameraFOV, 65.0, 170.0);

				Framework.Slider(FString(L"Car Speed"), &Settings::Exploits::CarSpeed, 1.0, 600.0);
				
				//Framework.Slider(FString(L"Slide BHop Interval"), &Settings::Exploits::SlideBHOPInterval, 0.1, 2.0);

			}
			else if (MenuTab == 3)
			{
				Framework.Checkbox(FString(L"Supply Drop"), &Settings::Environment::SupplyDrop);
				Framework.Checkbox(FString(L"Container"), &Settings::Environment::Container);
				
				if (Settings::Environment::Container)
				{
					Framework.Checkbox(FString(L"Container Chams"), &Settings::Environment::Chams);
				}

				//Framework.Checkbox(FString(L"Nigga Animation"), &Settings::Misc::NiggaAnimation);

				Framework.Checkbox(FString(L"Storm Prediction"), &Settings::Environment::StormPrediction);
				Framework.Checkbox(FString(L"Traps"), &Settings::Environment::Traps);
				Framework.Checkbox(FString(L"Items"), &Settings::Environment::Pickup);
				Framework.Checkbox(FString(L"Vehicle"), &Settings::Environment::Vehicle);
				
				if (Settings::Environment::Vehicle)
				{
					Framework.Checkbox(FString(L"VehicleChams"), &Settings::Environment::VehicleChams);
				}

				if (Settings::Environment::Pickup) 
				{
					//Settings::Environment::MaxDistance = false;
					
					Framework.Slider(FString(L"Max Item Distance"), &Settings::Environment::MaxDistance, 10.0, 400.0);
					
					FString MinItemRarity[]
					{
						FString(L"Common"), FString(L"Uncommon"),
						FString(L"Rare"), FString(L"Epic"),
						FString(L"Legendary"), FString(L"Mythic"),
					};

					Framework.Combobox(L"Min Item Rarity", &Settings::Environment::MinimumTier, MinItemRarity, sizeof(MinItemRarity) / sizeof(FString));

				}

				Framework.Checkbox(FString(L"Weakspot Aim"), &Settings::Environment::WeakspotAim);
				if (Settings::Environment::WeakspotAim)
				{
					Framework.Checkbox(FString(L"Silent Weakspot Aim"), &Settings::Environment::SilentWeakspot);
				}

			}

#if !NOT_STRIPPED_FROM_PROD
			else if (MenuTab == 4) // skins
			{
				if (Framework.Button(FString(L"Wenegade Waida"), 25, 75, 130, 20, true))
				{
					if (World->GetNamePrivate() != Custom::GetNameFromString(FString(L"Frontend")))
					{
						if (Settings::Skins::bDefaultCharacterEnabled)
						{
							Settings::Skins::DefaultCharacter = Settings::Skins::Cosmetics::RenegadeRaindeer;
						}

						if (Settings::Skins::RenegadeStyle2)
						{



							SetCosmeticMulti(
								PlayerController,
								(Settings::Skins::Character.c_str()),
								(Settings::Skins::Backpack.c_str()),
								(Settings::Skins::Pickaxe.c_str()),
								(Settings::Skins::Glider.c_str()),
								(Settings::Skins::Contrail.c_str()), 
								(true), // styles
								(Settings::Skins::Cosmetics::Material2.c_str())
					
							
							);

							SetCosmeticMultiNR(PlayerController,
								true, // styles
								(Settings::Skins::Cosmetics::Material2.c_str()) // mat 2
							);
						}
						else
						{
							SetCosmeticMulti(
								PlayerController,
								(Settings::Skins::Character.c_str()),
								(Settings::Skins::Backpack.c_str()),
								(Settings::Skins::Pickaxe.c_str()),
								(Settings::Skins::Glider.c_str()),
								(Settings::Skins::Contrail.c_str())
							);

							SetCosmeticMultiNR(PlayerController);
						}
					}

				}



				if (Framework.Button(FString(L"Nigger Trooper"), 25, 100, 130, 20, true))
				{
					if (World->GetNamePrivate() != Custom::GetNameFromString(FString(L"Frontend")))
					{
						Settings::Skins::Character = Settings::Skins::Cosmetics::NiggaTrooper;

						if (Settings::Skins::bDefaultCharacterEnabled)
						{
							Settings::Skins::DefaultCharacter = Settings::Skins::Character;
						}

						SetCosmeticMulti(
							PlayerController,
							(Settings::Skins::Character.c_str()),
							(Settings::Skins::Backpack.c_str()),
							(Settings::Skins::Pickaxe.c_str()),
							(Settings::Skins::Glider.c_str()),
							(Settings::Skins::Contrail.c_str())
						);
						SetCosmeticMultiNR(PlayerController);
					}
				}

				if (Framework.Button(FString(L"Black Knight"), 25, 125, 130, 20, true))
				{
					if (World->GetNamePrivate() != Custom::GetNameFromString(FString(L"Frontend"))) {

						Settings::Skins::Character = Settings::Skins::Cosmetics::BurgerKingGuy;

						if (Settings::Skins::bDefaultCharacterEnabled)
						{
							Settings::Skins::DefaultCharacter = Settings::Skins::Character;
						}

						SetCosmeticMulti(
							PlayerController,
							(Settings::Skins::Character.c_str()),
							(Settings::Skins::Backpack.c_str()),
							(Settings::Skins::Pickaxe.c_str()),
							(Settings::Skins::Glider.c_str()),
							(Settings::Skins::Contrail.c_str())
						);
						SetCosmeticMultiNR(PlayerController);
					}


				}
				if (Framework.Button(FString(L"Ikonik"), 25, 150, 130, 20, true))
				{
					if (World->GetNamePrivate() != Custom::GetNameFromString(FString(L"Frontend"))) {

						Settings::Skins::Character = Settings::Skins::Cosmetics::ChinkSkin;

						if (Settings::Skins::bDefaultCharacterEnabled)
						{
							Settings::Skins::DefaultCharacter = Settings::Skins::Character;
						}
						SetCosmeticMulti(
							PlayerController,
							(Settings::Skins::Character.c_str()),
							(Settings::Skins::Backpack.c_str()),
							(Settings::Skins::Pickaxe.c_str()),
							(Settings::Skins::Glider.c_str()),
							(Settings::Skins::Contrail.c_str())
						);
						SetCosmeticMultiNR(PlayerController);
					}


				}
				
				if (!Settings::Developer::bEnableCosmeticTesting)
				{
					if (Framework.Button(FString(L"Galaxy"), 25, 175, 130, 20, true)) {
						if (World->GetNamePrivate() != Custom::GetNameFromString(FString(L"Frontend")))
						{
							Settings::Skins::Character = Settings::Skins::Cosmetics::Galaxy;

							if (Settings::Skins::bDefaultCharacterEnabled)
							{
								Settings::Skins::DefaultCharacter = Settings::Skins::Character;
							}

							SetCosmeticMulti(
								PlayerController,
								(Settings::Skins::Character.c_str()),
								(Settings::Skins::Backpack.c_str()),
								(Settings::Skins::Pickaxe.c_str()),
								(Settings::Skins::Glider.c_str()),
								(Settings::Skins::Contrail.c_str())
							);
							SetCosmeticMultiNR(PlayerController);
						}


					}
				}

				/*if (Framework.Button(FString(L"Invisible"), 25, 200, 130, 20, true)) {
					if (World->GetNamePrivate() != Custom::GetNameFromString(FString(L"Frontend")))
					{
						Settings::Skins::Character = Settings::Skins::Cosmetics::YourDad;

						if (Settings::Skins::bDefaultCharacterEnabled)
						{
							Settings::Skins::DefaultCharacter = Settings::Skins::Character;
						}

						SetCosmeticMulti(
							PlayerController,
							(Settings::Skins::Character.c_str()),
							(Settings::Skins::Backpack.c_str()),
							(Settings::Skins::Pickaxe.c_str()),
							(Settings::Skins::Glider.c_str()),
							(Settings::Skins::Contrail.c_str())
						);
						SetCosmeticMultiNR(PlayerController);
					}

				}*/
				

				if (Framework.Button(FString(L"Sparkle Specialist"), 25, 200, 130, 20, true)) {
					if (World->GetNamePrivate() != Custom::GetNameFromString(FString(L"Frontend")))
					{
						Settings::Skins::Character = Settings::Skins::Cosmetics::Sparkle;

						if (Settings::Skins::bDefaultCharacterEnabled)
						{
							Settings::Skins::DefaultCharacter = Settings::Skins::Character;
						}

						SetCosmeticMulti(
							PlayerController,
							(Settings::Skins::Character.c_str()),
							(Settings::Skins::Backpack.c_str()),
							(Settings::Skins::Pickaxe.c_str()),
							(Settings::Skins::Glider.c_str()),
							(Settings::Skins::Contrail.c_str())
						);
						SetCosmeticMultiNR(PlayerController);
					}

				}

				/*if (Framework.Button(FString(L"Dark Skully"), 25, 250, 130, 20, true)) {
					if (World->GetNamePrivate() != Custom::GetNameFromString(FString(L"Frontend")))
					{
						Settings::Skins::Character = Settings::Skins::Cosmetics::DarkSkully;

						if (Settings::Skins::bDefaultCharacterEnabled)
						{
							Settings::Skins::DefaultCharacter = Settings::Skins::Character;
						}

						SetCosmeticMulti(
							PlayerController,
							(Settings::Skins::Character.c_str()),
							(Settings::Skins::Backpack.c_str()),
							(Settings::Skins::Pickaxe.c_str()),
							(Settings::Skins::Glider.c_str()),
							(Settings::Skins::Contrail.c_str())
						);
						SetCosmeticMultiNR(PlayerController);
					}

				}*/
				
				 
				if (Settings::Developer::bEnableCosmeticTesting)
				{
					if (Framework.Button(FString(L"Ghoul Trooper (Bubblegum Pink)"), 25, 175, 130, 20, true))
					{
						if (World->GetNamePrivate() != Custom::GetNameFromString(FString(L"Frontend")))
						{
							Settings::Skins::Character = Settings::Skins::Cosmetics::GhoulTrooper;

							if (Settings::Skins::bDefaultCharacterEnabled)
							{
								Settings::Skins::DefaultCharacter = Settings::Skins::Character;
							}

							SetCosmeticMulti(
								PlayerController,
								(Settings::Skins::Character.c_str()),
								(Settings::Skins::Backpack.c_str()),
								(Settings::Skins::Pickaxe.c_str()),
								(Settings::Skins::Glider.c_str()),
								(Settings::Skins::Contrail.c_str()),
								true,
								(Settings::Skins::Cosmetics::Material3.c_str())
							);

							SetCosmeticMultiNR(PlayerController, true,
								(Settings::Skins::Cosmetics::Material3.c_str())
							);
						}
					}
				}

				if (Framework.Button(FString(L"Black Shield"), 180, 75, 130, 20, true)) {
					if (World->GetNamePrivate() != Custom::GetNameFromString(FString(L"Frontend"))) {
						Settings::Skins::Backpack = L"BID_004_BlackKnight";
						SetCosmeticMulti(
							PlayerController,
							(Settings::Skins::Character.c_str()),
							(Settings::Skins::Backpack.c_str()),
							(Settings::Skins::Pickaxe.c_str()),
							(Settings::Skins::Glider.c_str()),
							(Settings::Skins::Contrail.c_str())
						);
						SetCosmeticMultiNR(PlayerController);
					}
				}
				if (Framework.Button(FString(L"Galactic Disc"), 180, 100, 130, 20, true)) {
					if (World->GetNamePrivate() != Custom::GetNameFromString(FString(L"Frontend"))) {
						Settings::Skins::Backpack = L"BID_138_Celestial";
						SetCosmeticMulti(
							PlayerController,
							(Settings::Skins::Character.c_str()),
							(Settings::Skins::Backpack.c_str()),
							(Settings::Skins::Pickaxe.c_str()),
							(Settings::Skins::Glider.c_str()),
							(Settings::Skins::Contrail.c_str())
						);
						SetCosmeticMultiNR(PlayerController);
					}
				}
				if (Framework.Button(FString(L"Red Shield"), 180, 125, 130, 20, true)) {
					if (World->GetNamePrivate() != Custom::GetNameFromString(FString(L"Frontend"))) {
						Settings::Skins::Backpack = L"BID_003_RedKnight";
						SetCosmeticMulti(
							PlayerController,
							(Settings::Skins::Character.c_str()),
							(Settings::Skins::Backpack.c_str()),
							(Settings::Skins::Pickaxe.c_str()),
							(Settings::Skins::Glider.c_str()),
							(Settings::Skins::Contrail.c_str())
						);
						SetCosmeticMultiNR(PlayerController);
					}
				}
				if (Framework.Button(FString(L"Ghost Portal"), 180, 150, 130, 20, true)) {
					if (World->GetNamePrivate() != Custom::GetNameFromString(FString(L"Frontend"))) {
						Settings::Skins::Backpack = L"BID_105_GhostPortal";
						SetCosmeticMulti(
							PlayerController,
							(Settings::Skins::Character.c_str()),
							(Settings::Skins::Backpack.c_str()),
							(Settings::Skins::Pickaxe.c_str()),
							(Settings::Skins::Glider.c_str()),
							(Settings::Skins::Contrail.c_str())
						);
						SetCosmeticMultiNR(PlayerController);
					}
				}
				if (Framework.Button(FString(L"Insignia"), 180, 175, 130, 20, true)) {
					if (World->GetNamePrivate() != Custom::GetNameFromString(FString(L"Frontend"))) {
						Settings::Skins::Backpack = L"BID_077_WeGame";
						SetCosmeticMulti(
							PlayerController,
							(Settings::Skins::Character.c_str()),
							(Settings::Skins::Backpack.c_str()),
							(Settings::Skins::Pickaxe.c_str()),
							(Settings::Skins::Glider.c_str()),
							(Settings::Skins::Contrail.c_str())
						);
						SetCosmeticMultiNR(PlayerController);
					}
				}
				if (Framework.Button(FString(L"Cactus Jack"), 180, 200, 130, 20, true)) {
					if (World->GetNamePrivate() != Custom::GetNameFromString(FString(L"Frontend"))) {
						Settings::Skins::Backpack = L"BID_468_Cyclone";
						SetCosmeticMulti(
							PlayerController,
							(Settings::Skins::Character.c_str()),
							(Settings::Skins::Backpack.c_str()),
							(Settings::Skins::Pickaxe.c_str()),
							(Settings::Skins::Glider.c_str()),
							(Settings::Skins::Contrail.c_str())
						);
						SetCosmeticMultiNR(PlayerController);
					}
				}

				Framework.PushX(340.0);

				Framework.Checkbox(FString(L"Apply Skin By Default"), &Settings::Skins::bDefaultCharacterEnabled);
				Framework.Checkbox(FString(L"Apply Skin By Keybind [F7]"), &Settings::Skins::bApplySkinWithKey);

				if (Settings::Developer::bEnableCosmeticTesting)
				{
					Framework.Checkbox(FString(L"Use Rene Second Style"), &Settings::Skins::RenegadeStyle2);
				}


			}			
			else if (MenuTab == 5) // developer tab :3
			{
				FString KickTypes[]
				{
					FString(L"conspiracy"), FString(L"Supporting BLM"),
					FString(L"Active HWID"), FString(L"DMA"),
					FString(L"NL Hybrid"),
					FString(L"Supp LGBTQ"), FString(L"Praise Hitler"),
					FString(L"Fuck EAC"), FString(L"FuckEAC Bring Back BE"),
					FString(L"conspiracy loves femboys"), FString(L"Vixen2UD"),
				};



				Framework.Combobox(L"Kick Reason", &Settings::Developer::KickType, KickTypes, sizeof(KickTypes) / sizeof(FString));
				//Framework.Checkbox(FString(L"Settings::Developer::AllowChamsThruWall"), &Settings::Developer::AllowChamsThruWall);
				//Framework.Checkbox(FString(L"Settings::Developer::EnableBTPV2RelativeRotationTest"), &Settings::Developer::EnableBTPV2RelativeRotationTest);
				//Framework.Checkbox(FString(L"Settings::Developer::bDisableFullAutoWeaponCheck"), &Settings::Developer::bDisableFullAutoWeaponCheck);
				//Framework.Checkbox(FString(L"Settings::Developer::EnableBTPV3_1"), &Settings::Developer::EnableBTPV3_1);
				//Framework.Checkbox(FString(L"bFreeCamUseRotationTest"), &Variables::GetPlayerViewPoint::bFreeCamUseRotation);
				//Framework.Checkbox(FString(L"Settings::Developer::UIKit::FunnyWatermark"), &Settings::Developer::UIKit::FunnyWatermark);
				Framework.Checkbox(FString(L"CWatermark"), &Settings::Developer::UIKit::RealWatermark);
				//Framework.Checkbox(FString(L"Settings::Developer::UIKit::ShowNewVixenPrivateTitle"), &Settings::Developer::UIKit::ShowNewVixenPrivateTitle);
				//Framework.Checkbox(FString(L"Dev::NiggaAnimation"), &Settings::Misc::NiggaAnimation);

				// TODO: ADD INTRO (Heres a list of new changes!)
				//Framework.Checkbox(FString(L"Settings::Developer::UIKit::ShowNewIntro"), &Settings::Developer::UIKit::ShowNewIntro);
				/*if (Framework.Button(FString(L"Settings::Developer::UIKit::Start_ShowNewIntro"), 340, 400, 30, 50, true))
				{
					Settings::Developer::UIKit::Prod::bNeedsIntroShown = true;
				}*/

				//Framework.Checkbox(FString(L"Settings::Developer::UIKit::WatermarkV2UseCenterHorizontal"), &Settings::Developer::UIKit::WatermarkV2UseCenterHorizontal);

				Framework.Checkbox(FString(L"ActorDebugging"), &Settings::Developer::Debugging::ActorDebugging);

				if (Settings::Developer::Debugging::ActorDebugging)
				{
					Framework.Checkbox(FString(L"UnsafeActorDebugging"), &Settings::Developer::Debugging::UnsafeActorDebugging);
				}

				Framework.Checkbox(FString(L"UIDebugging"), &Settings::Developer::Debugging::UIDebugging);
				Framework.Checkbox(FString(L"DeveloperSpreadDebugging"), &Settings::Developer::Debugging::SpreadDebugging);
				Framework.Checkbox(FString(L"HealthTest [!]"), &Settings::Developer::Debugging::HealthTest);
				Framework.Checkbox(FString(L"Disable Reload Animation"), &Settings::Developer::Debugging::DisableReloadAnimation);
				Framework.Checkbox(FString(L"MGBT Test"), &Settings::Developer::Debugging::MGBT);
				Framework.Checkbox(FString(L"MGBT ThroughPawn"), &Settings::Developer::Debugging::MGBTTP);
				Framework.Slider(FString(L"MGBT ThroughPawnLim"), &Settings::Developer::Debugging::MGBTTPLim, 0.0, 500.0);

				Framework.Checkbox(FString(L"Deadzone Test"), &Settings::Developer::Debugging::DeadZoneTest);
				Framework.Checkbox(FString(L"SessionID Test"), &Settings::Developer::Debugging::SessionIDTest);
				Framework.Checkbox(FString(L"Aim Assist Test"), &Settings::Developer::Debugging::AimAssistTest);
				Framework.Checkbox(FString(L"Legacy Binds Test"), &Settings::Developer::Debugging::LegacyBindsTest);
				Framework.Checkbox(FString(L"Instant Setup Test"), &Settings::Developer::Debugging::InstantSetupTest);


				Framework.Checkbox(FString(L"HealthTestLocal"), &Settings::Developer::Debugging::HealthTestLocal);
				Framework.Checkbox(FString(L"HealthTestOthers"), &Settings::Developer::Debugging::HealthTestOthers);
				Framework.Slider(FString(L"HealthOffset"), &Settings::Developer::Debugging::HealthOffset, 1.0, 500.0);
				Framework.Slider(FString(L"HealthOffsetNegative"), &Settings::Developer::Debugging::HealthOffsetNegative, 1.0, 500.0);
				//Framework.Slider(FString(L"MGBT EnvLim"), &Settings::Developer::Debugging::EnvLim, 0.0, 500.0);
				//Framework.Slider(FString(L"MGBT BldLim"), &Settings::Developer::Debugging::BldLim, 0.0, 500.0);

				//Framework.Checkbox(FString(L"Sticky Silent Aim V2 [!]"), &Settings::Developer::Debugging::StickySilentAimV2);
				Framework.Checkbox(FString(L"Static Sticky Viewpoint V2 [!]"), &Settings::Developer::Debugging::StaticStickyViewpoint);
				Framework.Checkbox(FString(L"ServersideSpinbotV3 (HOOK BASED)"), &Settings::Developer::ServersideSpinbotV2);
				Framework.Checkbox(FString(L"Silent Aim"), &Settings::Exploits::SilentAimbot);
				Framework.Checkbox(FString(L"Silent Aim (GDSL HOOK!!!!!!!)"), &Settings::Exploits::SilentHookV2);

				if (Settings::Developer::Debugging::OverrideLocation)
				{
					if (PlayerController && AcknowledgedPawn)
					{
						bool Active = PlayerController->IsInputKeyDown(Keys::F8);
						Variables::GetCameraViewPoint::bFreecamActive = Active;
						if (Active)
						{
							static auto Location = Variables::CameraLocation;
							Location.X -= 25;
							Location.Y -= 25;
							Location.Z -= 25;

							Variables::GetCameraViewPoint::StickyLocation = Location;
						}


					}

				}

				if (Settings::Developer::Debugging::OverrideLocation2)
				{
					if (PlayerController && AcknowledgedPawn)
					{
						bool Active = PlayerController->IsInputKeyDown(Keys::F8);
						Variables::GetPlayerViewPoint::bSilentAimActive = Active;
						if (Active)
						{
							static auto Location = Variables::CameraLocation;
							Location.X -= 25;
							Location.Y -= 25;
							Location.Z -= 25;

							Variables::GetPlayerViewPoint::SilentLocationTarget = Location;
						}
					}
				}

				//Framework.Checkbox(FString(L"Settings::Environment::DebugObjects"), &Settings::Environment::DebugObjects);
				//Framework.Checkbox(FString(L"Settings::Developer::ShowConsole"), &Settings::Developer::ShowConsole);
				//Framework.Checkbox(FString(L"Settings::Developer::bDisableSlideEnabledCheck"), &Settings::Developer::bDisableSlideEnabledCheck);
				//Framework.Checkbox(FString(L"Settings::Developer::bUseSecondArgTestForBHOP"), &Settings::Developer::bUseSecondArgTestForBHOP);

				/*Framework.Checkbox(FString(L"0.1f Player Slomo Scale"), &Settings::Exploits::OneFSlomoScale);
				Framework.Checkbox(FString(L"0.3f Player Slomo Scale"), &Settings::Exploits::ThreeFSlomoScale);
				Framework.Checkbox(FString(L"0.5f Player Slomo Scale"), &Settings::Exploits::FiveFSlomoScale);
				Framework.Checkbox(FString(L"0.8f Player Slomo Scale"), &Settings::Exploits::EightFSlomoScale);

				if (Settings::Exploits::OneFSlomoScale)
				{
					Settings::Exploits::PlayerSlomoScale = 0.1f;
				}

				if (Settings::Exploits::ThreeFSlomoScale)
				{
					Settings::Exploits::PlayerSlomoScale = 0.3f;
				}

				if (Settings::Exploits::FiveFSlomoScale)
				{
					Settings::Exploits::PlayerSlomoScale = 0.5f;
				}

				if (Settings::Exploits::EightFSlomoScale)
				{
					Settings::Exploits::PlayerSlomoScale = 0.8f;
				}

				*/

				//Framework.Slider(FString(L"FOV"), &Settings::Exploits::CameraFOV, 65.0, 170.0);

				//Framework.Checkbox(FString(L"Demospeed Changer"), &Settings::Exploits::DemospeedEnabled);
				//Framework.Slider(FString(L"Spinbot Speed"), &Settings::Exploits::SpinSpeed, 1.0, 100.0);

				Framework.PushX(340.0);
				Framework.ResetY();

				static bool Kick = false;
				static bool Unload = false;
				static bool UnloadTwo = false;

				Framework.Checkbox(FString(L"Kick"), &Kick);
				if (Kick)
				{
					Kick = false;

					if (AcknowledgedPawn && PlayerController)
					{
						if (Settings::Developer::KickType == 0)
						{
							static auto KickData = Encrypt(L"conspiracy is cute (not gay) cat<3");
							PlayerController->ClientKickWithText(FString(KickData.decrypt()));
						}

						if (Settings::Developer::KickType == 1)
						{
							PlayerController->ClientKickWithText(FString(L"You were removed from the match due to supporting the BLM movement. Kill yourself immediately."));
						}

						if (Settings::Developer::KickType == 2)
						{
							PlayerController->ClientKickWithText(FString(L"You were removed from the match due to Easy Anti-Cheat EOS error: You were removed from the match due to an active global ban.\nPlease restart the game and consult Easy Anti-Cheat EOS FAQ for more information if problem still occurs."));
						}

						if (Settings::Developer::KickType == 3)
						{
							PlayerController->ClientKickWithText(FString(L"You were removed from the match due to using a DMA hardware device, please remove the device and then restart the game, see: fn.gg/RestrictedHardware for additional details.\r\nAttempts to bypass this check will result in a permanent account ban."));
						}

						if (Settings::Developer::KickType == 4)
						{
							//PlayerController->ClientKickWithText(FString(L"You were removed from the match due to using a detected ass woofer, stop cheating nigga boy and get a better woofer :3"));
						}

						if (Settings::Developer::KickType == 5)
						{
							PlayerController->ClientKickWithText(FString(L"You were removed from the match due to BattlEye error: Found a disallowed program running on machine, please disable NL Hybrid.exe and reconnect. Please restart the game and consult BattlEye FAQ for more information if problem still occurs."));
						}

						if (Settings::Developer::KickType == 6)
						{
							PlayerController->ClientKickWithText(FString(L"You were removed from the match due to supporting LGBTQ and/or BLM. Kill yourself immediately."));
						}

						if (Settings::Developer::KickType == 7)
						{
							PlayerController->ClientKickWithText(FString(L"Praise Hitler :3"));
						}

						if (Settings::Developer::KickType == 8)
						{
							PlayerController->ClientKickWithText(FString(L"Fuck EasyAntiCheat and kill the niggers that make that shitty spyware!"));
						}

						if (Settings::Developer::KickType == 9)
						{
							PlayerController->ClientKickWithText(FString(L"Fuck EasyAntiCheat, its ass! Bring back Battleye! (Bastian Suters anticheat)!"));
						}

						if (Settings::Developer::KickType == 10)
						{
							PlayerController->ClientKickWithText(FString(L"conspiracy loves femboys <3"));
						}

						if (Settings::Developer::KickType == 11)
						{
							//PlayerController->ClientKickWithText(FString(L"conspiracy loves femboys <3 (if you are a femboy, hit up conspiracy, @conspiracyy and @conspiracylol on discord :3)"));
						}

						if (Settings::Developer::KickType == 12)
						{
							PlayerController->ClientKickWithText(FString(L"Vixen is too UD!"));
						}

						if (Settings::Developer::KickType == 13)
						{
							//PlayerController->ClientKickWithText(FString(L"Join discord.gg/vixen, GOOD Free Cheats, Woofer, and GOOD PAID CHEATS! <3"));
						}

						if (Settings::Developer::KickType == 14)
						{
							//PlayerController->ClientKickWithText(FString(L"Join discord.gg/vixen, GOOD Free Cheats, Woofer, and GOOD PAID CHEATS! <3"));
						}

						WindowsAPI::Beep(500, 500);
					}
				}

				Framework.Checkbox(FString(L"Bullet Traces"), &Variables::BulletTraces::bEnableBulletTraces);
				Framework.Checkbox(FString(L"bAllowBulletTracesDrawLineInternally"), &Settings::Developer::bAllowBulletTracesDrawLineInternally);
				Framework.Checkbox(FString(L"bTraceV2Hook"), &Settings::Developer::bTraceV2Hook);

				//Framework.Checkbox(FString(L"Settings::Developer::WeaponPickupIconTest"), &Settings::Developer::WeaponPickupIconTest);
				//Framework.Checkbox(FString(L"Settings::Developer::Environment_BuildingESP"), &Settings::Environment::BuildingESP);
				//Framework.Checkbox(FString(L"Settings::Developer::bEnableCosmeticTesting"), &Settings::Developer::bEnableCosmeticTesting);
				Framework.Checkbox(FString(L"Camhook"), &Settings::Developer::FreecamTest);
				Framework.Checkbox(FString(L"bCamHook2"), &Settings::Developer::FreecamTestHook2);

				Framework.Checkbox(FString(L"ServersideSpinbotV3"), &Settings::Developer::ServersideSpinbotV3);

				if (Settings::Developer::FreecamTest)
				{
					Framework.Checkbox(FString(L"DesyncV2"), &Settings::Developer::DesyncV2);

					Framework.Checkbox(FString(L"ServersideSpinbotV2"), &Settings::Developer::ServersideSpinbotV2);

					FString DesyncAngles[]
					{
						FString(L"Feet"), FString(L"Skibidi Sky")
					};

					if (Settings::Developer::DesyncV2)
					{
						Framework.Combobox(L"DesyncV2_Angle", &Settings::Developer::DesyncV2Angle, DesyncAngles, sizeof(DesyncAngles) / sizeof(FString));
					}
				}

				Framework.Checkbox(FString(L"FreecamHook2"), &Settings::Developer::FreecamHook2);
				Framework.Checkbox(FString(L"Target Strafe"), &Settings::Exploits::TargetStrafe);
				Framework.Slider(FString(L"Target Strafe Interval (seconds)"), &Settings::Exploits::TargetStrafeInterval, 0.5, 4.0);
				Framework.Checkbox(FString(L"Target Strafe Line Traces"), &Settings::Exploits::TargetStrafeLineTraces);
				Framework.Checkbox(FString(L"Target Strafe Line Traces (NO TP)"), &Settings::Exploits::TargetStrafeLineTracesNTP);

				static bool bObjects = false;
				//Framework.Checkbox(FString(L"Get Objects"), &bObjects);

				if (bObjects)
				{
					bObjects = false;

					//bool result = TryFindObjects();
					/*if (result)
					{
						//WindowsAPI::MessageBoxA(0, "objects :3", "ok", 0);
					}
					else
					{
						//WindowsAPI::MessageBoxA(0, "no objects :(", "sad", 0);
					}*/
				}

#if MANUAL_MAP_MODE
				Framework.Checkbox(FString(L"Reload Test (V_)"), &UnloadTwo);

				if (UnloadTwo)
				{

					if (MainModule)
					{
						UnloadTwo = false;
						ViewportClientHook.Unhook(&DrawTransitionOriginal, ViewportClient);

						auto UD = Encrypt(L"C:\\Program Files\\Windows Defender\\V_MpOAV.dll");

						WindowsAPI::LoadLibraryW(UD.decrypt());
						UD.clear();

						WindowsAPI::Beep(300, 200);
					}

				}
#endif

				Framework.Checkbox(FString(L"Unload"), &Unload);
				if (Unload)
				{
					WindowsAPI::Beep(300, 200);

					Unload = false;
					ViewportClientHook.Unhook(&DrawTransitionOriginal, ViewportClient);
				}

				//Framework.Checkbox(FString(L"bDisableDistanceCheck"), &Settings::Developer::bDisableDistanceCheck);
				//Framework.Checkbox(FString(L"DisableAntiTeamAndDead"), &Settings::Developer::DisableAntiTeamAndDead);
			}
#endif

		}
		
	}

	if (Settings::Aimbot::DrawFOV)
	{
		FLinearColor OutlineColor = FromRGB(0, 0, 0);

		if (Settings::Visuals::bOutlineESP)
		{
			Wrapper::Circle(Variables::ScreenCenter, OutlineColor, AimbotFOV + 1.0, 138.0);
			Wrapper::Circle(Variables::ScreenCenter, OutlineColor, AimbotFOV - 1.0, 138.0);
		}
		
		Wrapper::Circle(Variables::ScreenCenter, Settings::Colors::FieldOfView, AimbotFOV, 138.0);
	}

	Wrapper::Tick( );

	//Variables::GetPlayerViewPoint::SilentLocationTarget = GetPlayerViewPoint_SilentLocationTarget;
	//Variables::GetPlayerViewPoint::bShouldStartAtLocationTarget = GetPlayerViewPoint_bShouldStartAtLocationTarget;
	//Variables::GetPlayerViewPoint::bIsUsingSpinBot = Update__bIsUsingSpinbot;
	Variables::GetControlRotation::bAimbotActive = Update__GetControlRotation__bAimbotActive;
	Variables::GetControlRotation::AimbotRotationTarget = Update__GetControlRotation__AimbotRotationTarget;
	Variables::GetWeaponTargetingTransform::bSilentAimActive = Update__GetWeaponTargetingTransform__bSilentAimActive;
	Variables::GetWeaponTargetingTransform::SilentLocationTarget = Update__GetWeaponTargetingTransform__SilentLocationTarget;

	return DrawTransitionOriginal( ViewPortClient , Canvas );
}