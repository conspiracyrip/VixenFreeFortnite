#pragma once

namespace Settings::Aimbot
{
	bool ShakeXOnly = false;
	bool ShakeYOnly = false;
	bool NoRecoil = false;
	bool MemoryAim = true;
	bool MouseAim = false;
	bool Pred = false;
	bool NoReload = false;
	bool Aimbot = true;
	bool SmartAimbot = false;
	bool Triggerbot = false;
	double Smoothing = 8.0;
	bool OnlyShotgun = false;
	bool TargetLine = false;
	bool TriggerbotConstant = false;
	bool TriggerbotLeftShift = false;
	double TriggerbotInterval = 5.0;
	double TriggerbotDelay = 50.0;
	double Speed = 10.0;
	bool PredictProjectiles = false;
	double FOV = 10.0;
	bool DrawFOV = true;
	double MaxDistance = 100.0;
	bool PredictionDot = false;
	int BoneType = 1;
	int KeyType = 0;
	int SmoothingType = 1;
	bool SkipKnocked = false;
	bool IgnoreBots = false;
	bool VisibleOnly = false;
	double ShakeSpeed = 0.0;
	double ShakeFactor = 5.0;
	double ShakeFactorSmooth = 1.0;

	bool Shake = false;
}
namespace Settings::Aimbot::StickySilent
{
	bool StickyCameraSpin = false;
}

namespace Settings::Cache {
	bool bChamsWasActive = false;
}
namespace Settings::Visuals
{
	bool TextShadow = true;
	bool bOutlinedText = true;
	double BoxThiccness = 3.4;
	double BoxThiccnessClose = 3.8;
	double CornerThiccness = 4.4;
	double CornerThiccnessClose = 2.2;
	double SkeletonThiccness = 5.0;
	double OutlineThiccness = 2.4;
	double OutlineThiccnessClose = 4.0;
	bool bOutlineESP = true;
}
namespace Settings::Player
{
	bool ShowReloading, ShowScoped, ShowAmmoCount, LevelESP, KillCount = false;
	bool NoSelfchams = false;
	bool HeroTest = false;
	bool ThankedBusDriver = false;
	bool Dishwasher = false;
	bool Enable = true;
	int BoxType = 2;
	bool FilledBox = false;
	bool FOVArrows = false;
	bool Skeleton = false;
	int LineType = 0;
	bool Name = false;
	bool Rank = false;
	bool Platform = false;
	bool Weapon = false;
	bool Distance = false;
	bool Radar = false;
	bool EyeDirection = false;
	bool Spectators = false;
	int CrosshairMode = 0;

	double MaxDistance = 350.0;

	double RadarX = 5.0;
	double RadarY = 35.0;
}

namespace Settings::Environment
{
	bool Chests = false;
	bool Ammobox = false;
	bool SilentWeakspot = false;
	bool Pickup = false;
	bool Container = false;
	bool SupplyDrone = false;
	bool Vehicle = false;
	bool SupplyDrop = false;
	bool WeakspotAim = false;
	bool PerformanceMode = false;
	bool BuildingESP = false;
	bool Chams = false;
	bool Traps = false;
	bool DebugObjects = false;
	bool ShowConsole = false;
	bool VehicleChams = false;
	int MinimumTier = 1;
	double MaxDistance = 200.0;
	bool PickupIcons = false;
	bool InstantDestoryBuilding = false;
	bool StormPrediction = false;
}

namespace Settings::Exploits
{
	bool TargetStrafe = false;
	double TargetStrafeInterval = 1.0;
	bool TargetStrafeLineTracesNTP = false;
	bool YeetToOrbit = false;
	bool PlayerFly = false;
	bool TargetStrafeLineTraces = false;
	bool SlideBHOPUseSpaceBar = false;
	bool SlideBHOPUseLeftAlt = false;

	bool OneFSlomoScale = false;
	bool FiveFSlomoScale = false;
	bool ThreeFSlomoScale = false;
	bool EightFSlomoScale = false;

