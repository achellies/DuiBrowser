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
// BCAffineTransformEA.h
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#ifndef AffineTransform_h
#define AffineTransform_h

#include "BALBase.h"

namespace WKAL {

class IntPoint;
class IntRect;
class FloatPoint;
class FloatRect;

class AffineTransform : public WKALBase {
public:
    AffineTransform();
    AffineTransform(double a, double b, double c, double d, double e, double f);

    void setMatrix(double a, double b, double c, double d, double e, double f);
    void map(double x, double y, double *x2, double *y2) const;
    IntPoint mapPoint(const IntPoint&) const;
    FloatPoint mapPoint(const FloatPoint&) const;
    IntRect mapRect(const IntRect&) const;
    FloatRect mapRect(const FloatRect&) const;
    
    bool isIdentity() const;
    
    double a() const;
    void setA(double a);

    double b() const;
    void setB(double b);

    double c() const;
    void setC(double c);

    double d() const;
    void setD(double d);

    double e() const;
    void setE(double e);

    double f() const;
    void setF(double f);

    void reset();

    AffineTransform& multiply(const AffineTransform&);
    AffineTransform& scale(double); 
    AffineTransform& scale(double sx, double sy); 
    AffineTransform& scaleNonUniform(double sx, double sy);
    AffineTransform& rotate(double d);
    AffineTransform& rotateFromVector(double x, double y);
    AffineTransform& translate(double tx, double ty);
    AffineTransform& shear(double sx, double sy);
    AffineTransform& flipX();
    AffineTransform& flipY();
    AffineTransform& skew(double angleX, double angleY);
    AffineTransform& skewX(double angle);
    AffineTransform& skewY(double angle);
 
    double det() const;
    bool isInvertible() const;
    AffineTransform inverse() const;

    operator BalMatrix() const;
    bool operator==(const AffineTransform&) const;
    bool operator!=(const AffineTransform& other) const { return !(*this == other); }
    AffineTransform& operator*=(const AffineTransform&);
    AffineTransform operator*(const AffineTransform&);
    
private:
    BalMatrix m_transform;
    double m_m11;
    double m_m12;
    double m_m21;
    double m_m22;
    double m_dx;
    double m_dy;
};

} // namespace WebCore

#endif // AffineTransform_h
