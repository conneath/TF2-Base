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

DECLARE_BUILD_FACTORY( CEmbeddedItemModelPanel );
DECLARE_BUILD_FACTORY( CItemModelPanel );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CEmbeddedItemModelPanel::CEmbeddedItemModelPanel( vgui::Panel* pParent, const char* pName ) : BaseClass( pParent, "itemmodelpanel" )
{
}

//-----------------------------------------------------------------------------
// Purpose: Get the needed info from an item definition (name, model, attribs etc.)
//-----------------------------------------------------------------------------
void CEmbeddedItemModelPanel::SetEconItem( CEconItemView* pItem )
{
	if ( !pItem )
		return;

	// FIXME: if a weapon has different worldmodels for different classes this will break, but we dont have any weapon like this rn so i cant be bothered
	SetMDL( pItem->GetWorldDisplayModel(TF_CLASS_UNDEFINED) ); 
}

//////////////////
// CItemModelPanel
//////////////////
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CItemModelPanel::CItemModelPanel( vgui::Panel* parent, const char* name ) : vgui::EditablePanel( parent, name )
{
	m_pEmbItemModelPanel = NULL;

	InvalidateLayout( true, true );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CItemModelPanel::ApplySchemeSettings( vgui::IScheme* pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );
	SetProportional( true );
	LoadControlSettings( "Resource/UI/ItemModelPanel.res" );

	m_pEmbItemModelPanel = dynamic_cast<CEmbeddedItemModelPanel*>(FindChildByName( "itemmodelpanel" )); // the actual panel used for displaying the item's model
	m_pNameLabel = dynamic_cast<CTFLabel*>(FindChildByName( "namelabel" ));
	m_pAttribLabel = dynamic_cast<CTFLabel*>(FindChildByName( "attriblabel" ));
}

void CItemModelPanel::ApplySettings( KeyValues* inResourceData )
{
	BaseClass::ApplySettings( inResourceData );
	InvalidateLayout( false, true ); // Force ApplySchemeSettings to run.
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CItemModelPanel::PerformLayout()
{
	BaseClass::PerformLayout();

	int x, y, w, h;
	m_pEmbItemModelPanel->GetBounds( x, y, w, h );
	int iAdjustedY = y + ((GetTall() - m_nModelTall) / 2);
	m_pEmbItemModelPanel->SetBounds( m_nModelX, iAdjustedY, m_nModelWidth, m_nModelTall );

	m_pNameLabel->SizeToContents();
	m_pAttribLabel->SizeToContents();

	int iNameWide, iNameTall;
	m_pNameLabel->GetContentSize( iNameWide, iNameTall );

	int iAttribWide, iAttribTall;
	m_pAttribLabel->GetContentSize( iAttribWide, iAttribTall );

	// I legitimately don't know
	iAdjustedY = abs( GetTall() - (iAttribTall + iNameTall) ) / 2;

	m_pNameLabel->SetBounds( m_nTextX, iAdjustedY, m_nTextWide, iNameTall );
	m_pNameLabel->SetContentAlignment( vgui::Label::Alignment::a_south );

	m_pAttribLabel->SetBounds( m_nTextX, iAdjustedY + iNameTall, m_nTextWide, iAttribTall );
	m_pAttribLabel->SetContentAlignment( vgui::Label::Alignment::a_north );
}

//-----------------------------------------------------------------------------
// Purpose: Get the needed info from an item definition (name, model, attribs etc.)
//-----------------------------------------------------------------------------
void CItemModelPanel::SetEconItem( CEconItemView* pItem )
{
	if ( !pItem )
		return;

	m_pEmbItemModelPanel->SetEconItem( pItem );

	CEconItemDefinition* pItemDef = pItem->GetStaticData();
	SetDialogVariable( "itemname", g_pVGuiLocalize->Find(pItemDef->item_name) );
	SetDialogVariable( "attriblist", g_pVGuiLocalize->Find( pItemDef->item_type_name ) );
	//wchar_t* pszFormatString[80];
	//g_pVGuiLocalize->ConstructString( pszFormatString, sizeof( pszFormatString ), g_pVGuiLocalize->Find( "#ItemTypeDesc" ), 2, pItemDef->max_ilevel, pItemDef->item_type_name );
	
	//SetDialogVariable( "attriblist",  );
	//for ( int i = 0; i < pItemDef->attributes.Count(); i++ )
	//{
		//pItemDef->attributes[i].GetStaticData()->
	//}
}