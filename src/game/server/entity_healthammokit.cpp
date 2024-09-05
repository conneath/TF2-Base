//======= Copywrong 2024, Conneath Industries, No rights reserved, lol. =======//
//
// Purpose: CTF Health + ammo kit.
//
//=============================================================================//
#include "cbase.h"
#include "items.h"
#include "tf_gamerules.h"
#include "tf_shareddefs.h"
#include "tf_player.h"
#include "tf_team.h"
#include "engine/IEngineSound.h"
#include "entity_healthammokit.h"

//=============================================================================
//
// CTF HealthKit defines.
//

#define TF_HEALTHKIT_MODEL			"models/items/healthammokit.mdl"
#define TF_HEALTHKIT_PICKUP_SOUND	"HealthKit.Touch"
#define TF_AMMOPACK_PICKUP_SOUND	"AmmoPack.Touch"

LINK_ENTITY_TO_CLASS(item_healthammokit, CHealthAmmoKit);

//=============================================================================
//
// CTF HealthKit functions.
//

//-----------------------------------------------------------------------------
// Purpose: Spawn function for the healthkit
//-----------------------------------------------------------------------------
void CHealthAmmoKit::Spawn(void)
{
	Precache();
	SetModel(GetPowerupModel());

	BaseClass::Spawn();
}

//-----------------------------------------------------------------------------
// Purpose: Precache function for the healthkit
//-----------------------------------------------------------------------------
void CHealthAmmoKit::Precache(void)
{
	PrecacheModel(GetPowerupModel());
	PrecacheScriptSound(TF_HEALTHKIT_PICKUP_SOUND);
}

//-----------------------------------------------------------------------------
// Purpose: MyTouch function for the healthkit
//-----------------------------------------------------------------------------
bool CHealthAmmoKit::MyTouch(CBasePlayer* pPlayer)
{
	bool bSuccess = false;

	// HEALTH

	if (ValidTouch(pPlayer))
	{
		if (pPlayer->TakeHealth(ceil(pPlayer->GetMaxHealth() * PackRatios[GetPowerupSize()]), DMG_GENERIC))
		{
			CSingleUserRecipientFilter user(pPlayer);
			user.MakeReliable();

			UserMessageBegin(user, "ItemPickup");
			WRITE_STRING(GetClassname());
			MessageEnd();

			EmitSound(user, entindex(), TF_HEALTHKIT_PICKUP_SOUND);

			bSuccess = true;

			CTFPlayer* pTFPlayer = ToTFPlayer(pPlayer);

			Assert(pTFPlayer);

			// Healthkits also contain a fire blanket.
			if (pTFPlayer->m_Shared.InCond(TF_COND_BURNING))
			{
				pTFPlayer->m_Shared.RemoveCond(TF_COND_BURNING);
			}
		}

		// AMMO

		CTFPlayer* pTFPlayer = ToTFPlayer(pPlayer);
		if (!pTFPlayer)
			return false;

		int iMaxPrimary = pTFPlayer->GetPlayerClass()->GetData()->m_aAmmoMax[TF_AMMO_PRIMARY];
		if (pPlayer->GiveAmmo(ceil(iMaxPrimary * PackRatios[GetPowerupSize()]), TF_AMMO_PRIMARY, true))
		{
			bSuccess = true;
		}

		int iMaxSecondary = pTFPlayer->GetPlayerClass()->GetData()->m_aAmmoMax[TF_AMMO_SECONDARY];
		if (pPlayer->GiveAmmo(ceil(iMaxSecondary * PackRatios[GetPowerupSize()]), TF_AMMO_SECONDARY, true))
		{
			bSuccess = true;
		}

		int iMaxMetal = pTFPlayer->GetPlayerClass()->GetData()->m_aAmmoMax[TF_AMMO_METAL];
		if (pPlayer->GiveAmmo(ceil(iMaxMetal * PackRatios[GetPowerupSize()]), TF_AMMO_METAL, true))
		{
			bSuccess = true;
		}

		// did we give them anything?
		if (bSuccess)
		{
			CSingleUserRecipientFilter filter(pPlayer);
			EmitSound(filter, entindex(), TF_AMMOPACK_PICKUP_SOUND);
		}
	}

	return bSuccess;
}