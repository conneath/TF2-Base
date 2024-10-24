//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//===========================================================================//

#include "cbase.h"
#include "tf_econ_entity.h"
#include "eventlist.h"

#ifdef CLIENT_DLL
#include "model_types.h"

#include "tf_weaponbase.h"
#include "tf_viewmodel.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IMPLEMENT_NETWORKCLASS_ALIASED( EconEntity, DT_EconEntity )

BEGIN_NETWORK_TABLE( CEconEntity, DT_EconEntity )
#ifdef CLIENT_DLL
RecvPropDataTable( RECVINFO_DT( m_Item ), 0, &REFERENCE_RECV_TABLE( DT_ScriptCreatedItem ) ),
RecvPropDataTable( RECVINFO_DT( m_AttributeManager ), 0, &REFERENCE_RECV_TABLE( DT_AttributeContainer ) ),
#else
SendPropDataTable( SENDINFO_DT( m_Item ), &REFERENCE_SEND_TABLE( DT_ScriptCreatedItem ) ),
SendPropDataTable( SENDINFO_DT( m_AttributeManager ), &REFERENCE_SEND_TABLE( DT_AttributeContainer ) ),
#endif
END_NETWORK_TABLE()

#ifdef CLIENT_DLL
BEGIN_PREDICTION_DATA( C_EconEntity )
DEFINE_PRED_TYPEDESCRIPTION( m_AttributeManager, CAttributeContainer ),
END_PREDICTION_DATA()
#endif

CEconEntity::CEconEntity()
{
	m_pAttributes = this;
}

CEconEntity::~CEconEntity()
{
}

#ifdef CLIENT_DLL

//-----------------------------------------------------------------------------
// Purpose: Valve hack from live tf2 for drawing weapon attachments (e.g. the Kritzkrieg)
//-----------------------------------------------------------------------------
void DrawEconEntityAttachedModels( CBaseAnimating* pEnt, CEconEntity* pAttachedModelSource, const ClientModelRenderInfo_t* pInfo, int iMatchDisplayFlags )
{
	if ( !pEnt || !pAttachedModelSource || !pInfo )
		return;

	// Draw our attached models
	for ( int i = 0; i < pAttachedModelSource->m_vecAttachedModels.Size(); i++ )
	{
		const AttachedModelData_t& attachedModel = pAttachedModelSource->m_vecAttachedModels[i];

		if ( attachedModel.m_pModel && (attachedModel.m_iModelDisplayFlags & iMatchDisplayFlags) )
		{
			ClientModelRenderInfo_t infoAttached = *pInfo;

			infoAttached.pRenderable = pEnt;
			infoAttached.instance = MODEL_INSTANCE_INVALID;
			infoAttached.entity_index = pEnt->index;
			infoAttached.pModel = attachedModel.m_pModel;
			infoAttached.pModelToWorld = &infoAttached.modelToWorld;

			// Turns the origin + angles into a matrix
			AngleMatrix( infoAttached.angles, infoAttached.origin, infoAttached.modelToWorld );

			DrawModelState_t state;
			matrix3x4_t* pBoneToWorld;
			bool bMarkAsDrawn = modelrender->DrawModelSetup( infoAttached, &state, NULL, &pBoneToWorld );
			pEnt->DoInternalDrawModel( &infoAttached, (bMarkAsDrawn && (infoAttached.flags & STUDIO_RENDER)) ? &state : NULL, pBoneToWorld );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CEconEntity::OnPreDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnPreDataChanged( updateType );

	m_AttributeManager.OnPreDataChanged( updateType );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CEconEntity::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );

	m_AttributeManager.OnDataChanged( updateType );

	UpdateAttachmentModels();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CEconEntity::FireEvent( const Vector& origin, const QAngle& angles, int event, const char* options )
{
	if ( event == AE_CL_BODYGROUP_SET_VALUE_CMODEL_WPN )
	{
		// Something?
	}
	else
		BaseClass::FireEvent( origin, angles, event, options );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CEconEntity::OnFireEvent( C_BaseViewModel* pViewModel, const Vector& origin, const QAngle& angles, int event, const char* options )
{
	if ( event == AE_CL_BODYGROUP_SET_VALUE_CMODEL_WPN )
	{
		return true;
	}
	return false;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CEconEntity::UpdateAttachmentModels( void )
{
	CEconItemView* pItem = GetItem();
	CEconItemDefinition* pItemDef = pItem->GetStaticData();

	// Update the state of additional model attachments
	m_vecAttachedModels.Purge();
	if ( pItemDef /* && AttachmentModelsShouldBeVisible()*/ )
	{
		//int iTeamNumber = GetTeamNumber();
		{
			//int iAttachedModels = pItemDef->GetVisuals()->attached_models.Count();
			//for ( int i = 0; i < iAttachedModels; i++ )
			{
				//attachedmodel_t* pModel = &pItemDef->GetVisuals(TF_TEAM_RED)->attached_models[i];
				//const char* pszModelName = pItemDef->GetVisuals()->attached_models[i];
				const char* pszModelName = pItemDef->model_attachment;
				int iModelIndex = modelinfo->GetModelIndex( pszModelName );
				if ( iModelIndex >= 0 )
				{
					AttachedModelData_t attachedModelData;
					attachedModelData.m_pModel = modelinfo->GetModel( iModelIndex );
					attachedModelData.m_iModelDisplayFlags = /*pModel->m_iModelDisplayFlags*/ 0x03;
					m_vecAttachedModels.AddToTail( attachedModelData );
				}
			}
		}
	}
}

#endif

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CEconEntity::SetItem( CEconItemView& newItem )
{
	m_Item = newItem;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CEconItemView* CEconEntity::GetItem( void )
{
	return &m_Item;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CEconEntity::HasItemDefinition( void ) const
{
	return (m_Item.GetItemDefIndex() >= 0);
}

//-----------------------------------------------------------------------------
// Purpose: Shortcut to get item ID.
//-----------------------------------------------------------------------------
int CEconEntity::GetItemID( void )
{
	return m_Item.GetItemDefIndex();
}

//-----------------------------------------------------------------------------
// Purpose: Derived classes need to override this.
//-----------------------------------------------------------------------------
void CEconEntity::GiveTo( CBaseEntity* pEntity )
{
}

//-----------------------------------------------------------------------------
// Purpose: Add or remove this from owner's attribute providers list.
//-----------------------------------------------------------------------------
void CEconEntity::ReapplyProvision( void )
{
	CBaseEntity* pOwner = GetOwnerEntity();
	CBaseEntity* pOldOwner = m_hOldOwner.Get();

	if ( pOwner != pOldOwner )
	{
		if ( pOldOwner )
		{
			m_AttributeManager.StopProvidingTo( pOldOwner );
		}

		if ( pOwner )
		{
			m_AttributeManager.ProviteTo( pOwner );
			m_hOldOwner = pOwner;
		}
		else
		{
			m_hOldOwner = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CEconEntity::UpdateOnRemove( void )
{
	SetOwnerEntity( NULL );
	ReapplyProvision();
	BaseClass::UpdateOnRemove();
}
