// © 2024 Marco Silva. All Rights Reserved.


#include "Data/WeaponData.h"

float UWeaponData::GetEffectiveRate() const
{
	return FireRate + (BurstAmount * BurstRate);
}