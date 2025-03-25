#pragma once

//reinterpret_cast<void(__cdecl*)(AFortWeapon*, uintptr_t, void*)>(DoSpoofCall)(FortWeapon, SpoofCode, (void*)(GameBase + Offsets::Free));

void (*TryToFireOriginal)(AFortWeapon*);
void TryToFire(AFortWeapon* FortWeapon)
{

	reinterpret_cast<void(__cdecl*)(AFortWeapon*, uintptr_t)>(FortWeapon, TryToFireOriginal);
}