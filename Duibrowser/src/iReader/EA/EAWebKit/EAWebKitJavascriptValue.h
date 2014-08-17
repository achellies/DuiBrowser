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
// EAWebKitJavascriptValue.h
//
// Created by Chris Stott
///////////////////////////////////////////////////////////////////////////////


#ifndef EAWEBKIT_EAWEBKITJAVASCRIPTVALUE_H
#define EAWEBKIT_EAWEBKITJAVASCRIPTVALUE_H

#include <EAWebKit/EAWebkitSTLWrapper.h>

namespace EA
{
    namespace WebKit
    {
		//////////////////////////////////////////////////////////////////////////
		//
		enum JavascriptValueType
		{
			JavascriptValueType_Undefined,
			JavascriptValueType_Number,
			JavascriptValueType_String,
			JavascriptValueType_Boolean,
            JavascriptValueType_Array,
            JavascriptValueType_Object,
		};

		//////////////////////////////////////////////////////////////////////////
		// This is incredibly simple & stupid. Expect very few of these
		// objects to be created
		// 
		// Also, don't hold onto these - extract what you want immediately.
		class JavascriptValue
		{
		public:
			//////////////////////////////////////////////////////////////////////////
			// Constructors
			explicit	JavascriptValue()
			:	mType(JavascriptValueType_Undefined)
			{
				
			}


			explicit	JavascriptValue(double value)
			:	mType(JavascriptValueType_Number)
			,	mNumberValue(value)
			{
				// empty
			}

// CSidhall 3/24/10 - Removed now for would only work now for internal EAWebKit calls because of the added fixed string = operator
/*
			explicit	JavascriptValue(const EASTLFixedString16Wrapper& value)
			:	mType(JavascriptValueType_String)
			{
				mStringValue =value;
			}	
*/
			explicit	JavascriptValue(bool value)
			:	mType(JavascriptValueType_Boolean)
			,	mBooleanValue(value)
			{
				// empty
			}	

			//////////////////////////////////////////////////////////////////////////
			// Type
			JavascriptValueType GetType() const
			{
				return mType;
			}


			//////////////////////////////////////////////////////////////////////////
			// Setters
			void SetUndefined()
			{
				mType = JavascriptValueType_Undefined;
			}

			void SetNumberValue(double value)
			{
				mType = JavascriptValueType_Number;
				mNumberValue = value;
			}

            // Note for setting a string: For setting the string value, this needs now 2 steps (in no particular order):
            // 1) The fixed mStringValue needs to be set with SetCharacters() after getting it with GetStringValue() 
            // 2) The String type needs to be set with SetStringType 
            void SetStringType()
			{
				mType = JavascriptValueType_String;
            }

			void SetBooleanValue(bool value)
			{
				mType = JavascriptValueType_Boolean;
				mBooleanValue = value;
			}

            // Similar to string. First use SetJavaScriptValues() after getting it with GetArrayValue(),
            // then call SetArrayType()
            void SetArrayType()
            {
                mType = JavascriptValueType_Array;
            }

            void SetObjectType()
            {
                mType = JavascriptValueType_Object;
            }

			//////////////////////////////////////////////////////////////////////////
			// Getters
			double GetNumberValue() const
			{
				return mNumberValue;
			}

            // Note: To convert the fixed string, you will need to use the GetCharacters() API 
			EASTLFixedString16Wrapper& GetStringValue()
			{
				return mStringValue;
			}

            const EASTLFixedString16Wrapper& GetStringValue() const
            {
                return mStringValue;
            }

			bool GetBooleanValue() const
			{
				return mBooleanValue;
			}

            EASTLVectorJavaScriptValueWrapper& GetArrayValue()
            {
                return mArrayValue;
            }

            const EASTLVectorJavaScriptValueWrapper& GetArrayValue() const
            {
                return mArrayValue;
            }

            EASTLJavascriptValueHashMapWrapper& GetHashMapValue()
            {
                return mHashMapValue;
            }

            const EASTLJavascriptValueHashMapWrapper& GetHashMapValue() const
            {
                return GetHashMapValue();
            }

            

		private:
			JavascriptValueType		mType;
			union 
			{
				double					mNumberValue;
				bool					mBooleanValue;
            };

			// TODO: dsiems: Investigate if there's another way to fix this problem, having the fixed string
			// here bloats every instance of this class by ~217 bytes.  
			// Ideally this class would stay in the 8-24 byte range.
            EASTLFixedString16Wrapper mStringValue;  // CSidhall 3/24/10 - changed from a pointer to fixed 
                                                     // string wrapper to fix problem with multiple strings 
                                                     // set in a same callback and getting stomped.
            EASTLVectorJavaScriptValueWrapper mArrayValue;
            EASTLJavascriptValueHashMapWrapper mHashMapValue;
		};
    } // namespace WebKit

} // namespace EA


#endif // EAWEBKIT_EAWEBKITJAVASCRIPTVALUE_H










