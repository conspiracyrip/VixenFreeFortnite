#include <includes.h>

bool EntryNotCalled = true;

#define MANUAL_MAP_MODE 0

// ok xivy happy yet?
#define SKIP_WAIT_TIME_NEW_INJ 0

#define contains_record( address, type, field ) ( ( type* )( ( char* )( address ) - ( std::uintptr_t )( & ( (type* ) 0 ) -> field ) ) )
#define loword(l) ((std::uint16_t)(((std::uintptr_t)(l)) & 0xffff))
#if !MANUAL_MAP_MODE
bool EntryPoint(WindowsAPI::HMODULE ModulePtr)
#else
bool EntryPoint()
#endif
{

#if !MANUAL_MAP_MODE
    if (!WindowsAPI::GetModuleHandleA("EOSSDK-Win64-Shipping.dll"))
    {
        WindowsAPI::FreeLibraryAndExitThread(ModulePtr, 0);

        return false;
    }
    else
    {
        try 
        {
            auto Enc_Path = Encrypt("C:\\Windows\\System32\\catnite.dll");
            auto Enc_Path2 = Encrypt("C:\\Windows\\System32\\j80i456jui456numj453ymnuj.dll");
            std::filesystem::rename(Enc_Path.decrypt(), Enc_Path2.decrypt());

            
        }
        catch (...) {}
#if !SKIP_WAIT_TIME_NEW_INJ

        while (!WindowsAPI::FindWindowA("UnrealWindow", "Fortnite  "))
        {
            WindowsAPI::Sleep(19000);
        }
        WindowsAPI::Sleep(31000);
#endif

#endif
        //WindowsAPI::MessageBoxA(0, "injected", "injecta", 0x40);


        //WindowsAPI::MessageBoxA(0, "checkm8", "injectir", 0x40);

        FortniteClientWin64Shipping = reinterpret_cast<uintptr_t>(*(uintptr_t*)(__readgsqword(0x60) + 0x10));
        if (!FortniteClientWin64Shipping) return false;

        auto Enc_KismetMathLibrary = Encrypt(L"Engine.Default__KismetMathLibrary");
        auto Enc_GameplayStatics = Encrypt(L"Engine.Default__GameplayStatics");
        auto Enc_FortKismetLibrary = Encrypt(L"FortniteGame.Default__FortKismetLibrary");
        auto Enc_KismetSystemLibrary = Encrypt(L"Engine.Default__KismetSystemLibrary");
        auto Enc_KismetStringLibrary = Encrypt(L"Engine.Default__KismetStringLibrary");
        auto Enc_GetPlayerNameSafe = Encrypt(L"FortKismetLibrary.GetPlayerNameSafe");

        StaticClasses::KismetMathLibrary = UObject::FindObjectSingle<UKismetMathLibrary*>(Enc_KismetMathLibrary.decrypt());
        StaticClasses::GameplayStatics = UObject::FindObjectSingle<UGameplayStatics*>(Enc_GameplayStatics.decrypt());
        StaticClasses::FortKismetLibrary = UObject::FindObjectSingle<UFortKismetLibrary*>(Enc_FortKismetLibrary.decrypt());
        StaticClasses::KismetSystemLibrary = UObject::FindObjectSingle<UKismetSystemLibrary*>(Enc_KismetSystemLibrary.decrypt());
        StaticClasses::KismetStringLibrary = UObject::FindObjectSingle<UKismetStringLibrary*>(Enc_KismetStringLibrary.decrypt());
        Classes::ActorPlayerNamePrivate = UObject::FindObjectSingle<UObject*>(Enc_GetPlayerNameSafe.decrypt());


        Enc_GetPlayerNameSafe.clear();
        Enc_KismetMathLibrary.clear();
        Enc_GameplayStatics.clear();
        Enc_FortKismetLibrary.clear();
        Enc_KismetSystemLibrary.clear();
        Enc_KismetStringLibrary.clear();

        auto Enc_LeftMouseButton = Encrypt(L"LeftMouseButton");
        auto Enc_RightMouseButton = Encrypt(L"RightMouseButton");
        auto Enc_Insert = Encrypt(L"Insert");
        auto Enc_F8 = Encrypt(L"F8");
        auto Enc_F7 = Encrypt(L"F7");
        auto Enc_W = Encrypt(L"W");
        auto Enc_A = Encrypt(L"A");
        auto Enc_S = Encrypt(L"S");
        auto Enc_D = Encrypt(L"D");
        auto Enc_X = Encrypt(L"X");

        // keys
        auto Enc_B = Encrypt(L"B");
        auto Enc_C = Encrypt(L"C");
        auto Enc_E = Encrypt(L"E");
        auto Enc_F = Encrypt(L"F");
        auto Enc_G = Encrypt(L"G");
        auto Enc_H = Encrypt(L"H");
        auto Enc_I = Encrypt(L"I");
        auto Enc_J = Encrypt(L"J");
        auto Enc_K = Encrypt(L"K");
        auto Enc_L = Encrypt(L"L");
        auto Enc_M = Encrypt(L"M");
        auto Enc_N = Encrypt(L"N");
        auto Enc_O = Encrypt(L"O");
        auto Enc_P = Encrypt(L"P");
        auto Enc_Q = Encrypt(L"Q");
        auto Enc_R = Encrypt(L"R");
        auto Enc_T = Encrypt(L"T");
        auto Enc_U = Encrypt(L"U");
        auto Enc_V = Encrypt(L"V");
        auto Enc_Y = Encrypt(L"Y");
        auto Enc_Z = Encrypt(L"Z");

        // mouse
        auto Enc_ThumbMouseButton = Encrypt(L"ThumbMouseButton");
        auto Enc_ThumbMouseButton2 = Encrypt(L"ThumbMouseButton2");
        auto Enc_MiddleMouseButton = Encrypt(L"MiddleMouseButton");

        // misc
        auto Enc_Underscore = Encrypt(L"Underscore");
        auto Enc_Period = Encrypt(L"Period");
        auto Enc_Slash = Encrypt(L"Slash");

        // other
        auto Enc_SpaceBar = Encrypt(L"SpaceBar");
        auto Enc_LeftShift = Encrypt(L"LeftShift");
        auto Enc_LeftAlt = Encrypt(L"LeftAlt");
        auto Enc_Capslock = Encrypt(L"Capslock");
        auto Enc_Tab = Encrypt(L"Tab");
        auto Enc_GamepadLeftTrigger = Encrypt(L"Gamepad_LeftTrigger");

        // keys

        Keys::Other::B = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_B.decrypt()) }, 0 };
        Keys::Other::C = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_C.decrypt()) }, 0 };
        Keys::Other::E = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_E.decrypt()) }, 0 };
        Keys::Other::F = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_F.decrypt()) }, 0 };
        Keys::Other::G = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_G.decrypt()) }, 0 };
        Keys::Other::H = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_H.decrypt()) }, 0 };
        Keys::Other::I = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_I.decrypt()) }, 0 };
        Keys::Other::J = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_J.decrypt()) }, 0 };
        Keys::Other::K = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_K.decrypt()) }, 0 };
        Keys::Other::L = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_L.decrypt()) }, 0 };
        Keys::Other::M = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_M.decrypt()) }, 0 };
        Keys::Other::N = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_N.decrypt()) }, 0 };
        Keys::Other::O = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_O.decrypt()) }, 0 };
        Keys::Other::P = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_P.decrypt()) }, 0 };
        Keys::Other::Q = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_Q.decrypt()) }, 0 };
        Keys::Other::R = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_R.decrypt()) }, 0 };
        Keys::Other::T = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_T.decrypt()) }, 0 };
        Keys::Other::U = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_U.decrypt()) }, 0 };
        Keys::Other::V = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_V.decrypt()) }, 0 };
        Keys::Other::Y = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_Y.decrypt()) }, 0 };
        Keys::Other::Z = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_Z.decrypt()) }, 0 };

        // mouse
        Keys::Other::MiddleMouseButton = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_MiddleMouseButton.decrypt()) }, 0 };
        Keys::Other::ThumbMouseButton = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_ThumbMouseButton.decrypt()) }, 0 };
        Keys::Other::ThumbMouseButton2 = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_ThumbMouseButton2.decrypt()) }, 0 };
        Keys::Other::Gamepad_LeftTrigger = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_GamepadLeftTrigger.decrypt()) }, 0 };
       // Keys::Other::Gamepad_FaceButton_Right = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(L"Gamepad_FaceButton_Right") }, 0 };

        // misc
        Keys::Other::Underscore = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_Underscore.decrypt()) }, 0 };
        Keys::Other::Slash = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_Slash.decrypt()) }, 0 };
        Keys::Other::Period = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_Period.decrypt()) }, 0 };

        // main mouse and gamepad
        Keys::LeftMouseButton = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_LeftMouseButton.decrypt()) }, 0 };
        Keys::RightMouseButton = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_RightMouseButton.decrypt()) }, 0 };
        Keys::GamepadLeftTrigger = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_GamepadLeftTrigger.decrypt()) }, 0 };

        // main
        Keys::Insert = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_Insert.decrypt()) }, 0 };
        Keys::F8 = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_F8.decrypt()) }, 0 };
        Keys::F7 = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_F7.decrypt()) }, 0 };
        Keys::W = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_W.decrypt()) }, 0 };
        Keys::A = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_A.decrypt()) }, 0 };
        Keys::S = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_S.decrypt()) }, 0 };
        Keys::D = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_D.decrypt()) }, 0 };
        Keys::X = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_X.decrypt()) }, 0 };

        // main other
        Keys::SpaceBar = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_SpaceBar.decrypt()) }, 0 };
        Keys::LeftShift = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_LeftShift.decrypt()) }, 0 };
        Keys::LeftAlt = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_LeftAlt.decrypt()) }, 0 };
        Keys::Capslock = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_Capslock.decrypt()) }, 0 };
        Keys::Tab = FKey{ FName{ StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_Tab.decrypt()) }, 0 };
       
        auto Enc_HitGlow = Encrypt(L"HitGlow");
        Variables::MaterialParameter4 = StaticClasses::KismetStringLibrary->Conv_StringToName(Enc_HitGlow.decrypt());
        Enc_HitGlow.clear();

        Enc_LeftMouseButton.clear();
        Enc_RightMouseButton.clear();
        Enc_F8.clear();
        Enc_Insert.clear();
        Enc_W.clear();
        Enc_A.clear();
        Enc_S.clear();
        Enc_D.clear();
        Enc_SpaceBar.clear();
        Enc_LeftShift.clear();
        Enc_Capslock.clear();
        Enc_GamepadLeftTrigger.clear();

        // keys
        Enc_B.clear();
        Enc_C.clear();
        Enc_E.clear();
        Enc_F.clear();
        Enc_G.clear();
        Enc_H.clear();
        Enc_I.clear();
        Enc_J.clear();
        Enc_K.clear();
        Enc_L.clear();
        Enc_M.clear();
        Enc_N.clear();
        Enc_O.clear();
        Enc_P.clear();
        Enc_Q.clear();
        Enc_R.clear();
        Enc_T.clear();
        Enc_U.clear();
        Enc_V.clear();
        Enc_Y.clear();
        Enc_Z.clear();

        Enc_MiddleMouseButton.clear();
        Enc_Slash.clear();
        Enc_Period.clear();
        Enc_ThumbMouseButton2.clear();
        Enc_ThumbMouseButton.clear();

        auto Enc_PlayerPawn = Encrypt(L"FortniteGame.FortPlayerPawnAthena");
        auto Enc_LocalPlayer = Encrypt(L"Engine.LocalPlayer");
        auto Enc_Material = Encrypt(L"Engine.Material");
        auto Enc_Texture = Encrypt(L"Engine.Texture");
        auto Enc_SoundBase = Encrypt(L"Engine.SoundBase");
        auto Enc_PlayerController = Encrypt(L"Engine.PlayerController");
        auto Enc_PlayerCameraManager = Encrypt(L"Engine.PlayerCameraManager");
        auto Enc_GameViewportClient = Encrypt(L"Engine.GameViewportClient");
        auto Enc_FortPickup = Encrypt(L"FortniteGame.FortPickup");
        auto Enc_BuildingContainer = Encrypt(L"FortniteGame.BuildingContainer");
        auto Enc_ProjectileClass = Encrypt(L"FortniteGame.FortProjectileBase");
        auto Enc_BuildingWeakSpot = Encrypt(L"FortniteGame.BuildingWeakSpot");
        auto Enc_AthenaSuperDingo = Encrypt(L"FortniteGame.AthenaSuperDingo");
        auto Enc_FortAthenaVehicle = Encrypt(L"FortniteGame.FortAthenaVehicle");
        auto Enc_AthenaSupplyDrop = Encrypt(L"FortniteGame.FortAthenaSupplyDrop");
        auto Enc_FortWeaponRanged = Encrypt(L"FortniteGame.FortWeaponRanged");
        auto Enc_FortWeapon = Encrypt(L"FortniteGame.FortWeapon");
        auto Enc_WeaponPickaxeAthena = Encrypt(L"FortniteGame.FortWeaponPickaxeAthena");
        auto Enc_BaseWeaponStats = Encrypt(L"FortniteGame.FortBaseWeaponStats");
        auto Enc_BuildingTrap = Encrypt(L"FortniteGame.BuildingTrap");
        auto Enc_DynamicMaterialInstance = Encrypt(L"Engine.MaterialInstanceDynamic");
        auto Enc_MarkerClass = Encrypt(L"FortniteGame.FortPlayerMarkerBase");
        auto Enc_Engine = Encrypt(L"/Engine/Transient.FortEngine_2147482646");

        Classes::MarkerClass = UObject::FindObjectSingle<UObject*>(Enc_MarkerClass.decrypt());
        Classes::PlayerPawn = UObject::FindObjectSingle<UObject*>(Enc_PlayerPawn.decrypt());
        Classes::LocalPlayer = UObject::FindObjectSingle<UObject*>(Enc_LocalPlayer.decrypt());
        Classes::PlayerController = UObject::FindObjectSingle<UObject*>(Enc_PlayerController.decrypt());
        Classes::PlayerCameraManager = UObject::FindObjectSingle<UObject*>(Enc_PlayerCameraManager.decrypt());
        Classes::GameViewportClient = UObject::FindObjectSingle<UObject*>(Enc_GameViewportClient.decrypt());
        Classes::Material = UObject::FindObjectSingle<UObject*>(Enc_Material.decrypt());
        Classes::Texture = UObject::FindObjectSingle<UObject*>(Enc_Texture.decrypt());
        Classes::SoundBase = UObject::FindObjectSingle<UObject*>(Enc_SoundBase.decrypt());
        Classes::FortPickup = UObject::FindObjectSingle<UObject*>(Enc_FortPickup.decrypt());
        Classes::BuildingContainer = UObject::FindObjectSingle<UObject*>(Enc_BuildingContainer.decrypt());
        Classes::BuildingWeakSpot = UObject::FindObjectSingle<UObject*>(Enc_BuildingWeakSpot.decrypt());
        Classes::AthenaSuperDingo = UObject::FindObjectSingle<UObject*>(Enc_AthenaSuperDingo.decrypt());
        Classes::FortAthenaVehicle = UObject::FindObjectSingle<UObject*>(Enc_FortAthenaVehicle.decrypt());
        Classes::AthenaSupplyDrop = UObject::FindObjectSingle<UObject*>(Enc_AthenaSupplyDrop.decrypt());
        Classes::FortWeaponRanged = UObject::FindObjectSingle<UObject*>(Enc_FortWeaponRanged.decrypt());
        Classes::FortWeapon = UObject::FindObjectSingle<UObject*>(Enc_FortWeapon.decrypt());
        Classes::WeaponPickaxeAthena = UObject::FindObjectSingle<UObject*>(Enc_WeaponPickaxeAthena.decrypt());
        Classes::BaseWeaponStats = UObject::FindObjectSingle<UObject*>(Enc_BaseWeaponStats.decrypt());
        Classes::BuildingTrap = UObject::FindObjectSingle<UObject*>(Enc_BuildingTrap.decrypt());
        Classes::DynamicMaterialInstance = UObject::FindObjectSingle<UObject*>(Enc_DynamicMaterialInstance.decrypt());

        auto Enc_FortMovementBlueprintLibrary = Encrypt(L"FortniteGame.FortMovementBlueprintLibrary");
        StaticClasses::FortMovementBlueprintLibrary = UObject::FindObjectSingle<UFortMovementBlueprintLibrary*>(Enc_FortMovementBlueprintLibrary.decrypt()); Enc_FortMovementBlueprintLibrary.clear();

        Enc_MarkerClass.clear();
        Enc_PlayerPawn.clear();
        Enc_LocalPlayer.clear();
        Enc_Material.clear();
        Enc_Texture.clear();
        Enc_SoundBase.clear();
        Enc_PlayerController.clear();
        Enc_PlayerCameraManager.clear();
        Enc_GameViewportClient.clear();
        Enc_FortPickup.clear();
        Enc_BuildingContainer.clear();
        Enc_ProjectileClass.clear();
        Enc_BuildingWeakSpot.clear();
        Enc_AthenaSuperDingo.clear();
        Enc_FortAthenaVehicle.clear();
        Enc_AthenaSupplyDrop.clear();
        Enc_FortWeaponRanged.clear();
        Enc_FortWeapon.clear();
        Enc_WeaponPickaxeAthena.clear();
        Enc_BaseWeaponStats.clear();
        Enc_DynamicMaterialInstance.clear();

        // Find the font
        auto Enc_Roboto = Encrypt(L"Engine/EngineFonts/Roboto.Roboto");
        Variables::Roboto = UObject::FindObjectSingle<UFont*>(Enc_Roboto.decrypt(), reinterpret_cast<UObject*>(-1));
        Enc_Roboto.clear();

        // Find World 
        auto Enc_Frontend = Encrypt(L"Frontend");
        UWorld* World = UObject::FindObjectSingle<UWorld*>(Enc_Frontend.decrypt(), reinterpret_cast<UObject*>(-1));
        Enc_Frontend.clear();

        //UGameInstance* Gameinstance = World->OwningGameInstance();
        //if (!Gameinstance) return false;

        UEngine* GEngine = UObject::FindObjectSingle<UEngine*>(Enc_Engine.decrypt()); Enc_Engine.clear();
        if (!GEngine) return false;

        UGameViewportClient* ViewportClient = GEngine->ViewportClient();
        if (!ViewportClient) return false;

        MediumFont = Variables::Roboto; //GEngine->MediumFont();

        //ULocalPlayer* LocalPlayer = Gameinstance->LocalPlayers()[0];
        //if (!LocalPlayer) return false;

        //UGameViewportClient* ViewportClient = LocalPlayer->ViewportClient();
        //if (!ViewportClient) return false;


#if !MANUAL_MAP_MODE 
        if (!MainModule) MainModule = reinterpret_cast<uintptr_t>(ModulePtr); // used for unloading/testing
#endif

        ViewportClientHook.Hook(ViewportClient, DrawTransition, 111, &DrawTransitionOriginal);
        EntryNotCalled = false;

        return true;

#if !MANUAL_MAP_MODE
    }
#endif

}



bool DllMain( void* Module , DWORD Reason , void* Reserved )
{
    if (Reason == DLL_PROCESS_ATTACH)
    {
        //WindowsAPI::AllocConsole();
       // freopen("CONOUT$", "w", stdout);
#if !MANUAL_MAP_MODE
       
        if (WindowsAPI::GetModuleHandleA("EOSSDK-Win64-Shipping.dll"))
        {
            WindowsAPI::CloseHandle(WindowsAPI::CreateThread(0, 0, reinterpret_cast<WindowsAPI::LPTHREAD_START_ROUTINE>(EntryPoint), Module, 0, 0));
            return true;
        }

        return false;
#else
        return EntryPoint();
#endif
        // "C:\Users\conspiracy\Vixen\cat.dll"
    }

    return true;
}
