#pragma once

bool ( *HasInfiniteAmmoOrignal )( AFortWeapon* );
bool HasInfiniteAmmo( AFortWeapon* CurrentWeapon )
{
	bool ReturnValue = HasInfiniteAmmoOrignal( CurrentWeapon );

	if ( Settings::Exploits::InfiniteAmmo )
	{
		return false;
	}

	return ReturnValue;
}

bool ( *HasInfiniteChargesOrignal )( UObject* , UFortItemDefinition* );
bool HasInfiniteCharges( UObject* This , UFortItemDefinition* WeaponData )
{
	bool ReturnValue = HasInfiniteChargesOrignal( This , WeaponData );

	if ( Settings::Exploits::InfiniteAmmo )
	{
		return true;
	}

	return ReturnValue;
}

bool ( *HasInfiniteEnergyPoolOrignal )( UObject* , UFortItemDefinition* );
bool HasInfiniteEnergyPool( UObject* This , UFortItemDefinition* WeaponData )
{
	bool ReturnValue = HasInfiniteEnergyPoolOrignal( This , WeaponData );

	if ( Settings::Exploits::InfiniteAmmo )
	{
		return true;
	}

	return ReturnValue;
}

bool ( *HasInfiniteConsumablesOrignal )( UObject* );
bool HasInfiniteConsumables( UObject* This )
{
	bool ReturnValue = HasInfiniteConsumablesOrignal( This );

	if ( Settings::Exploits::InfiniteAmmo )
	{
		return true;
	}

	return ReturnValue;
}

bool ( *HasInfiniteMagazineAmmoOrignal )( UObject* );
bool HasInfiniteMagazineAmmo( UObject* This )
{
	bool ReturnValue = HasInfiniteMagazineAmmoOrignal( This );

	if ( Settings::Exploits::InfiniteAmmo )
	{
		return true;
	}

	return ReturnValue;
}

bool ( *HasInfiniteReserveAmmoOrignal )( UObject* , UFortItemDefinition* );
bool HasInfiniteReserveAmmo( UObject* This , UFortItemDefinition* WeaponData )
{
	bool ReturnValue = HasInfiniteReserveAmmoOrignal( This , WeaponData );

	if ( Settings::Exploits::InfiniteAmmo )
	{
		return true;
	}

	return ReturnValue;
}

bool ( *InventoryOwnerHasInfiniteAmmoOrignal )( UObject* , UFortItemDefinition* );
bool InventoryOwnerHasInfiniteAmmo( UObject* This , UFortItemDefinition* WeaponData )
{
	bool ReturnValue = InventoryOwnerHasInfiniteAmmoOrignal( This , WeaponData );

	if ( Settings::Exploits::InfiniteAmmo )
	{
		return true;
	}

	return ReturnValue;
}

bool ( *UsesConsumableAmmoOrignal )( UObject* , UFortItemDefinition* );
bool UsesConsumableAmmo( UObject* This , UFortItemDefinition* WeaponData )
{
	bool ReturnValue = UsesConsumableAmmoOrignal( This , WeaponData );

	if ( Settings::Exploits::InfiniteAmmo )
	{
		return true;
	}

	return ReturnValue;
}

bool ( *UsesEnergyPoolForAmmoOrignal )( UObject* , UFortItemDefinition* );
bool UsesEnergyPoolForAmmo( UObject* This , UFortItemDefinition* WeaponData )
{
	bool ReturnValue = UsesEnergyPoolForAmmoOrignal( This , WeaponData );

	if ( Settings::Exploits::InfiniteAmmo )
	{
		return true;
	}

	return ReturnValue;
}

bool ( *UsesChargesForAmmoOrignal )( UObject* , UFortItemDefinition* );
bool UsesChargesForAmmo( UObject* This , UFortItemDefinition* WeaponData )
{
	bool ReturnValue = UsesChargesForAmmoOrignal( This , WeaponData );

	if ( Settings::Exploits::InfiniteAmmo )
	{
		return true;
	}

	return ReturnValue;
}