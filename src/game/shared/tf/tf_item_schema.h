//================================================================
// Goldrush Item Schema - powered by conneath industries ltd co tm
// credit to the tf2c team for making the original econ system
// that every tf2 mod seems to use these days (including us)
//================================================================
#ifndef TF_ITEM_SCHEMA_H
#define TF_ITEM_SCHEMA_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_shareddefs.h"
#include "networkvar.h"
#include "utlvector.h"
#include "utldict.h"
#include "weapon_parse.h"
#include "string_t.h"

enum
{
	ATTRIB_FORMAT_INVALID = -1,
	ATTRIB_FORMAT_PERCENTAGE = 0,
	ATTRIB_FORMAT_INVERTED_PERCENTAGE,
	ATTRIB_FORMAT_ADDITIVE,
	ATTRIB_FORMAT_ADDITIVE_PERCENTAGE,
	ATTRIB_FORMAT_OR,
};

enum
{
	ATTRIB_EFFECT_INVALID = -1,
	ATTRIB_EFFECT_UNUSUAL = 0,
	ATTRIB_EFFECT_STRANGE,
	ATTRIB_EFFECT_NEUTRAL,
	ATTRIB_EFFECT_POSITIVE,
	ATTRIB_EFFECT_NEGATIVE,
};

// Macro used to make strings empty
#define CLEAR_STR(name)		\
		name[0] = '\0'

struct TFAttributeDefinition
{
	TFAttributeDefinition()
	{
		CLEAR_STR( name );
		CLEAR_STR( attribute_class );
		CLEAR_STR( description_string );
		string_attribute = false;
		description_format = -1;
		hidden = false;
		effect_type = -1;
		stored_as_integer = false;
	}

	char name[128];
	char attribute_class[128];
	char description_string[128];
	bool string_attribute;
	int description_format;
	int effect_type;
	bool hidden;
	bool stored_as_integer;
};

// Client specific.
#ifdef CLIENT_DLL
EXTERN_RECV_TABLE( DT_EconItemAttribute );
// Server specific.
#else
EXTERN_SEND_TABLE( DT_EconItemAttribute );
#endif

class CTFItemAttribute
{
public:
	DECLARE_EMBEDDED_NETWORKVAR();
	DECLARE_CLASS_NOBASE( CTFItemAttribute );

	CTFItemAttribute()
	{
		Init( -1, 0.0f );
	}
	CTFItemAttribute( int iIndex, float flValue )
	{
		Init( iIndex, flValue );
	}
	CTFItemAttribute( int iIndex, float flValue, const char* pszAttributeClass )
	{
		Init( iIndex, flValue, pszAttributeClass );
	}
	CTFItemAttribute( int iIndex, const char* pszValue, const char* pszAttributeClass )
	{
		Init( iIndex, pszValue, pszAttributeClass );
	}

	void Init( int iIndex, float flValue, const char* pszAttributeClass = NULL );
	void Init( int iIndex, const char* pszValue, const char* pszAttributeClass = NULL );
	TFAttributeDefinition* GetStaticData( void );

public:
	CNetworkVar( int, m_iAttributeDefinitionIndex );
	CNetworkVar( float, value ); // m_iRawValue32
	CNetworkString( value_string, 128 );
	CNetworkString( attribute_class, 128 );
	string_t m_strAttributeClass;
};

class TFItemVisuals
{
public:
	TFItemVisuals()
	{
		SetDefLessFunc( animation_replacement );
		memset( aWeaponSounds, 0, sizeof( aWeaponSounds ) );
	}

public:
	CUtlDict< bool, unsigned short > player_bodygroups;
	CUtlMap< int, int > animation_replacement;
	CUtlDict< const char*, unsigned short > playback_activity;
	CUtlDict< const char*, unsigned short > misc_info;
	char aWeaponSounds[NUM_SHOOT_SOUND_TYPES][MAX_WEAPON_STRING];
	//CUtlDict< EconItemStyle*, unsigned short > styles;
};

// Contains what makes an item an item (name, quality, etc.)
class CTFItemDefinition
{
public:
	CTFItemDefinition()
	{
		CLEAR_STR( name );
		used_by_classes = 0;

		for ( int i = 0; i < TF_CLASS_COUNT_ALL; i++ )
			item_slot_per_class[i] = -1;

		show_in_armory = false;
		CLEAR_STR( item_class );
		CLEAR_STR( item_type_name );
		CLEAR_STR( item_name );
		CLEAR_STR( item_description );
		item_slot = -1;
		anim_slot = -1;
		CLEAR_STR( item_quality );
		baseitem = false;
		propername = false;
		CLEAR_STR( item_logname );
		CLEAR_STR( item_iconname );
		min_ilevel = 0;
		max_ilevel = 0;
		CLEAR_STR( image_inventory );
		image_inventory_size_w = 0;
		image_inventory_size_h = 0;
		CLEAR_STR( model_player );
		CLEAR_STR( model_world );
		memset( model_player_per_class, 0, sizeof( model_player_per_class ) );
		attach_to_hands = 0;
		act_as_wearable = false;
		CLEAR_STR( mouse_pressed_sound );
		CLEAR_STR( drop_sound );
	}

	TFItemVisuals* GetVisuals( int iTeamNum = TEAM_UNASSIGNED );
	int GetLoadoutSlot( int iClass = TF_CLASS_UNDEFINED );
	CTFItemAttribute* IterateAttributes( string_t strClass );

public:
	char name[128]; // Internal name
	CUtlDict< bool, unsigned short > capabilities;
	CUtlDict< bool, unsigned short > tags;
	int used_by_classes; // Which classes can use this weapon?
	int item_slot_per_class[TF_CLASS_COUNT_ALL]; // Different classes have different slots for this weapon (e.g. Soldier has shotgun as secondary, Engi as primary)
	bool show_in_armory;
	char item_class[128]; // Item's classname (e.g. tf_weapon_medigun)
	char item_type_name[128]; // The type of item (e.g. Level 5 *Bat*)
	char item_name[128]; // Name shown in armory
	char item_description[128]; // Description (unused)
	int  item_slot; // Item slot (primary, secondary, melee, PDA)
	int  anim_slot; // Anim slot this item uses (primary,secondary,melee) used for the playermodel to know how to animate
	char item_quality[128]; // Quality ("Normal", "Unique", "Strange" etc.)
	bool baseitem; // Is this a base (stock?) item?
	bool propername; // if true, add "The" before item name e.g. "The Kritzkrieg"
	char item_logname[128]; // What name the killfeed should show in logfiles (mainly used by stat plugins like HLstatsX)
	char item_iconname[128]; // Killfeed icon name
	int	 min_ilevel; // Minimum level this item can have
	int	 max_ilevel; // Maximum level this item can have
	char image_inventory[128]; // Inventory icon
	int	 image_inventory_size_w; // Width
	int	 image_inventory_size_h; // Height
	char model_player[128]; // Viewmodel (v_/c_model)
	char model_world[128]; // Worldmodel (w_model)
	char model_player_per_class[TF_CLASS_COUNT_ALL][128]; // Different classes have different viewmodels for this weapon (probably isnt used often cause of c_models)
	int attach_to_hands; // Does this weapon use a c_model?
	bool act_as_wearable; // Unused
	CUtlVector<CTFItemAttribute> attributes; // Vector containing what attributes this item uses (see CTFItemAttribute for more details)
	TFItemVisuals visual[TF_TEAM_COUNT];
	char mouse_pressed_sound[128]; // Unused(?) What sound it makes when clicked on in the armory
	char drop_sound[128]; // Unused(?)
};
#endif