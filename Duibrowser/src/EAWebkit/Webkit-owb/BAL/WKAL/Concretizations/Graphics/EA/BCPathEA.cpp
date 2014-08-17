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
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#include "config.h"
#include "Path.h"

#include "AffineTransform.h"
#include "FloatRect.h"
#include "NotImplemented.h"
#include "PlatformString.h"

#include <math.h>
#include <wtf/MathExtras.h>

namespace WKAL {

Path::Path()
//    : m_path(new CairoPath())
{
    //OWB_PRINTF("Path::Path\n");
}

Path::~Path()
{
    //OWB_PRINTF("Path::~Path\n");
    //delete m_path;
}

Path::Path(const Path& other)
//    : m_path(new CairoPath())
{
    //OWB_PRINTF("Path::Path copy\n");
}

Path& Path::operator=(const Path& other)
{
    //OWB_PRINTF("Path::operator=\n");
    if (&other == this)
        return *this;

    clear();

    return *this;
}

void Path::clear()
{
    //OWB_PRINTF("Path::clear\n");
}

bool Path::isEmpty() const
{
    //OWB_PRINTF("Path::isEmpty\n");
    return true;
}

void Path::translate(const FloatSize& p)
{
    //OWB_PRINTF("Path::translate\n");
}

void Path::moveTo(const FloatPoint& p)
{
    //OWB_PRINTF("Path::moveTo\n");
}

void Path::addLineTo(const FloatPoint& p)
{
    //OWB_PRINTF("Path::addLineTo\n");
}

void Path::addRect(const FloatRect& rect)
{
    //OWB_PRINTF("Path::addRect\n");
}

/*
 * inspired by libsvg-cairo
 */
void Path::addQuadCurveTo(const FloatPoint& controlPoint, const FloatPoint& point)
{
    //OWB_PRINTF("Path::addQuadCurveTo\n");
}

void Path::addBezierCurveTo(const FloatPoint& controlPoint1, const FloatPoint& controlPoint2, const FloatPoint& controlPoint3)
{
    //OWB_PRINTF("Path::addBezierCurveTo\n");

}

void Path::addArc(const FloatPoint& p, float r, float sa, float ea, bool anticlockwise)
{
    //OWB_PRINTF("Path::addArc\n");
}

void Path::addArcTo(const FloatPoint& p1, const FloatPoint& p2, float radius)
{
    //OWB_PRINTF("Path::addArcTo\n");
    // FIXME: cairo_arc_to not yet in cairo see cairo.h
    // cairo_arc_to(m_cr, p1.x(), p1.y(), p2.x(), p2.y());
    notImplemented();
}

void Path::addEllipse(const FloatRect& rect)
{
    //OWB_PRINTF("Path::addEllipse\n");
}

void Path::closeSubpath()
{
    //OWB_PRINTF("Path::closeSubpath\n");
}

FloatRect Path::boundingRect() const
{
    //OWB_PRINTF("Path::boundingRect\n");
    return FloatRect();
}

bool Path::contains(const FloatPoint& point, WindRule rule) const
{
    //OWB_PRINTF("Path::contains\n");
    return false;
}

void Path::apply(void* info, PathApplierFunction function) const
{
    //OWB_PRINTF("Path::apply\n");
}

void Path::transform(const AffineTransform& trans)
{
    //OWB_PRINTF("Path::transform\n");
}

String Path::debugString() const
{
    //OWB_PRINTF("Path::debugString\n");
    String string = "";
    
    return string;
}

} // namespace WebCore
