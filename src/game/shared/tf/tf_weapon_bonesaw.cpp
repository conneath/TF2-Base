//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "cbase.h"
#include "tf_weapon_bonesaw.h"
#ifndef GAME_DLL
#include "c_tf_player.h"
#include "tf_viewmodel.h"
#include "tf_weapon_medigun.h"
#endif
#define UBERSAW_CHARGE_POSEPARAM		"syringe_charge_level"

//=============================================================================
//
// Weapon Bonesaw tables.
//

IMPLEMENT_NETWORKCLASS_ALIASED( TFBonesaw, DT_TFWeaponBonesaw )

BEGIN_NETWORK_TABLE( CTFBonesaw, DT_TFWeaponBonesaw )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CTFBonesaw )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( tf_weapon_bonesaw, CTFBonesaw );
PRECACHE_WEAPON_REGISTER( tf_weapon_bonesaw );

#ifdef CLIENT_DLL
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFBonesaw::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );

	UpdateChargePoseParam();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFBonesaw::UpdateAttachmentModels( void )
{
	BaseClass::UpdateAttachmentModels();

	CTFPlayer* pPlayer = ToTFPlayer( GetPlayerOwner() );
	CTFViewModel* viewmodel = nullptr;
	// this should just be under a helper function tbh
	// in live we had m_hViewmodelAttachment which is part of the econ entity, but here the weapon attachment is just part of CTFViewModel
	if(	pPlayer )
		 viewmodel = dynamic_cast<CTFViewModel*>(pPlayer->GetViewModel());

	// BUG: this wont work if you have a bonesaw that doesn't use attach_to_hands... but that's not going to happen right?
	if ( viewmodel && viewmodel->m_hViewmodelAddon )
	{
		m_iUberChargePoseParam = viewmodel->m_hViewmodelAddon->LookupPoseParameter( viewmodel->m_hViewmodelAddon->GetModelPtr(), UBERSAW_CHARGE_POSEPARAM );
	}
	else
	{
		m_iUberChargePoseParam = LookupPoseParameter( GetModelPtr(), UBERSAW_CHARGE_POSEPARAM );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFBonesaw::UpdateChargePoseParam( void )
{
	if ( m_iUberChargePoseParam >= 0 )
	{
		CTFPlayer* pTFPlayer = ToTFPlayer( GetOwner() );
		if ( pTFPlayer && pTFPlayer->IsPlayerClass( TF_CLASS_MEDIC ) )
		{
			CWeaponMedigun* pMedigun = (CWeaponMedigun*)pTFPlayer->Weapon_OwnsThisID( TF_WEAPON_MEDIGUN );
			if ( pMedigun )
			{
				m_flChargeLevel = pMedigun->GetChargeLevel();

				CTFViewModel* viewmodel = nullptr;
				if ( pTFPlayer )
					viewmodel = dynamic_cast<CTFViewModel*>(pTFPlayer->GetViewModel());

				// On the local client, we push the pose parameters onto the attached model
				if ( viewmodel && viewmodel->m_hViewmodelAddon )
				{
					viewmodel->m_hViewmodelAddon->SetPoseParameter( m_iUberChargePoseParam, pMedigun->GetChargeLevel() );
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFBonesaw::GetPoseParameters( CStudioHdr* pStudioHdr, float poseParameter[MAXSTUDIOPOSEPARAM] )
{
	if ( !pStudioHdr )
		return;

	BaseClass::GetPoseParameters( pStudioHdr, poseParameter );

	if ( m_iUberChargePoseParam >= 0 )
	{
		poseParameter[m_iUberChargePoseParam] = m_flChargeLevel;
	}
}

#endif