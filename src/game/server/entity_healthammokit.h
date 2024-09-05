///======= Copywrong 2024, Conneath Industries, No rights reserved, lol. =======//
//
// Purpose: CTF HealthKit.
//
//=============================================================================//
#ifndef ENTITY_HEALTHKIT_H
#define ENTITY_HEALTHKIT_H

#ifdef _WIN32
#pragma once
#endif

#include "tf_powerup.h"

//=============================================================================
//
// CTF HealthKit class.
//

class CHealthAmmoKit : public CTFPowerup
{
public:
	DECLARE_CLASS( CHealthAmmoKit, CTFPowerup );

	void	Spawn( void );
	void	Precache( void );
	bool	MyTouch( CBasePlayer *pPlayer );

	virtual const char *GetPowerupModel( void ) { return "models/items/healthammokit.mdl"; }

	powerupsize_t	GetPowerupSize( void ) { return POWERUP_SMALL; }
};

#endif // ENTITY_HEALTHKIT_H


