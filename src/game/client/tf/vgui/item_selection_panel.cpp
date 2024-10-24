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

CItemSelectionPanel::CItemSelectionPanel( vgui::Panel* parent ) : BaseClass( parent, "ItemSelectionPanel",
	vgui::scheme()->LoadSchemeFromFile( "Resource/ClientScheme.res", "ClientScheme" ) )
{
	//SetParent( parent );
	m_iCurrentClassIndex = TF_CLASS_UNDEFINED;
	m_iCurrentSlotIndex = TF_LOADOUT_SLOT_PRIMARY;
	m_iSelectedPreset = 0;
	//m_pItemContainer = NULL;

	m_pItemContainer = new EditablePanel( this, "itemcontainer" );
	m_pSlotLabel = new CTFLabel( this, "ItemSlotLabel", "" );
	InvalidateLayout( true, true );
}

void CItemSelectionPanel::ApplySchemeSettings( vgui::IScheme* pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );
	SetProportional( true );
	LoadControlSettings( "Resource/UI/ItemSelectionPanel.res" );
}

void CItemSelectionPanel::ApplySettings( KeyValues* inResourceData )
{
	BaseClass::ApplySettings( inResourceData );

	wchar_t* wzLocalizedClassName = g_pVGuiLocalize->Find( g_aPlayerClassNames[m_iCurrentClassIndex] );
	SetDialogVariable( "loadoutclass", wzLocalizedClassName );

	KeyValues* pItemsKV = inResourceData->FindKey( "itemskv" );
	KeyValues* pButtonsKV = inResourceData->FindKey( "buttonskv" );
	if ( pItemsKV )
	{
		// apply the keyvalues specified in the res to our panels and buttons
		FOR_EACH_VEC( m_vecItemPanels, i )
		{
			m_vecItemPanels[i]->ApplySettings( pItemsKV );

			if ( pButtonsKV )
			{
				m_vecChangeButtons[i]->ApplySettings( pButtonsKV );
			}
		}
	}
}

