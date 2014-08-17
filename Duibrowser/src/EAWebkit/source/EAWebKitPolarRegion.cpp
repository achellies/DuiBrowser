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
// EAWebKitPolarRegion.cpp
// By Chris Stott
///////////////////////////////////////////////////////////////////////////////

#include <EAWebKit/internal/InputBinding/EAWebKitPolarRegion.h>
#include <EAWebKit/internal/InputBinding/EAWebKitUtils.h>
#include <EAWebKit/EAWebkitAllocator.h>
#include <eastl/fixed_vector.h>
#include <platform/graphics/IntPoint.h>
#include <platform/graphics/FloatPoint.h>
#include <platform/graphics/IntRect.h>

namespace EA
{
	namespace WebKit
	{
		PolarRegion::PolarRegion(WebCore::IntRect rect, WebCore::IntPoint centre)
		{
			rect.move(-centre.x(),-centre.y());

			typedef eastl::fixed_vector<WebCore::IntPoint, 10, false, EASTLAllocator> IntPointCollection;

			IntPointCollection intPoints;
			intPoints.push_back(rect.bottomLeft());
			intPoints.push_back(rect.topLeft());
			intPoints.push_back(rect.bottomRight());
			intPoints.push_back(rect.topRight());

			intPoints.push_back(Average(rect.topLeft(), rect.topRight()));
			intPoints.push_back(Average(rect.bottomLeft(), rect.bottomRight()));
			intPoints.push_back(Average(rect.topRight(), rect.bottomRight()));
			intPoints.push_back(Average(rect.topLeft(), rect.bottomLeft()));

			bool initial = true;
			for ( IntPointCollection::iterator i = intPoints.begin(); i != intPoints.end(); ++i )
			{
				WebCore::FloatPoint point = CartesianToPolar(*i);

				if ( initial )
				{
					// seed with first values
					maxR = point.x();
					minR = point.x();

					maxTheta = point.y();
					minTheta = point.y();

					initial = false;
				}
				else
				{
					maxR = Max(maxR, point.x());
					minR = Min(minR, point.x());

					maxTheta = Max(maxTheta, point.y());
					minTheta = Min(minTheta, point.y());
				}
			}

			if (maxTheta-minTheta > 3.14159f)
			{
				Swap(minTheta,maxTheta);
			} 

		}

		WebCore::FloatPoint PolarRegion::CartesianToPolar(WebCore::IntPoint point)
		{
			WebCore::FloatPoint p(point);
			WebCore::FloatPoint f;

			f.setX(sqrt(p.x()*p.x() + p.y()*p.y()));
			f.setY(CalcAngle(p.x(),p.y()));

			if (f.y() < 0.0f)
			{
				f.setY(f.y() + 2*3.14159f);
			}

			return f;
		}

	} // namespace WebKit
} // namespace EA