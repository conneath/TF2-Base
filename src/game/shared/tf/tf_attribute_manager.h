//================================================================
// Goldrush Attribute Manager - powered by conneath industries ltd co tm
// credit to the tf2c team for making the original econ system
// that every tf2 mod seems to use these days (including us)
//================================================================
#ifndef TF_ATTRIBUTE_MANAGER_H
#define TF_ATTRIBUTE_MANAGER_H

#ifdef _WIN32
#pragma once
#endif

#include "gamestringpool.h"
#include "networkvar.h"

// Client specific.
#ifdef CLIENT_DLL
EXTERN_RECV_TABLE( DT_TFAttributeManager );
EXTERN_RECV_TABLE( DT_TFAttributeContainer );
// Server specific.
#else
EXTERN_SEND_TABLE( DT_TFAttributeManager );
EXTERN_SEND_TABLE( DT_TFAttributeContainer );
#endif

class CTFAttributeManager
{
public:
	DECLARE_EMBEDDED_NETWORKVAR();
	DECLARE_CLASS_NOBASE( CTFAttributeManager );

	CTFAttributeManager();


};
#endif // TF_ATTRIBUTE_MANAGER_H