void CItemSelectionPanel::OnCommand( const char* command )
{
	if ( Q_stricmp( command, "vguicancel" ) )
	{
		SetVisible( false );
	}
	if ( !Q_strnicmp(command, "equip ", 6) )
	{
		int iItemNum = atoi(command+6);
#ifdef DEBUG
		Msg( "Got told to change slot %i to weapon ID %i!\n", m_iCurrentSlotIndex, iItemNum);
#endif
		GetTFInventory()->SetWeaponPreset( m_iCurrentClassIndex, m_iCurrentSlotIndex, iItemNum );

		if ( engine->IsInGame() )
		{
			char szCmd[64];
			Q_snprintf( szCmd, sizeof( szCmd ), "weaponpresetclass %d %d %d;", m_iCurrentClassIndex, m_iCurrentSlotIndex, iItemNum );
			engine->ExecuteClientCmd( szCmd );
		}

		// HACK: tell our parent (which SHOULD be the loadout panel) to update, so our preset change is actually shown
		// I should use a vgui message instead of doing this but im tired boss
		dynamic_cast<CClassLoadoutPanel*>(GetParent())->UpdateModelPanels();
	}
}
void CItemSelectionPanel::PerformLayout( void )
{
	BaseClass::PerformLayout();

	int y = m_nItemYDelta;
	// set the position of each of our loadout items in the scrollable panel
	FOR_EACH_VEC( m_vecItemPanels, i )
	{
		m_vecItemPanels[i]->SetPos( m_nItemX, y );

		// if the item we're setting the position of is our currently equipped weapon
		// show the currently equipped label and bg behind it
		if ( m_iSelectedPreset == i )
		{
			EditablePanel* pBG = FindControl<EditablePanel>( "CurrentlyEquippedBackground", true );
			vgui::Label* pLabel = FindControl<vgui::Label>( "CurrentlyEquippedLabel", true );
			if ( pBG && pLabel )
			{
				pBG->SetVisible( true );
				pBG->SetPos( pBG->GetXPos(), y );
				int iLabelY = (pBG->GetTall() - pLabel->GetTall()) / 2;
				pLabel->SetVisible( true );
				pLabel->SetPos( pLabel->GetXPos(), y + iLabelY );
			}
			m_vecChangeButtons[i]->SetText( "#Keep" );
		}

		int iButtonY = (m_vecItemPanels[i]->GetTall() - m_vecChangeButtons[i]->GetTall()) / 2;
		m_vecChangeButtons[i]->SetPos( m_nButtonXPos, y + iButtonY );
		y += m_vecItemPanels[i]->GetTall() + m_nItemYDelta;
	}
	switch ( m_iCurrentSlotIndex )
	{
	case(TF_LOADOUT_SLOT_PRIMARY):
		m_pSlotLabel->SetText( "#ItemSel_PRIMARY" );
		break;
	case(TF_LOADOUT_SLOT_SECONDARY):
		m_pSlotLabel->SetText( "#ItemSel_SECONDARY" );
		break;
	case(TF_LOADOUT_SLOT_MELEE):
		m_pSlotLabel->SetText( "#ItemSel_MELEE" );
		break;
	default:
		m_pSlotLabel->SetText( "" );
	}

	SetVisible( true );
}
//-----------------------------------------------------------------------------
// Purpose: Set our class and slot, and set everything up accordingly
//-----------------------------------------------------------------------------
void CItemSelectionPanel::SetClassAndSlot( int iClass, int iSlot )
{
	m_iCurrentClassIndex = iClass;
	m_iCurrentSlotIndex = iSlot;

	m_vecItemPanels.PurgeAndDeleteElements(); // delete those model panels
	m_vecChangeButtons.PurgeAndDeleteElements();
	InvalidateLayout( true, true );

	//SetDialogVariable( "loadoutclass", g_pVGuiLocalize->Find( g_aPlayerClassNames[m_iCurrentClassIndex] ) );

	int iNumWeaponsForSlot = GetTFInventory()->NumWeapons( m_iCurrentClassIndex, m_iCurrentSlotIndex );
	char itempanelnamebuffer[30];
	char buttonnamebuffer[30];
	char buttoncommand[10];
	//int panelx = m_pItemContainer->GetWide() /4.5;
	//int panely = m_pItemContainer->GetTall() /6;

	for ( int i = 0; i < iNumWeaponsForSlot; i++ )
	{
		// create the vgui name of this panel to set later
		Q_snprintf( itempanelnamebuffer, sizeof( itempanelnamebuffer ), "weapon%i", i );
		Q_snprintf( buttonnamebuffer, sizeof( buttonnamebuffer ), "change%i", i );
		Q_snprintf( buttoncommand, sizeof( buttoncommand ), "equip %i", i );

		CItemModelPanel* itemPanel = new CItemModelPanel( this, itempanelnamebuffer );
		m_vecItemPanels.AddToTail( itemPanel );
		m_vecItemPanels[i]->SetParent( m_pItemContainer );
		m_vecItemPanels[i]->SetEconItem( GetTFInventory()->GetItem( m_iCurrentClassIndex, m_iCurrentSlotIndex, i ) );

		vgui::Button* changeButton = new vgui::Button( this, buttonnamebuffer, "Equip");
		changeButton->SetVisible( true );
		changeButton->AddActionSignalTarget( this );
		changeButton->SetParent( m_pItemContainer );
		changeButton->SetCommand( buttoncommand );

		m_vecChangeButtons.AddToTail( changeButton );
	}
	// cache our currently selected slot preset
	m_iSelectedPreset = GetTFInventory()->GetWeaponPreset( m_iCurrentClassIndex, m_iCurrentSlotIndex );

	InvalidateLayout( true, true );
	SetVisible( true );
}

