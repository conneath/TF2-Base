//================================================================
// Goldrush Item Schema - powered by conneath industries ltd co tm
// credit to the tf2c team for making the original econ system
// that every tf2 mod seems to use these days (including us)
//================================================================
#include "cbase.h"
#include "tf_item_system.h"
#include "script_parser.h"
#include "activitylist.h"
#include "tf_item_schema_parser.h"
#include "gamestringpool.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"
//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
static CTFItemSchema g_TFItemSchema;
static CTFSchemaParser g_TFSchemaParser;

CTFItemSchema* GetItemSchema()
{
	return &g_TFItemSchema;
}

//-----------------------------------------------------------------------------
// Purpose: constructor
//-----------------------------------------------------------------------------
CTFItemSchema::CTFItemSchema()
{
	SetDefLessFunc( m_Items );
	SetDefLessFunc( m_Attributes );

	m_bInited = false;
}

CTFItemSchema::~CTFItemSchema()
{
}

//-----------------------------------------------------------------------------
// Purpose: Initializer
//-----------------------------------------------------------------------------
bool CTFItemSchema::Init( void )
{
	if ( !m_bInited )
	{
		// Must register activities early so we can parse animation replacements.
		ActivityList_Free();
		ActivityList_RegisterSharedActivities();

		float flStartTime = engine->Time();
		g_TFSchemaParser.InitParser( "scripts/items/items_game.txt", true, false );
		float flEndTime = engine->Time();
		Msg( "[%s] Processing item schema took %.02fms. Parsed %d items and %d attributes.\n",
			CBaseEntity::IsServer() ? "SERVER" : "CLIENT",
			(flEndTime - flStartTime) * 1000.0f,
			m_Items.Count(),
			m_Attributes.Count() );

		m_bInited = true;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Runs on level start, precaches models and sounds from schema.
//-----------------------------------------------------------------------------
void CTFItemSchema::Precache( void )
{
	string_t strPrecacheAttribute = AllocPooledString( "custom_projectile_model" );

	// Precache everything from schema.
	FOR_EACH_MAP( m_Items, i )
	{
		CTFItemDefinition* pItem = m_Items[i];

		// Precache models.
		if ( pItem->model_world[0] != '\0' )
			CBaseEntity::PrecacheModel( pItem->model_world );

		if ( pItem->model_player[0] != '\0' )
			CBaseEntity::PrecacheModel( pItem->model_player );

		for ( int iClass = 0; iClass < TF_CLASS_COUNT_ALL; iClass++ )
		{
			const char* pszModel = pItem->model_player_per_class[iClass];
			if ( pszModel[0] != '\0' )
				CBaseEntity::PrecacheModel( pszModel );
		}

		// Precache visuals.
		for ( int i = 0; i < TF_TEAM_COUNT; i++ )
		{
			if ( i == TEAM_SPECTATOR )
				continue;

			TFItemVisuals* pVisuals = &pItem->visual[i];

			// Precache sounds.
			for ( int i = 0; i < NUM_SHOOT_SOUND_TYPES; i++ )
			{
				if ( pVisuals->aWeaponSounds[i][0] != '\0' )
					CBaseEntity::PrecacheScriptSound( pVisuals->aWeaponSounds[i] );
			}
		}

		// Cache all attrbute names.
		for ( int i = 0; i < pItem->attributes.Count(); i++ )
		{
			CTFItemAttribute* pAttribute = &pItem->attributes[i];
			pAttribute->m_strAttributeClass = AllocPooledString( pAttribute->attribute_class );

			// Special case for custom_projectile_model attribute.
			if ( pAttribute->m_strAttributeClass == strPrecacheAttribute )
			{
				CBaseEntity::PrecacheModel( pAttribute->value_string.Get() );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CTFItemDefinition* CTFItemSchema::GetItemDefinition( int id )
{
	if ( id < 0 )
		return NULL;

	CTFItemDefinition* itemdef = NULL;
	FIND_ELEMENT( m_Items, id, itemdef );
	return itemdef;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
TFAttributeDefinition* CTFItemSchema::GetAttributeDefinition( int id )
{
	if ( id < 0 )
		return NULL;

	TFAttributeDefinition* attribdef = NULL;
	FIND_ELEMENT( m_Attributes, id, attribdef );
	return attribdef;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
TFAttributeDefinition* CTFItemSchema::GetAttributeDefinitionByName( const char* name )
{
	//unsigned int index = m_Attributes.Find(name);
	//if (index < m_Attributes.Count())
	//{
	//	return &m_Attributes[index];
	//}
	FOR_EACH_MAP_FAST( m_Attributes, i )
	{
		if ( !V_stricmp( m_Attributes[i]->name, name ) )
		{
			return m_Attributes[i];
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
TFAttributeDefinition* CTFItemSchema::GetAttributeDefinitionByClass( const char* classname )
{
	//unsigned int index = m_Attributes.Find(name);
	//if (index < m_Attributes.Count())
	//{
	//	return &m_Attributes[index];
	//}
	FOR_EACH_MAP_FAST( m_Attributes, i )
	{
		if ( !V_stricmp( m_Attributes[i]->attribute_class, classname ) )
		{
			return m_Attributes[i];
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CTFItemSchema::GetAttributeIndex( const char* name )
{
	if ( !name )
		return -1;

	FOR_EACH_MAP_FAST( m_Attributes, i )
	{
		if ( !V_stricmp( m_Attributes[i]->name, name ) )
		{
			return m_Attributes.Key( i );
		}
	}

	return -1;
}