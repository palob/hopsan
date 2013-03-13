/*-----------------------------------------------------------------------------
 This source file is part of Hopsan NG

 Copyright (c) 2011 
    Mikael Axin, Robert Braun, Alessandro Dell'Amico, Björn Eriksson,
    Peter Nordin, Karl Pettersson, Petter Krus, Ingo Staack

 This file is provided "as is", with no guarantee or warranty for the
 functionality or reliability of the contents. All contents in this file is
 the original work of the copyright holders at the Division of Fluid and
 Mechatronic Systems (Flumes) at Linköping University. Modifying, using or
 redistributing any part of this file is prohibited without explicit
 permission from the copyright holders.
-----------------------------------------------------------------------------*/

//!
//! @file   GUIConnectorAppearance.cpp
//! @author Flumes <flumes@lists.iei.liu.se>
//! @date   2010-01-01
//!
//! @brief Contains the GUIConnectorAppearance class
//!
//$Id$

#include "GUIConnectorAppearance.h"
#include "qdebug.h"
#include "Configuration.h"
#include "common.h"

ConnectorAppearance::ConnectorAppearance(QString type, GraphicsTypeEnumT gfxType)
{
    //Set the connector type and style
    setTypeAndIsoStyle(type, gfxType);     //Need to use set type instead of setting directly as setType narrows types down to power or signal
}

//! @brief Set the Connector type
void ConnectorAppearance::setStyle(const ConnectorStyleEnumT style)
{
    mConnectorStyle = style;
}

ConnectorStyleEnumT ConnectorAppearance::getStyle()
{
    return mConnectorStyle;
}

void ConnectorAppearance::setIsoStyle(GraphicsTypeEnumT gfxType)
{
    mGfxType = gfxType;
}

void ConnectorAppearance::setTypeAndIsoStyle(QString porttype, GraphicsTypeEnumT gfxType)
{
    if(porttype=="POWERPORT")
    {
        setStyle(PowerConnectorStyle);
    }
    else if(porttype=="READPORT" || "WRITEPORT")
    {
        setStyle(SignalConnectorStyle);
    }
    else
    {
        setStyle(UndefinedConnectorStyle);
    }
    setIsoStyle(gfxType);
}

QPen ConnectorAppearance::getPen(QString situation)
{
    return getPen(mConnectorStyle, mGfxType, situation);
}

void ConnectorAppearance::setCustomColor(QColor color)
{
    mCustomColor = color;
}

QColor ConnectorAppearance::getCustomColor()
{
    return mCustomColor;
}


//! Get function for primary pen style
QPen ConnectorAppearance::getPen(ConnectorStyleEnumT style, GraphicsTypeEnumT gfxType, QString situation)
{
    if(situation == "Primary" && mCustomColor != QColor())
    {
        QPen retval = gConfig.getPen(style, gfxType, situation);
        retval.setColor(mCustomColor);
        return retval;
    }
    return gConfig.getPen(style, gfxType, situation);
}
