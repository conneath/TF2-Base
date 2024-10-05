//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef TF_VIEWMODEL_H
#define TF_VIEWMODEL_H
#ifdef _WIN32
#pragma once
#endif

#include "predictable_entity.h"
#include "utlvector.h"
#include "baseplayer_shared.h"
#include "shared_classnames.h"
#include "tf_weaponbase.h"

#if defined( CLIENT_DLL )
#define CTFViewModel C_TFViewModel
#endif


#ifdef CLIENT_DLL
class C_TFViewModel; // stupid

class C_ViewmodelAttachmentModel : public C_BaseViewModel
{
	DECLARE_CLASS( C_ViewmodelAttachmentModel, C_BaseViewModel );
public:

	virtual bool InitializeAsClientEntity( const char* pszModelName, RenderGroup_t renderGroup );

	virtual int	InternalDrawModel( int flags );
	bool OnPostInternalDrawModel( ClientModelRenderInfo_t* pInfo );
	virtual int	DrawModel( int flags );
	virtual int DrawOverriddenViewmodel( int flags );

	void SetViewmodel( C_TFViewModel* vm );

	virtual bool			IsViewModel() const { return true; }
	virtual RenderGroup_t	GetRenderGroup( void ) { return RENDER_GROUP_VIEW_MODEL_TRANSLUCENT; }

	virtual C_BaseEntity* GetItemTintColorOwner( void ) { return GetOwner(); }

private:
	CHandle< C_TFViewModel > m_viewmodel;
};
#endif

class CTFViewModel : public CBaseViewModel
{
	DECLARE_CLASS( CTFViewModel, CBaseViewModel );
public:

	DECLARE_NETWORKCLASS();

	CTFViewModel( void );
	virtual ~CTFViewModel( void );

	virtual void CalcViewModelLag( Vector& origin, QAngle& angles, QAngle& original_angles );
	virtual void CalcViewModelView( CBasePlayer *owner, const Vector& eyePosition, const QAngle& eyeAngles );
	virtual void AddViewModelBob( CBasePlayer *owner, Vector& eyePosition, QAngle& eyeAngles );

	virtual void SetWeaponModel( const char* pszModelname, CBaseCombatWeapon* weapon );

	int GetViewModelAttach( void ) { return m_bAttachToHands; }
	void SetViewModelAttach( bool bType ) { this->m_bAttachToHands = bType; }

#if defined( CLIENT_DLL )
	virtual bool ShouldPredict( void )
	{
		if ( GetOwner() && GetOwner() == C_BasePlayer::GetLocalPlayer() )
			return true;

		return BaseClass::ShouldPredict();
	}

	virtual void StandardBlendingRules( CStudioHdr *hdr, Vector pos[], Quaternion q[], float currentTime, int boneMask );
	virtual void ProcessMuzzleFlashEvent( void );

	virtual int GetSkin();
	BobState_t	&GetBobState() { return m_BobState; }

	virtual int DrawModel( int flags );
	virtual bool OnPostInternalDrawModel( ClientModelRenderInfo_t* pInfo );

	CHandle< C_ViewmodelAttachmentModel > m_hViewmodelAddon;
	//CHandle< C_ViewmodelAttachmentModel > m_hWeaponAttachment; // read from items_game "attached_model" (e.g. for Kritzkrieg)
	void UpdateViewmodelAddon( const char* pszModelname );

	void RemoveViewmodelAddon( void );

	// Attachments
	virtual int				LookupAttachment( const char* pAttachmentName );
	virtual bool			GetAttachment( int number, matrix3x4_t& matrix );
	virtual bool			GetAttachment( int number, Vector& origin );
	virtual	bool			GetAttachment( int number, Vector& origin, QAngle& angles );
	virtual bool			GetAttachmentVelocity( int number, Vector& originVel, Quaternion& angleVel );
#endif

private:

#if defined( CLIENT_DLL )

	// This is used to lag the angles.
	CInterpolatedVar<QAngle> m_LagAnglesHistory;
	QAngle m_vLagAngles;
	BobState_t		m_BobState;		// view model head bob state

	CTFViewModel( const CTFViewModel & ); // not defined, not accessible

	QAngle m_vLoweredWeaponOffset;

#endif

	bool m_bAttachToHands;
};

#endif // TF_VIEWMODEL_H
