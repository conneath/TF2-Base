//========= Team Fortress 2 Gold Rush ============//
//
// Purpose:
//
//================================================//

#include "cbase.h"
#include "item_model_panel.h"
#include "item_selection_panel.h"
#include <vgui/ILocalize.h>

CItemSelectionPanel::CItemSelectionPanel( vgui::Panel* parent, const char* name ) : vgui::EditablePanel( parent, name )
{
	//SetParent( parent );
	m_iCurrentClassIndex = NULL;
	m_iCurrentSlotIndex = NULL;
}

void CItemSelectionPanel::OnCommand( const char* command )
{
	if ( Q_stricmp( command, "vguicancel" ) )
	{
		SetVisible( false );
	}
}
//-----------------------------------------------------------------------------
// Purpose: Set our class and slot, and set everything up accordingly
//-----------------------------------------------------------------------------
void CItemSelectionPanel::SetClassAndSlot( int iClass, int iSlot )
{
	m_iCurrentClassIndex = iClass;
	m_iCurrentSlotIndex = iSlot;

	SetDialogVariable( "loadoutclass", g_pVGuiLocalize->Find( g_aPlayerClassNames[m_iCurrentClassIndex] ) );
}
void CItemSelectionPanel::ApplySchemeSettings( vgui::IScheme* pScheme )
{
	LoadControlSettings( "Resource/UI/ItemSelectionPanel.res" );
	BaseClass::ApplySchemeSettings( pScheme );
}
