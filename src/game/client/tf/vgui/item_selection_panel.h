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
//#include "character_info_panel.h"

class CItemModelPanel;

class CItemSelectionPanel : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CItemSelectionPanel, vgui::EditablePanel );
public:
	CItemSelectionPanel( vgui::Panel* parent, const char* name );
	virtual void ApplySchemeSettings( vgui::IScheme* pScheme );
	virtual void OnCommand( const char* command );

	void	SetClassAndSlot( int iClass, int iSlot );
	void	BuildItemPanels();
protected:
	int						m_iCurrentClassIndex;
	int						m_iCurrentSlotIndex;
};

#endif