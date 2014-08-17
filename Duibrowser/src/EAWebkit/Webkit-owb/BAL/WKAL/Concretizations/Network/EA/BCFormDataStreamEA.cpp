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
// BCFormDataStreamEA.h
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////

#include "config.h"

// We need to define __STDC_LIMIT_MACROS to define SIZE_MAX.
#ifndef __STDC_LIMIT_MACROS
    #define __STDC_LIMIT_MACROS
    #include <stdint.h>
    // #undef __STDC_LIMIT_MACROS    Disabled by Paul Pedriana because it causes a compiler warning.
#else
    #include <stdint.h>
#endif

#include "BCFormDataStreamEA.h"

#include "CString.h"
#include "FormData.h"
#include "ResourceRequest.h"

namespace WKAL {


FormDataStream::~FormDataStream()
{
    using namespace EA::WebKit;

    if (m_file != FileSystem::kFileObjectInvalid)
    {
        FileSystem* pFS = GetFileSystem();
        EAW_ASSERT(pFS != NULL);

        if(pFS)
        {
            pFS->CloseFile(m_file);
            pFS->DestroyFileObject(m_file);
        }
    }
}

size_t FormDataStream::read(void* ptr, size_t blockSize, size_t numberOfBlocks)
{
    using namespace EA::WebKit;

    // Check for overflow.
    if (!numberOfBlocks || (blockSize > (SIZE_MAX / numberOfBlocks)))
        return 0;

    Vector<FormDataElement> elements;

    if(m_resourceHandle->request().httpBody())
        elements = m_resourceHandle->request().httpBody()->elements();

    if(m_formDataElementIndex >= elements.size())
        return 0;

    const FormDataElement& element          = elements[m_formDataElementIndex];
    WebCore::CString       sElementFileName = element.m_filename.utf8();
    const char*            pPath            = sElementFileName.data();
    size_t                 toSend           = blockSize * numberOfBlocks;
    size_t                 sent             = 0;

    if(element.m_type == FormDataElement::encodedFile)
    {
        EA::WebKit::FileSystem* pFS = GetFileSystem();
        EAW_ASSERT(pFS != NULL);

        if(pFS)
        {
            if(m_file == FileSystem::kFileObjectInvalid)
            {
                m_file = pFS->CreateFileObject();

                if(m_file != FileSystem::kFileObjectInvalid)
                {
                    if(!pFS->OpenFile(m_file, pPath, FileSystem::kRead))
                    {
                        pFS->DestroyFileObject(m_file);
                        m_file = FileSystem::kFileObjectInvalid;
                    }
                }
            }

            if(m_file != FileSystem::kFileObjectInvalid)
            {
                EAW_ASSERT_FORMATTED(m_file != FileSystem::kFileObjectInvalid, "FormDataStream::read: Failed while trying to open %s for upload\n", pPath);
                return 0;
            }

            const int64_t readSize = pFS->ReadFile(m_file, ptr, (int64_t)toSend);

            if(readSize < 0)  // If there was error...
            {
                EAW_ASSERT_FORMATTED(readSize < 0, "FormDataStream::read: Failed while trying to read %s for upload\n", pPath);
                return 0;
            }

            sent = (size_t)readSize;

            if(pFS->GetFilePosition(m_file) == pFS->GetFileSize(m_file))  // If done reading the file...
            {
                pFS->CloseFile(m_file);
                pFS->DestroyFileObject(m_file);
                m_file = FileSystem::kFileObjectInvalid;

                m_formDataElementIndex++;
            }
        }
    } 
    else
    {
        const size_t elementSize = element.m_data.size() - m_formDataElementDataOffset;

        sent = (elementSize > toSend) ? toSend : elementSize;
        memcpy(ptr, element.m_data.data() + m_formDataElementDataOffset, sent);

        if (elementSize > sent)
            m_formDataElementDataOffset += sent;
        else
        {
            m_formDataElementDataOffset = 0;
            m_formDataElementIndex++;
        }
    }

    return sent;
}

} // namespace WebCore
