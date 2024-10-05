//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//===========================================================================//

#ifndef ECON_ENTITY_H
#define ECON_ENTITY_H

#ifdef _WIN32
#pragma once
#endif

#if defined( CLIENT_DLL )
#define CEconEntity C_EconEntity
#endif

#include "ihasattributes.h"
#include "tf_item_view.h"
#include "tf_attribute_manager.h"

struct wearableanimplayback_t
{
	int iStub;
};

// Additional attachments.
struct AttachedModelData_t
{
	const model_t* m_pModel;
	int m_iModelDisplayFlags;
};

//-----------------------------------------------------------------------------
// Purpose: BaseCombatWeapon is derived from this in live tf2.
//-----------------------------------------------------------------------------
class CEconEntity : public CBaseAnimating, public IHasAttributes
{
	DECLARE_CLASS( CEconEntity, CBaseAnimating );
	DECLARE_NETWORKCLASS();

#ifdef CLIENT_DLL
	DECLARE_PREDICTABLE();
#endif

public:
	CEconEntity();
	~CEconEntity();

#ifdef CLIENT_DLL
	virtual void OnPreDataChanged( DataUpdateType_t );
	virtual void OnDataChanged( DataUpdateType_t );
	virtual void FireEvent( const Vector& origin, const QAngle& angles, int event, const char* options );
	virtual bool OnFireEvent( C_BaseViewModel* pViewModel, const Vector& origin, const QAngle& angles, int event, const char* options );
#endif

	virtual int TranslateViewmodelHandActivity( int iActivity ) { return iActivity; }

	virtual void PlayAnimForPlaybackEvent( wearableanimplayback_t iPlayback ) {};

	virtual void SetItem( CEconItemView& newItem );
	CEconItemView* GetItem( void );
	virtual bool HasItemDefinition( void ) const;
	virtual int GetItemID( void );

	virtual void GiveTo( CBaseEntity* pEntity );

	virtual CAttributeManager* GetAttributeManager( void ) { return &m_AttributeManager; }
	virtual CAttributeContainer* GetAttributeContainer( void ) { return &m_AttributeManager; }
	virtual CBaseEntity* GetAttributeOwner( void ) { return NULL; }
	virtual void ReapplyProvision( void );

	void UpdatePlayerModelToClass( void );

	void UpdateAttachmentModels( void );

	virtual void UpdateOnRemove( void );

#ifdef CLIENT_DLL
	CUtlVector<AttachedModelData_t> m_vecAttachedModels;
#endif
protected:
	EHANDLE m_hOldOwner;
	CEconItemView m_Item;

private:
	CNetworkVarEmbedded( CAttributeContainer, m_AttributeManager );
};

#endif
