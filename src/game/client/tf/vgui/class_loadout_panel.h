//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef CLASS_LOADOUT_PANEL_H
#define CLASS_LOADOUT_PANEL_H
#ifdef _WIN32
#pragma once
#endif

//#include "base_loadout_panel.h"
//#include "tf_playermodelpanel.h"
//#include "item_selection_panel.h"
//#include <../common/GameUI/cvarslider.h>
#include <vgui/VGUI.h>
#include "vgui_controls/CheckButton.h"
#include "basemodel_panel.h"

#define NUM_ITEM_PANELS_IN_LOADOUT		CLASS_LOADOUT_POSITION_COUNT

class CLoadoutPresetPanel;
/*
class CLoadoutItemOptionsPanel : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CLoadoutItemOptionsPanel, vgui::EditablePanel );
public:
	CLoadoutItemOptionsPanel( Panel* parent, const char* pName );
	virtual void ApplySchemeSettings( vgui::IScheme* pScheme );
	virtual void PerformLayout( void );

	virtual void OnCommand( const char* command );
	virtual void OnMessage( const KeyValues* pParams, vgui::VPANEL hFromPanel );

	void SetItemSlot( loadout_positions_t eItemSlot, int iClassIndex );
	loadout_positions_t GetItemSlot() const { return m_eItemSlot; }
	void UpdateItemOptionsUI();

private:

	void AddControlsParticleEffect( void ) const;
	void AddControlsSetStyle( void ) const;
	CEconItemView* GetItem( void ) const;

	class vgui::PanelListPanel* m_pListPanel;
	CCvarSlider* m_pHatParticleSlider;
	CExButton* m_pSetStyleButton;
	vgui::CheckButton* m_pHatParticleUseHeadButton;

	int						m_iCurrentClassIndex;
	loadout_positions_t		m_eItemSlot;
};
*/

//-----------------------------------------------------------------------------
// A loadout screen that handles modifying the loadout of a specific class
//-----------------------------------------------------------------------------
class CClassLoadoutPanel : /*public CBaseLoadoutPanel*/ public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CClassLoadoutPanel, vgui::EditablePanel );
public:
	CClassLoadoutPanel( vgui::Panel* parent );
	~CClassLoadoutPanel();

	virtual void ApplySchemeSettings( vgui::IScheme* pScheme );
	//virtual void ApplySettings( KeyValues* inResourceData );
	virtual void OnCommand( const char* command );
	//virtual void ShowPanel( bool bShow );
	void		 ShowPanel( int iClass, bool bBackpack, bool bReturningFromArmory = false );
	virtual void UpdateModelPanels( void );
	//virtual void OnShowPanel( bool bVisible, bool bReturningFromArmory );
	virtual void PostShowPanel( bool bVisible );

	void		 SetClass( int iClass );
	void		 SetTeam( int iTeam );
protected:
	int						m_iCurrentClassIndex;
	int						m_iCurrentTeamIndex;
	int						m_iCurrentSlotIndex;
	CBaseModelPanel* m_pPlayerModelPanel;
	vgui::Label* m_pCaratLabel;
	vgui::Label* m_pClassLabel;
	//WIP
};

extern CClassLoadoutPanel* g_pClassLoadoutPanel;

#endif // CLASS_LOADOUT_PANEL_H
