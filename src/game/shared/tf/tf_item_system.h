//================================================================
// Goldrush Item Schema - powered by conneath industries ltd co tm
// credit to the tf2c team for making the original econ system
// that every tf2 mod seems to use these days (including us)
//================================================================
#ifndef TF_ITEM_SYSTEM_H
#define TF_ITEM_SYSTEM_H

#ifdef _WIN32
#pragma once
#endif
class CTFSchemaParser;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CTFItemSchema
{
	friend class CTFSchemaParser;
	friend class CTFInventory;

public:
	CTFItemSchema();
	~CTFItemSchema();

	bool Init( void );
	void Precache( void );

	CTFItemDefinition* GetItemDefinition( int id );
	TFAttributeDefinition* GetAttributeDefinition( int id );
	TFAttributeDefinition* GetAttributeDefinitionByName( const char* name );
	TFAttributeDefinition* GetAttributeDefinitionByClass( const char* name );
	int GetAttributeIndex( const char* classname );

private:
	CUtlDict< int, unsigned short >					m_GameInfo;
	CUtlDict< TFItemQuality, unsigned short >			m_Qualities;
	CUtlDict< TFItemColor, unsigned short >			m_Colors;
	CUtlDict< KeyValues*, unsigned short >			m_PrefabsValues;
	CUtlMap< int, CTFItemDefinition* >			m_Items;
	CUtlMap< int, TFAttributeDefinition* >		m_Attributes;

	bool m_bInited;
};

CTFItemSchema* GetItemSchema();

#endif //TF_ITEM_SYSTEM_H