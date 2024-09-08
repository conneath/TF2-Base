//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#include "cbase.h"
#include "character_info_panel.h"
#include "class_loadout_panel.h"
#include "tf_playerclass_shared.h"
#include "charinfo_loadout_subpanel.h"
#include "item_model_panel.h"
#include "tf_inventory.h"

CClassLoadoutPanel* g_pClassLoadoutPanel = NULL;
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CClassLoadoutPanel::CClassLoadoutPanel( vgui::Panel* parent )
	: vgui::EditablePanel( parent, "full_loadout" )
{
	m_iCurrentClassIndex = TF_CLASS_UNDEFINED;
	m_iCurrentTeamIndex = TF_TEAM_RED;
	m_iCurrentSlotIndex = -1;
	m_pPlayerModelPanel = NULL;
	m_pPrimaryWeaponPanel = NULL;
	m_pSecondaryWeaponPanel = NULL;
	m_pMeleeWeaponPanel = NULL;

	/*
	m_pSelectionPanel = NULL;
	m_pTauntHintLabel = NULL;
	m_pTauntLabel = NULL;
	m_pTauntCaratLabel = NULL;
	m_pPassiveAttribsLabel = NULL;
	m_pLoadoutPresetPanel = NULL;
	m_pPresetsExplanationPopup = NULL;
	m_pTauntsExplanationPopup = NULL;
	m_pBuildablesButton = NULL;

	m_pCharacterLoadoutButton = NULL;
	m_pTauntLoadoutButton = NULL;

	m_bInTauntLoadoutMode = false;
	*/
	g_pClassLoadoutPanel = this;

	//m_pItemOptionPanel = new CLoadoutItemOptionsPanel( this, "ItemOptionsPanel" );
}

