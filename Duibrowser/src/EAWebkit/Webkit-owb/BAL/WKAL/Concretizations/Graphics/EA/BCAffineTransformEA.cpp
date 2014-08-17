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
#include "BALBase.h"

#include "AffineTransform.h"
#include "IntRect.h"
#include "FloatRect.h"
#include "MathExtras.h"


namespace WKAL {

AffineTransform::AffineTransform()
    : m_m11(1)
    , m_m12(0)
    , m_m21(0)
    , m_m22(1)
    , m_dx(0)
    , m_dy(0)
{
}

AffineTransform::AffineTransform(double a, double b, double c, double d, double tx, double ty)
    : m_m11(a)
    , m_m12(b)
    , m_m21(c)
    , m_m22(d)
    , m_dx(tx)
    , m_dy(ty)
{
}


void AffineTransform::setMatrix(double a, double b, double c, double d, double tx, double ty)
{
    m_m11 = a;
    m_m12 = b;
    m_m21 = c;
    m_m22 = d;
    m_dx = tx;
    m_dy = ty;
}

void AffineTransform::map(double x, double y, double *x2, double *y2) const
{
    *x2 = m_m11 * x + m_m21 * y + m_dx;
    *y2 = m_m22 * y + m_m12 * x + m_dy;

}

IntRect AffineTransform::mapRect(const IntRect& rect) const
{
    IntRect result;
    if (m_m12 == 0.0F && m_m21 == 0.0F) {
        int x = lround(m_m11 * rect.x() + m_dx);
        int y = lround(m_m22 * rect.y() + m_dy);
        int w = lround(m_m11 * rect.width());
        int h = lround(m_m22 * rect.height());
        if (w < 0) {
            w = -w;
            x -= w;
        }
        if (h < 0) {
            h = -h;
            y -= h;
        }
        result = IntRect(x, y, w, h);
    } else {
        // see mapToPolygon for explanations of the algorithm.
        double x0, y0;
        double x, y;
        map(rect.location().x(), rect.location().y(), &x0, &y0);
        double xmin = x0;
        double ymin = y0;
        double xmax = x0;
        double ymax = y0;
        map(rect.right() + 1, rect.location().y(), &x, &y);
	if (x < xmin)
	    xmin = x;
        if (y < ymin)
	    ymin = y;
        if (x > xmax)
	    xmax = x;
	if (y > ymax)
	    ymax = y;
        map(rect.right() + 1, rect.bottom() + 1, &x, &y);
        if (x < xmin)
	    xmin = x;
        if (y < ymin)
	    ymin = y;
        if (x > xmax)
	    xmax = x;
	if (y > ymax)
	    ymax = y;
        map(rect.location().x(), rect.bottom() + 1, &x, &y);
        if (x < xmin)
	    xmin = x;
        if (y < ymin)
	    ymin = y;
        if (x > xmax)
	    xmax = x;
	if (y > ymax)
	    ymax = y;
        double w = xmax - xmin;
        double h = ymax - ymin;
        xmin -= (xmin - x0) / w;
    	ymin -= (ymin - y0) / h;
        xmax -= (xmax - x0) / w;
        ymax -= (ymax - y0) / h;
        result = IntRect(lround(xmin), lround(ymin), lround(xmax)-lround(xmin)+1, lround(ymax)-lround(ymin)+1);
    }

    return result;

    /*IntRect r ( (int)( m_m11*rect.x() + m_m21*rect.y() + m_dx ), (int)(m_m22*rect.y() + m_m12*rect.x() + m_dy), rect.width(), rect.height() );
    return r;*/
}

FloatRect AffineTransform::mapRect(const FloatRect& rect) const
{
    FloatRect result;
    if (m_m12 == 0.0F && m_m21 == 0.0F) {
        double x = m_m11 * rect.x() + m_dx;
        double y = m_m22 * rect.y() + m_dy;
        double w = m_m11 * rect.width();
        double h = m_m22 * rect.height();
        if (w < 0) {
            w = -w;
            x -= w;
        }
        if (h < 0) {
            h = -h;
            y -= h;
        }
        result = FloatRect(x, y, w, h);
    } else {
        double x0, y0;
        double x, y;
        map(rect.x(), rect.y(), &x0, &y0);
        double xmin = x0;
        double ymin = y0;
        double xmax = x0;
        double ymax = y0;
        map(rect.x() + rect.width(), rect.y(), &x, &y);
        if (x < xmin)
    	    xmin = x;
        if (y < ymin)
    	    ymin = y;
        if (x > xmax)
    	    xmax = x;
	if (y > ymax)
	    ymax = y;
        map(rect.x() + rect.width(), rect.y() + rect.height(), &x, &y);
        if (x < xmin)
    	    xmin = x;
        if (y < ymin)
    	    ymin = y;
        if (x > xmax)
    	    xmax = x;
	if (y > ymax)
	    ymax = y;
        map(rect.x(), rect.y() + rect.height(), &x, &y);
        if (x < xmin)
    	    xmin = x;
        if (y < ymin)
    	    ymin = y;
        if (x > xmax)
    	    xmax = x;
	if (y > ymax)
	    ymax = y;
        result = FloatRect(xmin, ymin, xmax-xmin, ymax - ymin);
    }
    return result;

    /*FloatRect r ( m_m11*rect.x() + m_m21*rect.y() + m_dx, m_m22*rect.y() + m_m12*rect.x() + m_dy, rect.width(), rect.height() );
    return r;*/
}

bool AffineTransform::isIdentity() const
{
    if ((m_m11 == 1)
    &&  (m_m12 == 0)
    &&  (m_m21 == 0)
    &&  (m_m22 == 1)
    &&  (m_dx == 0)
    &&  (m_dy == 0))
        return true;

    return false;
}
double AffineTransform::a() const
{
    return m_m11;
}

void AffineTransform::setA(double a)
{
    setMatrix(a, b(), c(), d(), e(), f());
}

double AffineTransform::b() const
{
    return m_m12;
}

void AffineTransform::setB(double b)
{
    setMatrix(a(), b, c(), d(), e(), f());
}

double AffineTransform::c() const
{
    return m_m21;
}

void AffineTransform::setC(double c)
{
    setMatrix(a(), b(), c, d(), e(), f());
}

double AffineTransform::d() const
{
    return m_m22;
}

void AffineTransform::setD(double d)
{
    setMatrix(a(), b(), c(), d, e(), f());
}

double AffineTransform::e() const
{
    return m_dx;
}

void AffineTransform::setE(double e)
{
    setMatrix(a(), b(), c(), d(), e, f());
}

double AffineTransform::f() const
{
    return m_dy;
}

void AffineTransform::setF(double f)
{
    setMatrix(a(), b(), c(), d(), e(), f);
}

void AffineTransform::reset()
{
    m_m11 = 1;
    m_m12 = 0;
    m_m21 = 0;
    m_m22 = 1;
    m_dx = 0;
    m_dy = 0;
}

AffineTransform &AffineTransform::scale(double sx, double sy)
{
    m_m11 *= sx;
    m_m12 *= sx;
    m_m21 *= sy;
    m_m22 *= sy;

    return *this;
}

//const double deg2rad = 0.017453292519943295769;        // pi/180

AffineTransform &AffineTransform::rotate(double d)
{
    double sind = 0;
    double cosd = 0;
    if (d == 90. || d == -270.)
        sind = 1.;
    else if (d == 270. || d == -90.)
        sind = -1.;
    else if (d == 180.)
        cosd = -1.;
    else{
        double b = deg2rad(d);                        // convert to radians
        sind = sin(b);                // fast and convenient
        cosd = cos(b);
    }
    double tm11 = cosd * m_m11 + sind * m_m21;
    double tm12 = cosd * m_m12 + sind * m_m22;
    double tm21 = -sind * m_m11 + cosd * m_m21;
    double tm22 = -sind * m_m12 + cosd * m_m22;
    m_m11 = tm11;
    m_m12 = tm12;
    m_m21 = tm21;
    m_m22 = tm22;

    return *this;
}

AffineTransform &AffineTransform::translate(double tx, double ty)
{
    m_dx += tx*m_m11 + ty*m_m21;
    m_dy += ty*m_m22 + tx*m_m12;

    return *this;
}

AffineTransform &AffineTransform::shear(double sx, double sy)
{
    double tm11 = sy * m_m21;
    double tm12 = sy * m_m22;
    double tm21 = sx * m_m11;
    double tm22 = sx * m_m12;
    m_m11 += tm11;
    m_m12 += tm12;
    m_m21 += tm21;
    m_m22 += tm22;

    return *this;
}

double AffineTransform::det() const
{
    return m_m11 * m_m22 - m_m12 * m_m21;
}

AffineTransform AffineTransform::inverse() const
{
    double determinant = det();
    if (determinant == 0.0)
        return AffineTransform();
    else {
        double dinv = 1.0 / determinant;
        AffineTransform imatrix((m_m22 * dinv), (-m_m12 * dinv),
                          (-m_m21 * dinv), (m_m11 * dinv),
                          ((m_m21 * m_dy - m_m22 * m_dx) * dinv),
                          ((m_m12 * m_dx - m_m11 * m_dy) * dinv));
        return imatrix;
    }
}

bool AffineTransform::operator==(const AffineTransform& at) const
{
    if ((m_m11 == at.m_m11)
    &&  (m_m12 == at.m_m12)
    &&  (m_m21 == at.m_m21)
    &&  (m_m22 == at.m_m22)
    &&  (m_dx == at.m_dx)
    &&  (m_dy == at.m_dy))
        return true;

    return false;
}

AffineTransform& AffineTransform::operator*=(const AffineTransform& b)
{
    double tm11 = m_m11 * b.m_m11 + m_m12 * b.m_m21;
    double tm12 = m_m11 * b.m_m12 + m_m12 * b.m_m22;
    double tm21 = m_m21 * b.m_m11 + m_m22 * b.m_m21;
    double tm22 = m_m21 * b.m_m12 + m_m22 * b.m_m22;
    double tdx  = m_dx * b.m_m11  + m_dy * b.m_m21 + b.m_dx;
    double tdy =  m_dx * b.m_m12  + m_dy * b.m_m22 + b.m_dy;

    m_m11 = tm11; m_m12 = tm12;
    m_m21 = tm21; m_m22 = tm22;
    m_dx = tdx; m_dy = tdy;

    return *this;
}

AffineTransform AffineTransform::operator*(const AffineTransform& b)
{
    AffineTransform result = *this;
    result *= b;

    return result;
}

AffineTransform::operator BalMatrix() const
{
    return m_transform;
}

}