	float PlayerSlomoScale = 0.5f;
	bool PlayerSlomo = false;
	bool NoWeaponOverheat = false;
	bool PerfectGliderBHop = false;
	bool RapidFireNoCooldown = false;
	bool SlideBHOP = false;
	bool FullAutoWeapons = false;
	double SlideBHOPInterval = 0.2;
	bool VehicleTPAura = false;
	bool BulletTPV2_1 = false;
	bool ServersideVehicleSpinbot = false;
	int TeleportMethod = 0;
	bool BulletTPV2 = false;
	bool BulletTPV2_2 = false;
	bool BulletTPV2_4 = false;
	bool InstantInteraction = false;
	bool BulletTP = false;
	bool RapidFire = false;
	bool RapidFireWithKeybind = false;
	bool FirstPerson = false;
	bool AirStuck = false;
	bool DoublePump = false;
	bool NoCarFlip = false;
	bool InstantKillAll = false;
	bool SilentAimbot = false;
	bool SilentHookV2 = false;
	bool SilentAimbotV2 = false;
	bool ThreeSixtyFOV = false;
	bool CarFly = false;
	bool CarSpeedHack = false;
	bool InfiniteFuel = false;
	double CarSpeed = 85.0;
	bool CameraFOVChanger = false;
	double CameraFOV = 120.0;
	double DemospeedValue = 1.0;
	bool DemospeedEnabled = false;
	bool chamsweird = false;
	bool NoSpread = false;
	bool NoRecoil = false;
	bool HitscanWeapons = false;
	bool Chams = false;
	bool HitSound = false;
	bool RainbowChams = false;
	bool Wireframe = false;
	bool bBackTrack = false;
	bool bItemReach = false;
	bool ChamsThroughWalls = false;
	bool AimWhileJumping = false;
	bool Spinbot = false;
	FRotator CachedSpin = FRotator();

	double SpinSpeed = 9.0;
	bool ShootThroughWalls = false;
	int HitSoundType = 0;
	bool InfiniteAmmo = false;

}

namespace Settings::Developer
{
	bool bUnlockDeveloperMode = false;
	bool WeaponPickupIconTest = false;
	bool bEnableCosmeticTesting = false;
	bool bTraceV2Hook = false;
	bool bAllowBulletTracesDrawLineInternally = false;
	bool UE_Console = false;
	bool bDisableSlideEnabledCheck = false;
	bool bUseSecondArgTestForBHOP = true;
	bool bDisableFullAutoWeaponCheck = false;
	bool bValidateGObjects = false;
	bool bHasGObjectBeenValidated = false;
	bool bHasGObjectBeenValidatedState = false;
	bool FreecamTest = false;
	bool DesyncV2 = false;
	bool FreecamTestHook2 = false;
	bool EnableBTPV3_1 = false;
	bool ServersideSpinbotV2 = false;
	bool ServersideSpinbotV3 = false;
	bool FreecamHook2 = false;
	bool EnableBTPV2RelativeRotationTest = false;
	bool DisableBTPV2_1RotationTest = false;
	bool AllowChamsThruWall = false;
	bool bDisableDistanceCheck = false;
	bool DisableAntiTeamAndDead = false;
	bool bKickKeybind = true;
	int KickType = 1;
	int	DesyncV2Angle = 0;
	FString KickString = (FString)L"conspiracy";
}


namespace Settings::Developer::Debugging
{
	bool bDidHealthTestFail = false;
	bool bDidShieldTestFail = false;
	bool DeadZoneTest, AimAssistTest, SessionIDTest, InstantSetupTest, LegacyBindsTest, SmartBuildTest = false;
	float MostRecentHealth = 1337.f;
	double HealthOffset = 51.0;
	double HealthOffsetNegative = 1.0;
	float MostRecentShield = 1337.f;
	double EnvLim = 10.0;
	double BldLim = 10.0;

	bool HealthTestOthers = false;
	bool DisableReloadAnimation = false;
	bool UnsafeActorDebugging = false;
	bool MGBT = false;
	bool MGBTTP = false;
	double MGBTTPLim = 1.0;

	bool ActorDebugging = false;
	bool ObjectDebugging = false;
	bool SpreadDebugging = false;
	bool OverrideLocation = false;
	bool OverrideLocation2 = false;
	bool UIDebugging = false;
	bool AccountIDTest = false;
	bool HealthTestLocal = false;
	bool StickySilentAimV2 = false;
	bool StaticStickyViewpoint = false;
	bool HealthTest = false;
	double TextOffset = 0.0;
}

