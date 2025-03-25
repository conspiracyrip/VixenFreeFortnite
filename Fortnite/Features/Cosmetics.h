

static void SetCosmeticMultiNR(APlayerController* PlayerController,
    bool bUseMat = false, FString RequestedMat = FString(L"None"))
{
    

    struct FFortAthenaLoadout
    {
        uint8_t Pad_0[0x0130]; // v30.30
    };

    FFortAthenaLoadout CosmeticLoadoutPC{};
    *(bool*)(uintptr_t(&CosmeticLoadoutPC) + 0x00C8) = true; // bIsDefaultCharacter
    *(bool*)(uintptr_t(&CosmeticLoadoutPC) + 0x00C9) = true; // bForceUpdateVariants

    static UObject* Function;
    if(!Function)Function=UObject::FindObjectSingle<UObject*>(L"/Script/FortniteGame.FortPlayerControllerAthena.ServerSetCosmeticLoadout");

    struct AFortPlayerControllerAthena_ServerSetCosmeticLoadout_Params
    {
        struct FFortAthenaLoadout Loadout;
        bool bRefreshPawn;
    };

    AFortPlayerControllerAthena_ServerSetCosmeticLoadout_Params Params{};

    Params.Loadout = CosmeticLoadoutPC;
    Params.bRefreshPawn = false;

    if (bUseMat)
    {
        FString Mat = RequestedMat;
        FName MatAsFName = StaticClasses::KismetStringLibrary->Conv_StringToName(Mat);

        // thanks xivy for help with styles :3
        // -conspiracy

        TArray<FCosmeticLoadoutSlot>& slots = *reinterpret_cast<TArray<FCosmeticLoadoutSlot>*>(uintptr_t(&CosmeticLoadoutPC) + 0x0000);

        for (int i = 0; i < slots.Size(); i++)
        {


            UObject** ObjectPtr = reinterpret_cast<UObject**>(uintptr_t(&slots[i]) + 0x08);
            if (!ObjectPtr) {
                continue;
            }

            UObject* Object = *ObjectPtr;
            if (!Object) {
                continue;

                *(UObject**)(uintptr_t(&CosmeticLoadoutPC) + 0x48) = Object; // Character

                TArray<FCosmeticCustomizationInfo>& CustomizationInfo = *reinterpret_cast<TArray<FCosmeticCustomizationInfo>*>(uintptr_t(&slots[i]) + 0x10);
                TArray<FMcpVariantChannelInfo> variants;

                for (int j = 0; j < CustomizationInfo.Size(); j++) {



                    FMcpVariantChannelInfo CharacterVariantChannel{};
                    auto& ChannelTag = *reinterpret_cast<FGameplayTag*>(uintptr_t(&CustomizationInfo[j]) + 0x0);
                    auto& ActiveTag = *reinterpret_cast<FGameplayTag*>(uintptr_t(&CustomizationInfo[j]) + 0x4);
                    CharacterVariantChannel.VariantChannelTag = ChannelTag; // ChannelTag
                    CharacterVariantChannel.ActiveVariantTag = ActiveTag; // ActiveTag

                    FGameplayTagContainer Fgg{};
                    TArray<FGameplayTag> arr;
                    arr.Add(ChannelTag);
                    arr.Add(ActiveTag);
                    Fgg.GameplayTags = arr;

                    CharacterVariantChannel.CustomData = *reinterpret_cast<FString*>(uintptr_t(&CustomizationInfo[j]) + 0x8); // AdditionalData
                    CharacterVariantChannel.OwnedVariantTags = Fgg; // OwnedVariantTags
                    CharacterVariantChannel.ItemVariantIsUsedFor = Object;
                    variants.Add(CharacterVariantChannel);
                }

                *(TArray<FMcpVariantChannelInfo>*)(uintptr_t(&CosmeticLoadoutPC) + 0x50) = variants;
            }
        }
    }

    PlayerController->ProcessEvent(Function, &Params);
}

