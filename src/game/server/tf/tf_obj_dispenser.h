//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Engineer's Dispenser
//
// $NoKeywords: $
//=============================================================================//

#ifndef TF_OBJ_DISPENSER_H
#define TF_OBJ_DISPENSER_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_obj.h"

class CTFPlayer;

// ------------------------------------------------------------------------ //
// Resupply object that's built by the player
// ------------------------------------------------------------------------ //
class CObjectDispenser : public CBaseObject
{
	DECLARE_CLASS( CObjectDispenser, CBaseObject );

public:
	DECLARE_SERVERCLASS();

	CObjectDispenser();
	~CObjectDispenser();

	static CObjectDispenser* Create(const Vector &vOrigin, const QAngle &vAngles);

	virtual void	Spawn();
	virtual void	GetControlPanelInfo( int nPanelIndex, const char *&pPanelName );
	virtual void	Precache();
	virtual bool	ClientCommand( CTFPlayer *pPlayer, const CCommand &args );

	virtual void	DetonateObject( void );
	virtual void	OnGoActive( void );	
	virtual bool	StartBuilding( CBaseEntity *pBuilder );
	virtual int		DrawDebugTextOverlays(void) ;
	virtual void	SetModel( const char *pModel );

	virtual int		GetAvailableMetal( void ) const;

	void RefillThink( void );
	void DispenseThink( void );

	virtual void StartTouch( CBaseEntity *pOther );
	virtual void EndTouch( CBaseEntity *pOther );

	virtual int	ObjectCaps( void ) { return (BaseClass::ObjectCaps() | FCAP_IMPULSE_USE); }

	bool DispenseAmmo( CTFPlayer *pPlayer );

	void StartHealing( CBaseEntity *pOther );
	void StopHealing( CBaseEntity *pOther );

	void AddHealingTarget( CBaseEntity *pOther );
	void RemoveHealingTarget( CBaseEntity *pOther );
	bool IsHealingTarget( CBaseEntity *pTarget );

	bool CouldHealTarget( CBaseEntity *pTarget );

	Vector GetHealOrigin( void );

	CUtlVector< EHANDLE >	m_hHealingTargets;

	virtual void	MakeCarriedObject( CTFPlayer* pCarrier );

protected:
	bool m_bUseGenerateMetalSound;

	CNetworkVar( int, m_iAmmoMetal );

private:
	void ResetHealingTargets( void );

	//CNetworkArray( EHANDLE, m_hHealingTargets, MAX_DISPENSER_HEALING_TARGETS );

	// Entities currently being touched by this trigger
	CUtlVector< EHANDLE >	m_hTouchingEntities;

	float m_flNextAmmoDispense;

	string_t m_iszCustomTouchTrigger;
	EHANDLE m_hTouchTrigger;

	DECLARE_DATADESC();
};

inline int CObjectDispenser::GetAvailableMetal( void ) const
{
	return m_iAmmoMetal;
}


//------------------------------------------------------------------------------
class CObjectCartDispenser : public CObjectDispenser
{
	DECLARE_CLASS( CObjectCartDispenser, CObjectDispenser );

public:
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	CObjectCartDispenser();

	virtual void Spawn( void );
	virtual void OnGoActive( void );
	virtual void GetControlPanelInfo( int nPanelIndex, const char*& pPanelName );

	virtual int	DispenseMetal( CTFPlayer* pPlayer );

	virtual bool	CanBeUpgraded( CTFPlayer* pPlayer ) { return false; }
	virtual void	SetModel( const char* pModel );

	//void InputSetDispenserLevel( inputdata_t& inputdata );
	void InputEnable( inputdata_t& inputdata );
	void InputDisable( inputdata_t& inputdata );
};
#endif // TF_OBJ_DISPENSER_H
