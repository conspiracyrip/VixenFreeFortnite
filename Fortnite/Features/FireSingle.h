#pragma once

void(*FireSingleOriginal)(AFortWeapon*);
void FireSingle(AFortWeapon* CurrentWeapon)
{
	

	reinterpret_cast<void* (__cdecl*)(AFortWeapon*, uintptr_t)>(CurrentWeapon, FireSingleOriginal);
}