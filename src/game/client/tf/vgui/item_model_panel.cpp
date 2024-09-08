//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#include "cbase.h"
#include "vgui/IInput.h"
#include <vgui/IVGui.h>
#include <vgui/VGUI.h>
#include <vgui/IScheme.h>
#include "item_model_panel.h"
#include "vgui/ILocalize.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

DECLARE_BUILD_FACTORY( CItemModelPanel );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CEmbeddedItemModelPanel::CEmbeddedItemModelPanel( vgui::Panel* pParent, const char* pName ) : BaseClass( pParent, pName )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CItemModelPanel::CItemModelPanel( vgui::Panel* parent, const char* name ) : vgui::EditablePanel( parent, name )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CItemModelPanel::ApplySchemeSettings( vgui::IScheme* pScheme )
{
	LoadControlSettings( "Resource/UI/ItemModelPanel.res" );
}

//-----------------------------------------------------------------------------
// Purpose: Get the needed info from an item definition (name, model, attribs etc.)
//-----------------------------------------------------------------------------
void CItemModelPanel::SetEconItem( CEconItemView* pItem )
{
	if ( !pItem )
		return;
	CEconItemDefinition* pItemDef = pItem->GetStaticData();
	
		SetDialogVariable( "itemname", g_pVGuiLocalize->Find(pItemDef->item_name) );
}