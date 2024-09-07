#ifndef IHASATTRIBUTES_H
#define IHASATTRIBUTES_H
#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"

class CTFAttributeManager;
class CTFAttributeContainer;

//-----------------------------------------------------------------------------
// Purpose: Allows to determine if entity has attribute related members
//-----------------------------------------------------------------------------
class IHasAttributes
{
public:
	virtual CTFAttributeManager* GetAttributeManager( void ) = 0;
	virtual CTFAttributeContainer* GetAttributeContainer( void ) = 0;
	virtual CBaseEntity* GetAttributeOwner( void ) = 0;
	virtual void ReapplyProvision( void ) = 0;
};

#endif // IHASATTRIBUTES_H