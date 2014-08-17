/*
Copyright (C) 2009-2010 Electronic Arts, Inc.  All rights reserved.

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
// EATextBaseline.h
//
// Copyright (c) 2006 Electronic Arts. All Rights Reserved.
// Maintained by Paul Pedriana and Talin.
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// This module implements an adapter to Typesetter which causes text to 
// follow an arbitrary baseline path as opposed to a horizontal line.
///////////////////////////////////////////////////////////////////////////////


#ifndef EATEXT_EATEXTBASELINE_H
#define EATEXT_EATEXTBASELINE_H


#include <EABase/EABase.h>
#include <EAText/EAText.h>
#include <EAText/EATextTypesetter.h>


namespace EA
{
    namespace Text
    {
        /// GlyphLayoutInfoEx
        ///
        /// Provides additional information for non-axis-aligned glyphs in 2D.
        /// This is useful for drawing non-uniformly rotated glyphs in a way that
        /// can't be solved with a single tranform for a glyph array. An example
        /// of this is a line of text that follows a curve of some sort.
        ///
        /// We follow the following conventions, demonstrated with a tilted M glyph.
        ///
        ///             [x2,y2]
        /// [x1,y1]    *
        ///       *    **
        ///        ** *  *  
        ///         *     * [x3,y3]
        ///          *
        ///        [x4,y4]
        ///
        struct GlyphLayoutInfoEx : public GlyphLayoutInfo // 48 bytes
        {
            float mfX3;
            float mfY3;
            float mfX4;
            float mfY4;
        };


        /// BaselineIterator
        ///
        /// Generic baseline position iterator.
        /// It is used by the BaselineLayout class to set glyph positions.
        ///
        /// The 'T' value of a line/curve refers to a value in the range
        /// of [0,1] and which defines a position along the curve from its
        /// beginning to its end.
        ///
        class BaselineIterator
        {
        public:
            virtual ~BaselineIterator() { }

            virtual float GetT() const = 0;
            virtual void  SetT(float fT) = 0;

            virtual float GetDistance() const = 0;
            virtual void  SetDistance(float fDistance) = 0;
            virtual void  MoveDistance(float fDistance) = 0;

            virtual float GetBaselineLength() const = 0;
            virtual void  GetPosition(Point& pPoint, float& fTangentAngle) = 0;
        };


    } // namespace Text

} // namespace EA




#endif // Header include guard








