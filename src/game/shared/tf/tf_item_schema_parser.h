//================================================================
// Goldrush Item Schema Parser - powered by conneath industries ltd co tm
// credit to the tf2c team for making the original econ system
// that every tf2 mod seems to use these days (including us)
//================================================================
#ifndef TF_ITEM_SCHEMA_PARSER_H
#define TF_ITEM_SCHEMA_PARSER_H

#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include "script_parser.h"

#define GET_STRING(copyto, from, name)													\
		if (from->GetString(#name, NULL))												\
			V_strncpy(copyto->name, from->GetString(#name), sizeof(copyto->name))

#define GET_STRING_DEFAULT(copyto, from, name, defaultstring)		\
		V_strncpy(copyto->name, from->GetString(#name, #defaultstring), sizeof(copyto->name))

#define GET_BOOL(copyto, from, name)													\
		copyto->name = from->GetBool(#name, copyto->name)

#define GET_FLOAT(copyto, from, name)													\
		copyto->name = from->GetFloat(#name, copyto->name)

#define GET_INT(copyto, from, name)														\
		copyto->name = from->GetInt(#name, copyto->name)

#define GET_STRING_CONVERT(copyto, from, name)											\
		if (from->GetString(#name, NULL))

#define FIND_ELEMENT(map, key, val)						\
		unsigned int index = map.Find(key);				\
		if (index != map.InvalidIndex())						\
			val = map.Element(index)				

#define FIND_ELEMENT_STRING(map, key, val)						\
		unsigned int index = map.Find(key);						\
		if (index != map.InvalidIndex())								\
			Q_snprintf(val, sizeof(val), map.Element(index))

#define IF_ELEMENT_FOUND(map, key)						\
		unsigned int index = map.Find(key);				\
		if (index != map.InvalidIndex())			

#define GET_VALUES_FAST_BOOL(dict, keys)\
		for (KeyValues *pKeyData = keys->GetFirstSubKey(); pKeyData != NULL; pKeyData = pKeyData->GetNextKey())\
				{													\
			IF_ELEMENT_FOUND(dict, pKeyData->GetName())		\
				{												\
				dict.Element(index) = pKeyData->GetBool();	\
				}												\
						else											\
				{												\
				dict.Insert(pKeyData->GetName(), pKeyData->GetBool());\
				}												\
				}


#define GET_VALUES_FAST_STRING(dict, keys)\
		for (KeyValues *pKeyData = keys->GetFirstSubKey(); pKeyData != NULL; pKeyData = pKeyData->GetNextKey())	\
				{													\
			IF_ELEMENT_FOUND(dict, pKeyData->GetName())		\
				{												\
				Q_snprintf((char*)dict.Element(index), sizeof(dict.Element(index)), pKeyData->GetString());		\
				}												\
						else											\
				{												\
				dict.Insert(pKeyData->GetName(), strdup(pKeyData->GetString()));\
				}												\
				}	

class CEconSchemaParser : public CScriptParser
{
public:
	DECLARE_CLASS_GAMEROOT( CEconSchemaParser, CScriptParser );
	void Parse( KeyValues* pKeyValuesData, bool bWildcard, const char* szFileWithoutEXT );
	void ParseGameInfo( KeyValues* pKeyValuesData );
	void ParseQualities( KeyValues* pKeyValuesData );
	void ParseColors( KeyValues* pKeyValuesData );
	void ParsePrefabs( KeyValues* pKeyValuesData );
	void ParseItems( KeyValues* pKeyValuesData );
	void ParseAttributes( KeyValues* pKeyValuesData );
	bool ParseVisuals( KeyValues* pData, CEconItemDefinition* pItem, int iIndex );
	bool ParseItemRec( KeyValues* pData, CEconItemDefinition* pItem );

};

#endif // TF_ITEM_SCHEMA_PARSER_H