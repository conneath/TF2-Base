//================================================================
// Goldrush Attribute Manager - powered by conneath industries ltd co tm
// credit to the tf2c team for making the original econ system
// that every tf2 mod seems to use these days (including us)
//================================================================
#ifndef TF_ATTRIBUTE_MANAGER_H
#define TF_ATTRIBUTE_MANAGER_H

#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"

// Client specific.
#ifdef CLIENT_DLL
EXTERN_RECV_TABLE( DT_TFAttributeManager );
EXTERN_RECV_TABLE( DT_TFAttributeContainer );
// Server specific.
#else
EXTERN_SEND_TABLE( DT_TFAttributeManager );
EXTERN_SEND_TABLE( DT_TFAttributeContainer );
#endif

class CTFAttributeManager
{
public:
	DECLARE_EMBEDDED_NETWORKVAR();
	DECLARE_CLASS_NOBASE( CTFAttributeManager );

	CTFAttributeManager();
	template <typename type>
	static type AttribHookValue( type value, const char* pszClass, const CBaseEntity* pEntity )
	{
		if ( !pEntity )
			return value;

		IHasAttributes* pAttribInteface = pEntity->GetHasAttributesInterfacePtr();

		if ( pAttribInteface )
		{
			string_t strAttributeClass = AllocPooledString_StaticConstantStringPointer( pszClass );
			float flResult = pAttribInteface->GetAttributeManager()->ApplyAttributeFloat( (float)value, pEntity, strAttributeClass );
			value = (type)flResult;
		}

		return value;
	}

#ifdef CLIENT_DLL
	virtual void		OnPreDataChanged( DataUpdateType_t updateType );
	virtual void		OnDataChanged( DataUpdateType_t updatetype );
#endif
	void				AddProvider( CBaseEntity* pEntity );
	void				RemoveProvider( CBaseEntity* pEntity );
	void				ProviteTo( CBaseEntity* pEntity );
	void				StopProvidingTo( CBaseEntity* pEntity );
	virtual void		InitializeAttributes( CBaseEntity* pEntity );
	virtual float		ApplyAttributeFloat( float flValue, const CBaseEntity* pEntity, string_t strAttributeClass );
	virtual string_t	ApplyAttributeString( string_t strValue, const CBaseEntity* pEntity, string_t strAttributeClass );

protected:
	CNetworkHandle( CBaseEntity, m_hOuter );
	bool m_bParsingMyself;

	CNetworkVar( int, m_iReapplyProvisionParity );
#ifdef CLIENT_DLL
	int m_iOldReapplyProvisionParity;
#endif

private:
	CUtlVector<EHANDLE> m_AttributeProviders;
};

template <>
string_t CTFAttributeManager::AttribHookValue<string_t>( string_t strValue, const char* pszClass, const CBaseEntity* pEntity );


class CTFAttributeContainer : public CTFAttributeManager
{
public:
	DECLARE_EMBEDDED_NETWORKVAR();
	DECLARE_CLASS( CTFAttributeContainer, CTFAttributeManager );
#ifdef CLIENT_DLL
	DECLARE_PREDICTABLE();
#endif

	CTFAttributeContainer();

	float		ApplyAttributeFloat( float flValue, const CBaseEntity* pEntity, string_t strAttributeClass );
	string_t	ApplyAttributeString( string_t strValue, const CBaseEntity* pEntity, string_t strAttributeClass );
};

#endif // ATTRIBUTE_MANAGER_H