//================================================================
// Goldrush Item Schema - powered by conneath industries ltd co tm
// credit to the tf2c team for making the original econ system
// that every tf2 mod seems to use these days (including us)
//================================================================
#include "cbase.h"
#include "tf_item_schema.h"
#include "tf_item_system.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//=============================================================================
// CTFItemAttribute
//=============================================================================


BEGIN_NETWORK_TABLE_NOBASE( CTFItemAttribute, DT_TFItemAttribute )
#ifdef CLIENT_DLL
RecvPropInt( RECVINFO( m_iAttributeDefinitionIndex ) ),
RecvPropFloat( RECVINFO( value ) ),
RecvPropString( RECVINFO( value_string ) ),
RecvPropString( RECVINFO( attribute_class ) ),
#else
SendPropInt( SENDINFO( m_iAttributeDefinitionIndex ) ),
SendPropFloat( SENDINFO( value ) ),
SendPropString( SENDINFO( value_string ) ),
SendPropString( SENDINFO( attribute_class ) ),
#endif
END_NETWORK_TABLE()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFItemAttribute::Init( int iIndex, float flValue, const char* pszAttributeClass /*= NULL*/ )
{
	m_iAttributeDefinitionIndex = iIndex;
	value = flValue;
	value_string.GetForModify()[0] = '\0';

	if ( pszAttributeClass )
	{
		V_strncpy( attribute_class.GetForModify(), pszAttributeClass, sizeof( attribute_class ) );
	}
	else
	{
		TFAttributeDefinition* pAttribDef = GetStaticData();
		if ( pAttribDef )
		{
			V_strncpy( attribute_class.GetForModify(), pAttribDef->attribute_class, sizeof( attribute_class ) );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTFItemAttribute::Init( int iIndex, const char* pszValue, const char* pszAttributeClass /*= NULL*/ )
{
	m_iAttributeDefinitionIndex = iIndex;
	value = 0.0f;
	V_strncpy( value_string.GetForModify(), pszValue, sizeof( value_string ) );

	if ( pszAttributeClass )
	{
		V_strncpy( attribute_class.GetForModify(), pszAttributeClass, sizeof( attribute_class ) );
	}
	else
	{
		TFAttributeDefinition* pAttribDef = GetStaticData();
		if ( pAttribDef )
		{
			V_strncpy( attribute_class.GetForModify(), pAttribDef->attribute_class, sizeof( attribute_class ) );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
TFAttributeDefinition* CTFItemAttribute::GetStaticData( void )
{
	return GetItemSchema()->GetAttributeDefinition( m_iAttributeDefinitionIndex );
}


//=============================================================================
// CTFItemDefinition
//=============================================================================

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
TFItemVisuals* CTFItemDefinition::GetVisuals( int iTeamNum /*= TEAM_UNASSIGNED*/ )
{
	if ( iTeamNum > LAST_SHARED_TEAM && iTeamNum < TF_TEAM_COUNT )
	{
		return &visual[iTeamNum];
	}

	return &visual[TEAM_UNASSIGNED];
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CTFItemDefinition::GetLoadoutSlot( int iClass /*= TF_CLASS_UNDEFINED*/ )
{
	if ( iClass && item_slot_per_class[iClass] != -1 )
	{
		return item_slot_per_class[iClass];
	}

	return item_slot;
}

//-----------------------------------------------------------------------------
// Purpose: Find an attribute with the specified class.
//-----------------------------------------------------------------------------
CTFItemAttribute* CTFItemDefinition::IterateAttributes( string_t strClass )
{
	// Returning the first attribute found.
	for ( int i = 0; i < attributes.Count(); i++ )
	{
		CTFItemAttribute* pAttribute = &attributes[i];

		if ( pAttribute->m_strAttributeClass == strClass )
		{
			return pAttribute;
		}
	}

	return NULL;
}