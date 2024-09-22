//========= Team Fortress 2 Gold Rush ============//
//
// Purpose:
//
//================================================//

#include "cbase.h"
#include "item_model_panel.h"
#include "item_selection_panel.h"
#include "tf_inventory.h"
#include "class_loadout_panel.h"
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
		m_vecItemPanels.PurgeAndDeleteElements(); // delete those model panels
		m_vecChangeButtons.PurgeAndDeleteElements();
	}
	if ( !Q_strnicmp(command, "equip ", 6) )
	{
		int iItemNum = atoi(command+6);
		Msg( "Got told to change slot %i to weapon ID %i!\n", m_iCurrentSlotIndex, iItemNum);
		GetTFInventory()->SetWeaponPreset( m_iCurrentClassIndex, m_iCurrentSlotIndex, iItemNum );
		// HACK: tell our parent (which SHOULD be the loadout panel) to update, so our preset change is actually shown
		// I should use a vgui message instead of doing this but im tired boss
		dynamic_cast<CClassLoadoutPanel*>(GetParent())->UpdateModelPanels();
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

	int iNumWeaponsForSlot = GetTFInventory()->NumWeapons( m_iCurrentClassIndex, m_iCurrentSlotIndex );
	char itempanelnamebuffer[30];
	char buttonnamebuffer[30];
	char buttoncommand[10];
	int panelx = 100;
	int panely = 100;
	// gold rush update positioned these panels in code ONLY, and i dont feel like decompiling it
	// so im just going to fucking eyeball it, fuck you.
	// i cant even access the selection panel in there to check panel positions via vgui_drawtree because i don't have the achievement items, fml
	for ( int i = 0; i < iNumWeaponsForSlot; i++ )
	{
		// create the vgui name of this panel to set later
		Q_snprintf( itempanelnamebuffer, sizeof( itempanelnamebuffer ), "weapon%i", i );
		Q_snprintf( buttonnamebuffer, sizeof( buttonnamebuffer ), "change%i", i );

		CItemModelPanel* itemPanel = new CItemModelPanel( this, itempanelnamebuffer );
		m_vecItemPanels.AddToTail( itemPanel );
		m_vecItemPanels[i]->InvalidateLayout(false, true);
		m_vecItemPanels[i]->SetPos( panelx, panely );
		panely += 200;
		m_vecItemPanels[i]->SetWide( 800 );
		m_vecItemPanels[i]->SetTall( 150 );
		m_vecItemPanels[i]->SetEconItem( GetTFInventory()->GetItem( m_iCurrentClassIndex, m_iCurrentSlotIndex, i ) );
		Msg( "Created new CItemModelPanel with weapon ID %i\n", i );

		CTFButton* changeButton = new CTFButton( this, buttonnamebuffer, "Equip");
		m_vecChangeButtons.AddToTail( changeButton );
		m_vecChangeButtons[i]->SetPos(600, panely-150);
		Q_snprintf( buttoncommand, sizeof( buttoncommand ), "equip %i", i );
		m_vecChangeButtons[i]->SetCommand( buttoncommand );
		/*
		if ( GetTFInventory()->GetItem( m_iCurrentClassIndex, m_iCurrentSlotIndex, GetTFInventory()->GetWeaponPreset( m_iCurrentClassIndex, m_iCurrentSlotIndex ) ) == GetTFInventory()->GetItem( m_iCurrentClassIndex, m_iCurrentSlotIndex, i ) )
		{

		}
		*/
	}
}
void CItemSelectionPanel::ApplySchemeSettings( vgui::IScheme* pScheme )
{
	LoadControlSettings( "Resource/UI/ItemSelectionPanel.res" );
	BaseClass::ApplySchemeSettings( pScheme );
}
