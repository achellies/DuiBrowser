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
// BCFileChooserEA.cpp
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "FileChooser.h"
#include "CString.h"
#include "Document.h"
#include "FrameView.h"
#include "Icon.h"
#include "StringTruncator.h"
#include <stdio.h>
#include <EAWebKit/EAWebKitConfig.h>
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/internal/EAWebKitViewHelper.h> // For multiview support

#if EAWEBKIT_DEFAULT_FILE_CHOOSER_HANDLING_ENABLED && defined(EA_PLATFORM_WINDOWS)
    #include <windows.h>
#endif


namespace OWBAL {


void FileChooser::openFileChooser(Document* document)
{
    //OWB_PRINTF("FileChooser::openFileChooser\n");

    // We use this protector because some systems implement asynchronous dialogs  
    // which could result in this class being deleted while it's executing.
    RefPtr<FileChooser> protector(this);

    // What we need to do here is implement an "open existing file for reading" dialog box.
    // Desktop platforms tend to support this natively, though a custom application may
    // wnat to override this. Upon successfully selecting a file, we need to call the
    // FileChooser::chooseFile(const String&) function with the path.

    // To do: Call back the user-installed file chooser here.

    bool bHandled = false;
    EA::WebKit::ViewNotification* pVN = EA::WebKit::GetViewNotification();

    if(pVN)
    {
        EA::WebKit::View* pView = EA::WebKit::AutoSetActiveView::GetActiveView();
        
        EA::WebKit::FileChooserInfo fci; 
        
        fci.mpView = pView;             
        fci.mbIsLoad     = true;
        fci.mFilePath[0] = 0;
        fci.mbSuccess    = false;

		if (pVN->ChooseFile(fci))
		{
			bHandled = true;
			const String resultFilename(fci.mFilePath); 
			chooseFile(resultFilename);
		}
    }

    if(!bHandled)
    {
        #if EAWEBKIT_DEFAULT_FILE_CHOOSER_HANDLING_ENABLED && defined(EA_PLATFORM_WINDOWS)

            char          filebuffer[256];
            OPENFILENAMEA ofn;

            memset(&ofn, 0, sizeof(ofn));

            ofn.lStructSize     = sizeof(OPENFILENAME);
            ofn.hwndOwner       = NULL; // Parent window handle
            ofn.lpstrFilter     = "All Files\0*.*\0";
            ofn.nFilterIndex    = 0;
            ofn.lpstrFile       = filebuffer;
            ofn.nMaxFile        = sizeof(filebuffer);
            ofn.lpstrInitialDir = NULL;
            ofn.lpstrTitle      = "Open File";
            ofn.Flags           = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

            BOOL result = GetOpenFileNameA(&ofn);

            if(result)
            {
                const String resultFilename(filebuffer);
                chooseFile(resultFilename); // Call our member chooseFile function.
            }

        #endif
    }
}


String FileChooser::basenameForWidth(const Font& /*font*/, int width) const
{
    //OWB_PRINTF("FileChooser::basenameForWidth\n");

    // This returns the file name portion of m_filename truncated to the size that fits within 'width'.

    if (width > 0)
    {
        // To do: need to at least get the file name portion of this.
        return m_filename;  
    }

    /*  Gtk implementation
    if (width > 0)
    {
        String string = fileButtonNoFileSelectedLabel();  // e.g. "(None)"

        if (!m_filename.isEmpty())
        {
            gchar* systemFilename = g_filename_from_utf8(m_filename.utf8().data(), -1, 0, 0, 0);

            if (systemFilename)
            {
                gchar* systemBasename = g_path_get_basename(systemFilename);
                g_free(systemFilename);

                stringByAdoptingFileSystemRepresentation(systemBasename, string);
            }
        }

        return StringTruncator::centerTruncate(string, width, font, false);
    }
    */

    return String();
}


}
