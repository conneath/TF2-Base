//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef TF_WEAPON_BONESAW_H
#define TF_WEAPON_BONESAW_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_weaponbase_melee.h"

#ifdef CLIENT_DLL
#define CTFBonesaw C_TFBonesaw
#endif

//=============================================================================
//
// Bonesaw class.
//
class CTFBonesaw : public CTFWeaponBaseMelee
{
public:

	DECLARE_CLASS( CTFBonesaw, CTFWeaponBaseMelee );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CTFBonesaw() {}
	virtual int			GetWeaponID( void ) const			{ return TF_WEAPON_BONESAW; }

#ifdef CLIENT_DLL // Ubersaw pose parameter stuff
	virtual void		OnDataChanged( DataUpdateType_t updateType );
	void				UpdateChargePoseParam( void );
	virtual void		GetPoseParameters( CStudioHdr* pStudioHdr, float poseParameter[MAXSTUDIOPOSEPARAM] );
	virtual void		UpdateAttachmentModels( void );
#endif

private:

#ifdef CLIENT_DLL
	int			m_iUberChargePoseParam;
	float		m_flChargeLevel;
#endif

	CTFBonesaw( const CTFBonesaw & ) {}
};

#endif // TF_WEAPON_BONESAW_H
