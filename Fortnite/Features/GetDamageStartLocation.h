#pragma once

void* (*GetDamageStartLocationOriginal)(AFortWeapon*, FVector*, FRotator*, int);
void* GetDamageStartLocation(AFortWeapon* CurrentWeapon, FVector* Location, FRotator* Rotation, int a4)
{
    void* ReturnValue = GetDamageStartLocationOriginal(CurrentWeapon, Location, Rotation, a4);

    if (Settings::Exploits::SilentAimbot)
    {
        if (Variables::GetWeaponTargetingTransform::bSilentAimActive && Variables::GetWeaponTargetingTransform::SilentLocationTarget)
        {
            FRotator PerfectRotation = StaticClasses::KismetMathLibrary->FindLookAtRotation(*Location, Variables::GetWeaponTargetingTransform::SilentLocationTarget);
            if (PerfectRotation && Rotation)
            {
                *Rotation = PerfectRotation;
            }
        }
    }

    return ReturnValue;
}