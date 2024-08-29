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
	//m_pTauntHintLabel = dynamic_cast<vgui::Label*>(FindChildByName( "TauntHintLabel" ));
	//m_pTauntLabel = dynamic_cast<CExLabel*>(FindChildByName( "TauntLabel" ));
	//m_pTauntCaratLabel = dynamic_cast<CExLabel*>(FindChildByName( "TauntCaratLabel" ));
	//m_pBuildablesButton = dynamic_cast<CExButton*>(FindChildByName( "BuildablesButton" ));
	//m_pCharacterLoadoutButton = dynamic_cast<CExImageButton*>(FindChildByName( "CharacterLoadoutButton" ));
	//m_pTauntLoadoutButton = dynamic_cast<CExImageButton*>(FindChildByName( "TauntLoadoutButton" ));
	//m_pPassiveAttribsLabel = dynamic_cast<CExLabel*>(FindChildByName( "PassiveAttribsLabel" ));
	//m_pLoadoutPresetPanel = dynamic_cast<CLoadoutPresetPanel*>(FindChildByName( "loadout_preset_panel" ));
	//m_pPresetsExplanationPopup = dynamic_cast<CExplanationPopup*>(FindChildByName( "PresetsExplanation" ));
	//m_pTauntsExplanationPopup = dynamic_cast<CExplanationPopup*>(FindChildByName( "TauntsExplanation" ));
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
		// conn - this is temporary and Poopoocaca
		if ( m_pClassLabel )
		{
			m_pClassLabel->SetText( g_aPlayerClassNames[m_iCurrentClassIndex] );	
		}
		/*
		if ( m_pBuildablesButton )
		{
			m_pBuildablesButton->SetVisible( m_iCurrentClassIndex == TF_CLASS_ENGINEER );
		}
		*/
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
	/*
	// Search for a Robot Costume
	bool bIsRobot = false;
	static CSchemaAttributeDefHandle pAttrDef_PlayerRobot( "appear as mvm robot" );
	// For now, fill them out with the local player's currently wielded items
	for ( int i = 0; i < m_pItemModelPanels.Count(); i++ )
	{
		CEconItemView* pItemData = TFInventoryManager()->GetItemInLoadoutForClass( m_iCurrentClassIndex, i );
		if ( !pItemData )
			continue;
		if ( FindAttribute( pItemData, pAttrDef_PlayerRobot ) )
		{
			bIsRobot = true;
			break;
		}
	}
	*/
	// We're showing the loadout for a specific class.
	TFPlayerClassData_t* pData = GetPlayerClassData( m_iCurrentClassIndex );
	if ( m_pPlayerModelPanel )
	{
		//m_pPlayerModelPanel->ClearCarriedItems();
		//m_pPlayerModelPanel->SetToPlayerClass( m_iCurrentClassIndex, bIsRobot );
		//m_pPlayerModelPanel->SetTeam( m_iCurrentTeamIndex );
		// conn - TODO: this is a placeholder right now, and it doesn't work when class data hasn't been loaded yet (e.g. before entering a server)
#ifdef DEBUG
		DevMsg( "%s\n", pData->GetModelName() );
#endif
		m_pPlayerModelPanel->SetMDL(pData->GetModelName());
		//m_pPlayerModelPanel->SetSkin();
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