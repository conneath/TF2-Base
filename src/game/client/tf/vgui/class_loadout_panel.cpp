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
#include "vgui/IVGui.h"

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
	m_pItemSelectionPanel = new CItemSelectionPanel( this );
	m_pItemSelectionPanel->SetVisible( false );
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
	SetProportional( true );
	BaseClass::ApplySchemeSettings( pScheme );

	m_pPlayerModelPanel = dynamic_cast<CBaseModelPanel*>(FindChildByName( "classmodelpanel" ));
	m_pClassLabel = dynamic_cast<vgui::Label*>(FindChildByName( "ClassLabel" ));

	// temporary until econ is implemented, these should show when you're changing a weapon slot
	FindChildByName( "InventoryCount0" )->SetVisible( false );
	FindChildByName( "InventoryCount1" )->SetVisible( false );
	FindChildByName( "InventoryCount2" )->SetVisible( false );

	m_pChangeButtonPrimary = FindChildByName( "ChangeButton0" );
	m_pChangeButtonPrimary->SetVisible( true );
	FindChildByName( "ChangeButton1" )->SetVisible( true );
	FindChildByName( "ChangeButton2" )->SetVisible( true );
	m_pItemSelectionPanel->SetVisible( false );
	// hardcoding ftw
	m_pPrimaryWeaponPanel = dynamic_cast<CItemModelPanel*>(FindChildByName( "modelpanel0" ));
	m_pSecondaryWeaponPanel = dynamic_cast<CItemModelPanel*>(FindChildByName( "modelpanel1" ));
	m_pMeleeWeaponPanel = dynamic_cast<CItemModelPanel*>(FindChildByName( "modelpanel2" ));

	// tell users no items exist yet for now
	//dynamic_cast<vgui::Label*>(FindChildByName( "NoneAvailableReason" ))->SetText( "#NoItemsExistLong" );
	FindChildByName( "NoneAvailableReason" )->SetVisible( false );
	FindChildByName( "NoneAvailableTitle" )->SetVisible( false );
	FindChildByName( "NoneAvailableTitle2" )->SetVisible( false );
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
	if ( FStrEq( command, "change0" ) )
	{
		if ( m_pItemSelectionPanel )
		{
			//m_pItemSelectionPanel->SetVisible( true );
			m_pItemSelectionPanel->SetClassAndSlot( m_iCurrentClassIndex, TF_LOADOUT_SLOT_PRIMARY );
		}
	}
	if ( FStrEq( command, "change1" ) )
	{
		if ( m_pItemSelectionPanel )
		{
			//m_pItemSelectionPanel->SetVisible( true );
			m_pItemSelectionPanel->SetClassAndSlot( m_iCurrentClassIndex, TF_LOADOUT_SLOT_SECONDARY );
		}
	}
	if ( FStrEq( command, "change2" ) )
	{
		if ( m_pItemSelectionPanel )
		{
			//m_pItemSelectionPanel->SetVisible( true );
			m_pItemSelectionPanel->SetClassAndSlot( m_iCurrentClassIndex, TF_LOADOUT_SLOT_MELEE );
		}
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

	// Playermodel panel (todo: make this its own function)
	TFPlayerClassData_t* pData = GetPlayerClassData( m_iCurrentClassIndex );
	if ( m_pPlayerModelPanel )
	{
		m_pPlayerModelPanel->SetMDL(pData->GetModelName());
		m_pPlayerModelPanel->ClearMergeMDLs();
		//m_pPlayerModelPanel->SetSkin(0);

		CEconItemView* pPlayerModelItem = GetTFInventory()->GetItem( m_iCurrentClassIndex, TF_LOADOUT_SLOT_PRIMARY, GetTFInventory()->GetWeaponPreset( m_iCurrentClassIndex, TF_LOADOUT_SLOT_PRIMARY ) );
		MDLHandle_t hMDL = mdlcache->FindMDL( pPlayerModelItem->GetWorldDisplayModel() );
		if ( hMDL != MDLHANDLE_INVALID )
		{
			m_pPlayerModelPanel->SetMergeMDL( hMDL );
		}
	}

	// set our weapon panels to the items for our class from CTFInventory
	/////////////////
	// PRIMARY SLOT
	/////////////////
	if ( m_pPrimaryWeaponPanel )
	{
		m_pPrimaryWeaponPanel->InvalidateLayout(false, true); // ffs...
		m_pPrimaryWeaponPanel->SetEconItem( GetTFInventory()->GetItem( m_iCurrentClassIndex, TF_LOADOUT_SLOT_PRIMARY, GetTFInventory()->GetWeaponPreset( m_iCurrentClassIndex, TF_LOADOUT_SLOT_PRIMARY ) ) );
	}
	if ( GetTFInventory()->NumWeapons( m_iCurrentClassIndex, TF_LOADOUT_SLOT_PRIMARY ) > 1 ) // more than 1 weapon available in this class' slot? make the change button visible
	{
		m_pChangeButtonPrimary->SetVisible( true );
	}

	/////////////////
	// SECONDARY SLOT
	/////////////////
	if ( m_pSecondaryWeaponPanel )
	{
		m_pSecondaryWeaponPanel->InvalidateLayout( false, true );
		m_pSecondaryWeaponPanel->SetEconItem( GetTFInventory()->GetItem( m_iCurrentClassIndex, TF_LOADOUT_SLOT_SECONDARY, GetTFInventory()->GetWeaponPreset( m_iCurrentClassIndex, TF_LOADOUT_SLOT_SECONDARY ) ) );
	}
	if ( GetTFInventory()->NumWeapons( m_iCurrentClassIndex, TF_LOADOUT_SLOT_SECONDARY ) > 1 )
	{
		m_pChangeButtonPrimary->SetVisible( true );
	}

	////////////////
	// MELEE SLOT
	////////////////
	if ( m_pMeleeWeaponPanel ) // no PDA/Invis slots for now, sorry spy and engi
	{
		m_pMeleeWeaponPanel->InvalidateLayout( false, true );
		m_pMeleeWeaponPanel->SetEconItem( GetTFInventory()->GetItem( m_iCurrentClassIndex, TF_LOADOUT_SLOT_MELEE, GetTFInventory()->GetWeaponPreset( m_iCurrentClassIndex, TF_LOADOUT_SLOT_MELEE ) ) );
	}
	if ( GetTFInventory()->NumWeapons( m_iCurrentClassIndex, TF_LOADOUT_SLOT_MELEE ) > 1 )
	{
		m_pChangeButtonPrimary->SetVisible( true );
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