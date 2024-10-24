//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef ITEM_SELECTION_PANEL_H
#define ITEM_SELECTION_PANEL_H
#ifdef _WIN32
#pragma once
#endif

#include "vgui_controls/EditablePanel.h"
#include "vgui_controls/ScrollableEditablePanel.h"
#include "tf_controls.h"
//#include "character_info_panel.h"

class CItemModelPanel;

class CItemSelectionPanel : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CItemSelectionPanel, vgui::EditablePanel );
public:
	CItemSelectionPanel( vgui::Panel* parent );
	virtual void ApplySchemeSettings( vgui::IScheme* pScheme );
	virtual void OnCommand( const char* command );
	virtual void	ApplySettings( KeyValues* inResourceData ) OVERRIDE;
	virtual void	PerformLayout( void ) OVERRIDE;

	void	SetClassAndSlot( int iClass, int iSlot );
	void	BuildItemPanels();
protected:
	int						m_iCurrentClassIndex;
	int						m_iCurrentSlotIndex;
	int						m_iSelectedPreset;
	vgui::EditablePanel*	m_pItemContainer;
	vgui::Label* m_pSlotLabel;
	CUtlVector<CItemModelPanel*> m_vecItemPanels;
	CUtlVector<vgui::Button*>	m_vecChangeButtons;

	CPanelAnimationVarAliasType( int, m_nButtonXPos, "button_xpos", "420", "proportional_xpos" );
	CPanelAnimationVarAliasType( int, m_nItemX, "itempanel_xpos", "130", "proportional_xpos" );
	CPanelAnimationVarAliasType( int, m_nItemYDelta, "itempanel_ydelta", "16", "proportional_ypos" );
};

#endif