namespace Settings::Developer::UIKit
{
	bool WatermarkV2UseCenterHorizontal = false;
	bool FunnyWatermark = false;
	bool RealWatermark = true;
	bool ShowNewVixenPrivateTitle = true;
	bool ShowNewIntro = true;


	namespace Prod
	{
		bool bHasCheckedIntro = false;
		bool bHasCheckedUpdate = false;
		bool bHasShownIntro = false;
		bool bIsIntroBeingShown = false;
		bool bNeedsIntroShown = true;
		double CurrentVersion = 1.0;
	}
}

namespace Settings::Misc
{
	bool DisableTextures = false;
	bool NiggaAnimation = false;
	bool BulletTraces = false;
	bool bGermanyCrosshairNiggerMode = false;
	double FontSize = 10.0;
	bool FakeLagging = false;
	bool TextOutline = false;
}

namespace Settings::Skins
{
	bool RenegadeStyle2 = false;
	bool bApplySkinWithKey = false;
	bool bCachedHasAppliedCharacter = false;
	bool bDefaultCharacterEnabled = false;

	std::wstring DefaultCharacter = L"CID_028_Athena_Commando_F";
	std::wstring Character = L"CID_028_Athena_Commando_F";
	std::wstring Backpack = L"BID_004_BlackKnight";
	std::wstring Pickaxe = L"Pickaxe_ID_376_FNCS";
	std::wstring Glider = L"Glider_Warthog";
	std::wstring Contrail = L"Contrail_SeleneCobra";

	namespace Cosmetics
	{
		bool AllowTestingOfMats = false;

		std::wstring RenegadeRaindeer = L"CID_028_Athena_Commando_F";
		std::wstring NiggaTrooper = L"CID_017_Athena_Commando_M";
		std::wstring BurgerKingGuy = L"CID_035_Athena_Commando_M_Medieval"; // burger king bk black knight
		std::wstring ChinkSkin = L"CID_313_Athena_Commando_M_KpopFashion";
		// i can't think of a racist/funny name for this
		std::wstring Galaxy = L"CID_175_Athena_Commando_M_Celestial"; // galaxy
		std::wstring YourDad = L"CID_636_Athena_Commando_M_GalileoGondola_78MFZ"; // this is obvious is it not?
		std::wstring Sparkle = L"CID_039_Athena_Commando_F_Disco"; 
		std::wstring DarkSkully = L"CID_850_Athena_Commando_F_SkullBriteCube";
		std::wstring GhoulTrooper = L"CID_029_Athena_Commando_F_Halloween";
		std::wstring Material3 = L"Mat3";
		std::wstring Material2 = L"Mat2";
		std::wstring Material1 = L"Mat1";
	}
}

namespace Settings::Colors
{
	FLinearColor RainbowChamsColorUWU = FLinearColor(1.f, 1.f, 0.f, 1.f); /* please help me ive been debugging wireframe and chams for the last 4 hours and im watching my
			friend bullet tp a 8 year old and him cry the whole time*/	/**
	* RGB ESP Colors
	* FromRGB(163,204,60) -> Green
	* FromRGB(182,119,163) -> Pink
	* FromRGB(142,130,209) -> Purple
	*/
	FLinearColor TextColor = FLinearColor(0.8f, 0.8f, 0.8f, 1.f);
	FLinearColor PlayerChams = FLinearColor( 1.f , 0.4f , 1.f , 1.f );
	FLinearColor TargetLine = FLinearColor( 1.f , 0.f , 0.f , 1.f );
	FLinearColor FieldOfView = FLinearColor( 1.f , 1.f , 1.f , 1.f );
	FLinearColor TeammateColor = FLinearColor( 0.0f , 1.0f , 1.0f , 1.0f );
	FLinearColor BoxVisible = FromRGB( 215, 21, 21 );
	FLinearColor BoxInVisible = FLinearColor( 1.0f , 0.0f , 0.0f , 1.0f );
	FLinearColor SkeletonVisible = FromRGB( 215, 21, 21 );
	FLinearColor SkeletonInVisible = FLinearColor( 1.0f , 0.0f , 0.0f , 1.0f );
	FLinearColor SnaplineVisible = FromRGB( 215, 21, 21 );
	FLinearColor SnaplineInVisible = FLinearColor( 1.0f , 0.0f , 0.0f , 1.0f );
}