CClassLoadoutPanel::~CClassLoadoutPanel()
{
	/*
	if ( m_pItemOptionPanelKVs )
	{
		m_pItemOptionPanelKVs->deleteThis();
		m_pItemOptionPanelKVs = NULL;
	}
	*/
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClassLoadoutPanel::ApplySchemeSettings( vgui::IScheme* pScheme )
{
	LoadControlSettings( "Resource/UI/FullLoadoutPanel.res" );

	BaseClass::ApplySchemeSettings( pScheme );

	m_pPlayerModelPanel = dynamic_cast<CBaseModelPanel*>(FindChildByName( "classmodelpanel" ));
	m_pClassLabel = dynamic_cast<vgui::Label*>(FindChildByName( "ClassLabel" ));

	// temporary until econ is implemented, these should show when you're changing a weapon slot
	FindChildByName( "InventoryCount0" )->SetVisible( false );
	FindChildByName( "InventoryCount1" )->SetVisible( false );
	FindChildByName( "InventoryCount2" )->SetVisible( false );

	FindChildByName( "ChangeButton0" )->SetVisible( false );
	FindChildByName( "ChangeButton1" )->SetVisible( false );
	FindChildByName( "ChangeButton2" )->SetVisible( false );

	// hardcoding ftw
	m_pPrimaryWeaponPanel = dynamic_cast<CItemModelPanel*>(FindChildByName( "modelpanel0" ));
	m_pSecondaryWeaponPanel = dynamic_cast<CItemModelPanel*>(FindChildByName( "modelpanel1" ));
	m_pMeleeWeaponPanel = dynamic_cast<CItemModelPanel*>(FindChildByName( "modelpanel2" ));

	// tell users no items exist yet for now
	dynamic_cast<vgui::Label*>(FindChildByName( "NoneAvailableReason" ))->SetText( "#NoItemsExistLong" );

	//m_pTopLinePanel = FindChildByName( "TopLine" ); where the fuck is this used lol
	/*
	if ( m_pPassiveAttribsLabel )
	{
		m_pPassiveAttribsLabel->SetMouseInputEnabled( false );
	}
	*/
	//m_pMouseOverTooltip->SetPositioningStrategy( IPTTP_BOTTOM_SIDE );
	/*
	m_aDefaultColors[LOADED][FG][DEFAULT] = vgui::scheme()->GetIScheme( GetScheme() )->GetColor( "Econ.Button.PresetDefaultColorFg", Color( 255, 255, 255, 255 ) );
	m_aDefaultColors[LOADED][FG][ARMED] = vgui::scheme()->GetIScheme( GetScheme() )->GetColor( "Econ.Button.PresetArmedColorFg", Color( 255, 255, 255, 255 ) );
	m_aDefaultColors[LOADED][FG][DEPRESSED] = vgui::scheme()->GetIScheme( GetScheme() )->GetColor( "Econ.Button.PresetDepressedColorFg", Color( 255, 255, 255, 255 ) );

	m_aDefaultColors[LOADED][BG][DEFAULT] = vgui::scheme()->GetIScheme( GetScheme() )->GetColor( "Econ.Button.PresetDefaultColorBg", Color( 255, 255, 255, 255 ) );
	m_aDefaultColors[LOADED][BG][ARMED] = vgui::scheme()->GetIScheme( GetScheme() )->GetColor( "Econ.Button.PresetArmedColorBg", Color( 255, 255, 255, 255 ) );
	m_aDefaultColors[LOADED][BG][DEPRESSED] = vgui::scheme()->GetIScheme( GetScheme() )->GetColor( "Econ.Button.PresetDepressedColorBg", Color( 255, 255, 255, 255 ) );

	m_aDefaultColors[NOTLOADED][FG][DEFAULT] = vgui::scheme()->GetIScheme( GetScheme() )->GetColor( "Button.TextColor", Color( 255, 255, 255, 255 ) );
	m_aDefaultColors[NOTLOADED][FG][ARMED] = vgui::scheme()->GetIScheme( GetScheme() )->GetColor( "Button.ArmedTextColor", Color( 255, 255, 255, 255 ) );
	m_aDefaultColors[NOTLOADED][FG][DEPRESSED] = vgui::scheme()->GetIScheme( GetScheme() )->GetColor( "Button.DepressedTextColor", Color( 255, 255, 255, 255 ) );

	m_aDefaultColors[NOTLOADED][BG][DEFAULT] = vgui::scheme()->GetIScheme( GetScheme() )->GetColor( "Button.BgColor", Color( 255, 255, 255, 255 ) );
	m_aDefaultColors[NOTLOADED][BG][ARMED] = vgui::scheme()->GetIScheme( GetScheme() )->GetColor( "Button.ArmedBgColor", Color( 255, 255, 255, 255 ) );
	m_aDefaultColors[NOTLOADED][BG][DEPRESSED] = vgui::scheme()->GetIScheme( GetScheme() )->GetColor( "Button.DepressedBgColor", Color( 255, 255, 255, 255 ) );
	*/
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClassLoadoutPanel::OnCommand( const char* command )
{
	if ( FStrEq( command, "back" ) )
	{
		//ShowPanel( false, false );
		if ( dynamic_cast<CCharInfoLoadoutSubPanel*>( GetParent() ) )
			dynamic_cast<CCharInfoLoadoutSubPanel*>( GetParent() )->OnLoadoutClosed(); // conn - todo: make this use a PostMessage instead
	}
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClassLoadoutPanel::ShowPanel( int iClass, bool bBackpack, bool bReturningFromArmory )
{
	bool bShow = (iClass != 0 || bBackpack);
	//OnShowPanel( bShow, bReturningFromArmory );

	SetVisible( bShow );

	if ( bShow )
	{
		//HideMouseOverPanel(); TODO - conn: item tooltips(?)

		//CreateItemPanels();

		UpdateModelPanels();

		// make the first slot be selected so controller input will work
		/*
		static ConVarRef joystick( "joystick" );
		if ( joystick.IsValid() && joystick.GetBool() && m_pItemModelPanels.Count() && m_pItemModelPanels[0] )
		{
			m_pItemModelPanels[0]->SetSelected( true );
			m_pItemModelPanels[0]->RequestFocus();
		}
		*/
	}
	else
	{
		// clear items from panels to make sure that items get invalidate on show panel
		/*
		FOR_EACH_VEC( m_pItemModelPanels, i )
		{
			m_pItemModelPanels[i]->SetItem( NULL );
		}
		*/
	}

	if ( !bReturningFromArmory )
	{
		PostShowPanel( bShow );
	}
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClassLoadoutPanel::PostShowPanel( bool bVisible )
{
	if ( bVisible )
	{
		if ( m_pPlayerModelPanel )
		{
			m_pPlayerModelPanel->SetVisible( true );
		}

		SetDialogVariable("loadoutclass", g_pVGuiLocalize->Find(g_aPlayerClassNames[m_iCurrentClassIndex]));
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClassLoadoutPanel::SetClass( int iClass )
{
	m_iCurrentClassIndex = iClass;
	/*
	if ( m_pLoadoutPresetPanel )
	{
		m_pLoadoutPresetPanel->SetClass( m_iCurrentClassIndex );
	}
	*/
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CClassLoadoutPanel::SetTeam( int iTeam )
{
	//Assert( IsValidTFTeam( iTeam ) );
	m_iCurrentTeamIndex = iTeam;
}
//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CClassLoadoutPanel::UpdateModelPanels( void )
{
	// We're showing the loadout for a specific class.
	TFPlayerClassData_t* pData = GetPlayerClassData( m_iCurrentClassIndex );
	if ( m_pPlayerModelPanel )
	{
		m_pPlayerModelPanel->SetMDL(pData->GetModelName());
		//m_pPlayerModelPanel->SetSkin(0);
	}
	// set our weapon panels to the items for our class from CTFInventory
	if ( m_pPrimaryWeaponPanel )
	{
		m_pPrimaryWeaponPanel->SetEconItem(GetTFInventory()->GetItem( m_iCurrentClassIndex, TF_LOADOUT_SLOT_PRIMARY, 0 ));
	}
	if ( m_pSecondaryWeaponPanel )
	{
		m_pSecondaryWeaponPanel->SetEconItem( GetTFInventory()->GetItem( m_iCurrentClassIndex, TF_LOADOUT_SLOT_SECONDARY, 0 ) );
	}
	if ( m_pMeleeWeaponPanel ) // no PDA/Invis slots for now, sorry spy and engi
	{
		m_pMeleeWeaponPanel->SetEconItem( GetTFInventory()->GetItem( m_iCurrentClassIndex, TF_LOADOUT_SLOT_MELEE, 0 ) );
	}


	/*
	// For now, fill them out with the local player's currently wielded items
	for ( int i = 0; i < m_pItemModelPanels.Count(); i++ )
	{
		CEconItemView* pItemData = TFInventoryManager()->GetItemInLoadoutForClass( m_iCurrentClassIndex, i );
		m_pItemModelPanels[i]->SetItem( pItemData );
		m_pItemModelPanels[i]->SetShowQuantity( true );
		m_pItemModelPanels[i]->SetSelected( false );
		SetBorderForItem( m_pItemModelPanels[i], false );

		if ( m_pPlayerModelPanel && pItemData && pItemData->IsValid() )
		{
			m_pPlayerModelPanel->AddCarriedItem( pItemData );
		}
	}

	if ( m_pPlayerModelPanel )
	{
		m_pPlayerModelPanel->HoldItemInSlot( m_iCurrentSlotIndex );
	}

	SetDialogVariable( "loadoutclass", g_pVGuiLocalize->Find( pData->m_szLocalizableName ) );

	UpdatePassiveAttributes();

	// Now layout again to position our item buttons
	InvalidateLayout();

	if ( m_pItemOptionPanel->IsVisible() )
	{
		m_pItemOptionPanel->UpdateItemOptionsUI();
	}
	*/
}
/* functions of interest from client/econ/
* m_pSelectionPanel is what live uses when you click on a weapon slot. here we handle that in this class
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseLoadoutPanel::AddNewItemPanel( int iPanelIndex )
{
	CItemModelPanel* pPanel = vgui::SETUP_PANEL( new CItemModelPanel( this, VarArgs( "modelpanel%d", iPanelIndex ) ) );
	pPanel->SetActAsButton( true, true );
	m_pItemModelPanels.AddToTail( pPanel );

#ifdef STAGING_ONLY
	if ( tf_use_card_tooltips.GetBool() )
	{
		pPanel->SetTooltip( m_pMouseOverCardTooltip, "" );
	}
	else
#endif
		pPanel->SetTooltip( m_pMouseOverTooltip, "" );

	Assert( iPanelIndex == (m_pItemModelPanels.Count() - 1) );
}


*/