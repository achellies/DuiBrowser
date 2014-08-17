/*
 * Copyright (C) 2008 Pleyo.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Pleyo nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY PLEYO AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PLEYO OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
* This file was modified by Electronic Arts Inc Copyright © 2008-2010
*/

///////////////////////////////////////////////////////////////////////////////
// BCContextMenuItemEA.cpp
// Modified by Paul Pedriana - 2008
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "ContextMenu.h"
#include "ContextMenuItem.h"
#include "CString.h"

#include <stdio.h>

#define WEBKIT_CONTEXT_MENU_ACTION "webkit-context-menu"

namespace WKAL {

// Extract the ActionType from the menu item
ContextMenuItem::ContextMenuItem(BalMenuItem* item)
    : m_platformDescription()
{
    // OWB_PRINTF("ContextMenuItem::ContextMenuItem\n");
}

ContextMenuItem::ContextMenuItem(ContextMenu*)
{
    // OWB_PRINTF("ContextMenuItem::ContextMenuItem\n");
    NotImplemented();
}

ContextMenuItem::ContextMenuItem(ContextMenuItemType type, ContextMenuAction action, const String& title, ContextMenu* subMenu)
{
    // OWB_PRINTF("ContextMenuItem::ContextMenuItem\n");
    m_platformDescription.type = type;
    m_platformDescription.action = action;
    m_platformDescription.title = title;

    setSubMenu(subMenu);
}

ContextMenuItem::~ContextMenuItem()
{
    // OWB_PRINTF("ContextMenuItem::~ContextMenuItem\n");
}

BalMenuItem* ContextMenuItem::createNativeMenuItem(const PlatformMenuItemDescription& menu)
{
    // OWB_PRINTF("ContextMenuItem::createNativeMenuItem\n");
    return 0;
}

PlatformMenuItemDescription ContextMenuItem::releasePlatformDescription()
{
    // OWB_PRINTF("ContextMenuItem::releasePlatformDescription\n");
    PlatformMenuItemDescription description = m_platformDescription;
    m_platformDescription = PlatformMenuItemDescription();
    return description;
}

ContextMenuItemType ContextMenuItem::type() const
{
    // OWB_PRINTF("ContextMenuItem::type\n");
    return m_platformDescription.type;
}

void ContextMenuItem::setType(ContextMenuItemType type)
{
    // OWB_PRINTF("ContextMenuItem::setType\n");
    m_platformDescription.type = type;
}

ContextMenuAction ContextMenuItem::action() const
{
    // OWB_PRINTF("ContextMenuItem::action\n");
    return m_platformDescription.action;
}

void ContextMenuItem::setAction(ContextMenuAction action)
{
    // OWB_PRINTF("ContextMenuItem::setAction\n");
    m_platformDescription.action = action;
}

String ContextMenuItem::title() const
{
    // OWB_PRINTF("ContextMenuItem::title\n");
    NotImplemented();
    return String();
}

void ContextMenuItem::setTitle(const String&)
{
    // OWB_PRINTF("ContextMenuItem::setTitle\n");
    NotImplemented();
}

PlatformMenuDescription ContextMenuItem::platformSubMenu() const
{
    // OWB_PRINTF("ContextMenuItem::platformSubMenu\n");
    return m_platformDescription.subMenu;
}

void ContextMenuItem::setSubMenu(ContextMenu* menu)
{
    // OWB_PRINTF("ContextMenuItem::setSubMenu\n");
}

void ContextMenuItem::setChecked(bool shouldCheck)
{
    // OWB_PRINTF("ContextMenuItem::setChecked\n");
    m_platformDescription.checked = shouldCheck;
}

void ContextMenuItem::setEnabled(bool shouldEnable)
{
    // OWB_PRINTF("ContextMenuItem::setEnabled\n");
    m_platformDescription.enabled = shouldEnable;
}

}