static void SetCosmeticMulti(APlayerController*PlayerController, 
    const wchar_t* SkinName, const wchar_t* BackblingName, const wchar_t* PickaxeName, const wchar_t* GliderName, const wchar_t* SkyDiveContrailName,
    bool bUseMat = false, FString RequestedMat = FString(L"None"))
{
    struct FFortAthenaLoadout
    {
        uint8_t Pad_0[0x0130]; // v28.20
    };

    FFortAthenaLoadout CosmeticLoadoutPC{};

    if (SkinName != L"") {
        std::wstring WSkinName = std::wstring(SkinName);
        std::wstring CWSkinName = L"/Game/Athena/Items/Cosmetics/Characters/" + WSkinName + L"." + WSkinName; 
        auto Object = UObject::FindObjectSingle<UObject*>(CWSkinName.c_str());
        if (Object) { 
            *(UObject**)(uintptr_t(&CosmeticLoadoutPC) + 0x48) = Object; 
        }
        else {
            std::wstring WSkinName = std::wstring(SkinName);
            std::wstring CWSkinName = L"/BRCosmetics/Athena/Items/Cosmetics/Characters/" + WSkinName + L"." + WSkinName; 
            auto Object = UObject::FindObjectSingle<UObject*>(CWSkinName.c_str()); 
            *(UObject**)(uintptr_t(&CosmeticLoadoutPC) + 0x48) = Object; 
        }// Character
    }
    if (BackblingName != L"") {
        std::wstring WBacknNAme = std::wstring(BackblingName);
        std::wstring Cwbajs = L"/BRCosmetics/Athena/Items/Cosmetics/Backpacks/" + WBacknNAme + L"." + WBacknNAme; 
        *(UObject**)(uintptr_t(&CosmeticLoadoutPC) + 0x60) = UObject::FindObjectSingle<UObject*>(Cwbajs.c_str()); // Backpack 
    }
    if (GliderName != L"") {
        std::wstring Glidernamemwe = std::wstring(GliderName);
        std::wstring Glasidswjd = L"/BRCosmetics/Athena/Items/Cosmetics/Gliders/" + Glidernamemwe + L"." + Glidernamemwe;
        *(UObject**)(uintptr_t(&CosmeticLoadoutPC) + 0x38) = UObject::FindObjectSingle<UObject*>(Glasidswjd.c_str()); // Glider
    }
    if (SkyDiveContrailName != L"") {
        std::wstring Comtrial = std::wstring(SkyDiveContrailName); 
        std::wstring Ciqaiskqi = L"/BRCosmetics/Athena/Items/Cosmetics/Contrails/" + Comtrial + L"." + Comtrial;
        *(UObject**)(uintptr_t(&CosmeticLoadoutPC) + 0x30) = UObject::FindObjectSingle<UObject*>(Ciqaiskqi.c_str()); // SkyDiveContrail 
    }
    if (PickaxeName != L"")
    {
        std::wstring PickaxeNameify = std::wstring(PickaxeName);
        std::wstring pixlers = L"/BRCosmetics/Athena/Items/Cosmetics/Pickaxes/" + PickaxeNameify + L"." + PickaxeNameify;
        *(UObject**)(uintptr_t(&CosmeticLoadoutPC) + 0x40) = UObject::FindObjectSingle<UObject*>(pixlers.c_str()); // Pickaxe Offset needed
    }

   // *(UObject**)(uintptr_t(&CosmeticLoadoutPC) + 0x40) = UObject::FindObjectSingle<UObject*>(pixlers.c_str()); // Pickaxe Offset needed

    
    *(bool*)(uintptr_t(&CosmeticLoadoutPC) + 0x00C8) = false; // bIsDefaultCharacter
    *(bool*)(uintptr_t(&CosmeticLoadoutPC) + 0x00C9) = true; // bForceUpdateVariants
   
    if (bUseMat)
    {
        FString Mat = RequestedMat;
        FName MatAsFName = StaticClasses::KismetStringLibrary->Conv_StringToName(Mat);
        // thanks xivy for help with styles :3
        // -conspiracy

        TArray<FCosmeticLoadoutSlot>& slots = *reinterpret_cast<TArray<FCosmeticLoadoutSlot>*>(uintptr_t(&CosmeticLoadoutPC) + 0x0000);
        for (int i = 0; i < slots.Size(); i++)
        {
            UObject** ObjectPtr = reinterpret_cast<UObject**>(uintptr_t(&slots[i]) + 0x08);
            if (!ObjectPtr)  continue;

            UObject* Object = *ObjectPtr;
            if (!Object) continue;

            *(UObject**)(uintptr_t(&CosmeticLoadoutPC) + 0x48) = Object; // Character

            TArray<FCosmeticCustomizationInfo>& CustomizationInfo = *reinterpret_cast<TArray<FCosmeticCustomizationInfo>*>(uintptr_t(&slots[i]) + 0x10); // idk wtf this is

            TArray<FMcpVariantChannelInfo> variants;

            for (int j = 0; j < CustomizationInfo.Size(); j++)
            {
                FMcpVariantChannelInfo CharacterVariantChannel{};
                auto& ChannelTag = *reinterpret_cast<FGameplayTag*>(uintptr_t(&CustomizationInfo[j]) + 0x0);
                auto& ActiveTag = *reinterpret_cast<FGameplayTag*>(uintptr_t(&CustomizationInfo[j]) + 0x4);

                CharacterVariantChannel.VariantChannelTag = ChannelTag; // ChannelTag
                CharacterVariantChannel.ActiveVariantTag = ActiveTag; // ActiveTag

                FGameplayTagContainer Fgg{};
                TArray<FGameplayTag> arr;
                arr.Add(ChannelTag);
                arr.Add(ActiveTag);
                Fgg.GameplayTags = arr;

                CharacterVariantChannel.CustomData = *reinterpret_cast<FString*>(uintptr_t(&CustomizationInfo[j]) + 0x8); // AdditionalData
                CharacterVariantChannel.OwnedVariantTags = Fgg; // OwnedVariantTags
                CharacterVariantChannel.ItemVariantIsUsedFor = Object;
                variants.Add(CharacterVariantChannel);
            }

            *(TArray<FMcpVariantChannelInfo>*)(uintptr_t(&CosmeticLoadoutPC) + 0x50) = variants;
        }
    }

           
    static UObject* Function; if(!Function) Function = UObject::FindObjectSingle<UObject*>(L"/Script/FortniteGame.FortPlayerControllerAthena.ServerSetCosmeticLoadout"); 

    struct AFortPlayerControllerAthena_ServerSetCosmeticLoadout_Params
    {
        struct FFortAthenaLoadout Loadout;
        bool bRefreshPawn;
    };

    AFortPlayerControllerAthena_ServerSetCosmeticLoadout_Params Params{};

    Params.Loadout = CosmeticLoadoutPC;
    Params.bRefreshPawn = true;

    PlayerController->ProcessEvent(Function, &Params);
}