/*
Copyright (C) 2008-2010 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

///////////////////////////////////////////////////////////////////////////////
// BCClipboardEA.h
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#ifndef BCClipboardEA_h
#define BCClipboardEA_h


#include "Clipboard.h"
#include <EABase/eabase.h>

#if defined(DISABLED_EA_PLATFORM_WINDOWS)  // The Windows-specific clipboard is disabled because we need to do some work on it for it to compile for us in our environment.
    #include "CachedResourceClient.h"
    #include "COMPtr.h"
#else
    #include "BALBase.h"
#endif


struct IDataObject;


namespace WKAL {
    class CachedImage;
    class IntPoint;
    class WCDataObject;


    #if defined(DISABLED_EA_PLATFORM_WINDOWS)

        class ClipboardWin : public Clipboard, public CachedResourceClient {
        public:
            static PassRefPtr<ClipboardWin> create(bool isForDragging, IDataObject* dataObject, ClipboardAccessPolicy policy)
            {
                return adoptRef(new ClipboardWin(isForDragging, dataObject, policy));
            }
            static PassRefPtr<ClipboardWin> create(bool isForDragging, WCDataObject* dataObject, ClipboardAccessPolicy policy)
            {
                return adoptRef(new ClipboardWin(isForDragging, dataObject, policy));
            }
            ~ClipboardWin();
        
            void clearData(const String& type);
            void clearAllData();
            String getData(const String& type, bool& success) const;
            bool setData(const String& type, const String& data);
        
            // extensions beyond IE's API
            HashSet<String> types() const;
        
            void setDragImage(CachedImage*, const IntPoint&);
            void setDragImageElement(Node*, const IntPoint&);

            virtual DragImageRef createDragImage(IntPoint& dragLoc) const;
            virtual void declareAndWriteDragImage(Element*, const KURL&, const String& title, Frame*);
            virtual void writeURL(const KURL&, const String&, Frame*);
            virtual void writeRange(Range*, Frame*);

            virtual bool hasData();

            COMPtr<IDataObject> dataObject() { return m_dataObject; }

        private:
            ClipboardWin(bool isForDragging, IDataObject*, ClipboardAccessPolicy);
            ClipboardWin(bool isForDragging, WCDataObject*, ClipboardAccessPolicy);

            void resetFromClipboard();
            void setDragImage(CachedImage*, Node*, const IntPoint&);

            COMPtr<IDataObject> m_dataObject;
            COMPtr<WCDataObject> m_writableDataObject;
            Frame* m_frame;
        };

    #else

        class ClipboardBal : public Clipboard, public WKALBase {
        public:
            ClipboardBal(ClipboardAccessPolicy, bool);
            ~ClipboardBal();


            void clearData(const String&);
            void clearAllData();
            String getData(const String&, bool&) const;
            bool setData(const String&, const String&);

            HashSet<String> types() const;
            IntPoint dragLocation() const;
            CachedImage* dragImage() const;
            void setDragImage(CachedImage*, const IntPoint&);
            Node* dragImageElement();
            void setDragImageElement(Node*, const IntPoint&);

            virtual DragImageRef createDragImage(IntPoint&) const;
            virtual void declareAndWriteDragImage(Element*, const KURL&, const String&, Frame*);
            virtual void writeURL(const KURL&, const String&, Frame*);
            virtual void writeRange(Range*, Frame*);

            virtual bool hasData();
        };

    #endif
}

#endif // Header include guard
