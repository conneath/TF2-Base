//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef ITEM_MODEL_PANEL_H
#define ITEM_MODEL_PANEL_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Panel.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/Tooltip.h>
#include "basemodel_panel.h"
#include "basemodelpanel.h"
#include "tf_item_schema.h"
#include "tf_controls.h"

//-----------------------------------------------------------------------------
// Purpose: The model panel that's embedded inside the CItemModelPanel vgui panel
//-----------------------------------------------------------------------------
class CEmbeddedItemModelPanel : public CBaseModelPanel
{
	DECLARE_CLASS_SIMPLE( CEmbeddedItemModelPanel, CBaseModelPanel );

public:
	CEmbeddedItemModelPanel( vgui::Panel* pParent, const char* pName );
	//virtual ~CEmbeddedItemModelPanel();
	void	SetEconItem( CEconItemView* pItem );
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CItemModelPanel : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CItemModelPanel, vgui::EditablePanel );
public:
	CItemModelPanel( vgui::Panel* parent, const char* name );
	//virtual ~CItemModelPanel( void );

	virtual void	ApplySchemeSettings( vgui::IScheme* pScheme ) OVERRIDE;
	virtual void	ApplySettings( KeyValues* inResourceData ) OVERRIDE;
	virtual void	PerformLayout( void ) OVERRIDE;

	void	SetEconItem( CEconItemView* pItem );
protected:
	CEmbeddedItemModelPanel* m_pEmbItemModelPanel;
	CTFLabel* m_pNameLabel;
	CTFLabel* m_pAttribLabel;

	CPanelAnimationVarAliasType( int, m_nModelX, "model_xpos", "0", "proportional_xpos" );
	CPanelAnimationVarAliasType( int, m_nModelWidth, "model_wide", "140", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_nModelTall, "model_tall", "100", "proportional_int" );

	CPanelAnimationVarAliasType( int, m_nTextX, "text_xpos", "0", "proportional_xpos" );
	CPanelAnimationVarAliasType( int, m_nTextWide, "text_wide", "140", "proportional_int" );
	CPanelAnimationVarAliasType( bool, m_bTextCenter, "text_center", "0", "bool" );
};
#endif