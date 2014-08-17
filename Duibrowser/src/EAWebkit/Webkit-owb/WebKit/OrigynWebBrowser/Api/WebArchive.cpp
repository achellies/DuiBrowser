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
* This file was modified by Electronic Arts Inc Copyright © 2009-2010
*/

#include "config.h"
#include "WebArchive.h"

#include <Archive.h>
#include "BAL/Includes/FakedDeepsee.h"

using namespace WebCore;

WebArchive* WebArchive::createInstance()
{
    WebArchive* instance = new WebArchive(0);
    return instance;
}

WebArchive* WebArchive::createInstance(PassRefPtr<Archive> coreArchive)
{
    WebArchive* instance = new WebArchive(coreArchive);

    return instance;
}

WebArchive::WebArchive(PassRefPtr<Archive> coreArchive)
    : m_archive(coreArchive)
{
    DS_CONSTRUCT();
}

WebArchive::~WebArchive()
{
    DS_DESTRUCT();
}

void WebArchive::initWithMainResource(WebResource* mainResource, WebResource** subResources, int cSubResources,  WebArchive** subFrameArchives, int cSubFrameArchives)
{
}

void WebArchive::initWithData(RefPtr<SharedBuffer>)
{
}

WebResource* WebArchive::mainResource()
{
    return 0;
}

RefPtr<SharedBuffer> WebArchive::data()
{
    RefPtr<SharedBuffer> buffer;

    return buffer;
}
