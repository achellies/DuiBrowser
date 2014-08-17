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

#ifndef DefaultPolicyDelegate_h
#define DefaultPolicyDelegate_h

#include <wtf/FastAllocBase.h>
class WebActionPropertyBag;
class WebFramePolicyListener;
class WebActionPropertyBag;
class WebMutableURLRequest;
class WebView;
class WebFrame;
class WebError;
class WebActionPropertyBag;
namespace WebCore {
    class String;
}


typedef enum WebNavigationType {
    WebNavigationTypeLinkClicked,
    WebNavigationTypeFormSubmitted,
    WebNavigationTypeBackForward,
    WebNavigationTypeReload,
    WebNavigationTypeFormResubmitted,
    WebNavigationTypeOther
} WebNavigationType;

#define WebActionNavigationTypeKey "WebActionNavigationTypeKey"
#define WebActionElementKey "WebActionElementKey"
#define WebActionButtonKey "WebActionButtonKey"
#define WebActionModifierFlagsKey "WebActionModifierFlagsKey"
#define WebActionOriginalURLKey "WebActionOriginalURLKey"


class DefaultPolicyDelegate/*: public WTF::FastAllocBase*/ {
public:

    /**
     *  sharedInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static DefaultPolicyDelegate* sharedInstance();

    /**
     *  createInstance description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    static DefaultPolicyDelegate* createInstance();
private:

    /**
     *  DefaultPolicyDelegate description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    DefaultPolicyDelegate();
public:

    /**
     *  ~DefaultPolicyDelegate description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual ~DefaultPolicyDelegate();

    /**
     * decidePolicyForNavigationAction description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void decidePolicyForNavigationAction( 
        /* [in] */ WebView *webView,
        /* [in] */ WebActionPropertyBag *actionInformation,
        /* [in] */ WebMutableURLRequest *request,
        /* [in] */ WebFrame *frame,
        /* [in] */ WebFramePolicyListener *listener);
    
    /**
     * decidePolicyForNewWindowAction description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void decidePolicyForNewWindowAction( 
        /* [in] */ WebView *webView,
        /* [in] */ WebActionPropertyBag *actionInformation,
        /* [in] */ WebMutableURLRequest *request,
        /* [in] */ WebCore::String frameName,
        /* [in] */ WebFramePolicyListener *listener);
    
    /**
     * decidePolicyForMIMEType description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void decidePolicyForMIMEType( 
        /* [in] */ WebView *webView,
        /* [in] */ const WebCore::String type,
        /* [in] */ WebMutableURLRequest *request,
        /* [in] */ WebFrame *frame,
        /* [in] */ WebFramePolicyListener *listener);
    
    /**
     * unableToImplementPolicyWithError description
     * @param[in]: description
     * @param[out]: description
     * @code
     * @endcode
     */
    virtual void unableToImplementPolicyWithError( 
        /* [in] */ WebView *webView,
        /* [in] */ WebError *error,
        /* [in] */ WebFrame *frame);

    //+daw ca 29/07/2008 static and global management
    private:
        static DefaultPolicyDelegate* m_st_pSharedinstance;
    public:
        static void staticFinalize();
    //-daw ca
};

#endif // DefaultPolicyDelegate_h
