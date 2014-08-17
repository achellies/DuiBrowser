/*
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"
#include "ClipboardISee.h"

#include "NotImplemented.h"
#include "StringHash.h"

#include "Editor.h"

namespace WebCore {

PassRefPtr<Clipboard> Editor::newGeneralClipboard(ClipboardAccessPolicy policy)
{
	return ClipboardISee::create(policy, false);
}

ClipboardISee::ClipboardISee(ClipboardAccessPolicy policy, bool forDragging)
    : Clipboard(policy, forDragging)
{
    notImplemented();
}

ClipboardISee::~ClipboardISee()
{
    notImplemented();
}

void ClipboardISee::clearData(const String&)
{
    notImplemented();
}

void ClipboardISee::clearAllData()
{
    notImplemented();
}

String ClipboardISee::getData(const String&, bool &success) const
{
    notImplemented();
    success = false;
    return String();
}

bool ClipboardISee::setData(const String&, const String&)
{
    notImplemented();
    return false;
}

HashSet<String> ClipboardISee::types() const
{
    notImplemented();
    return HashSet<String>();
}

IntPoint ClipboardISee::dragLocation() const
{
    notImplemented();
    return IntPoint(0, 0);
}

CachedImage* ClipboardISee::dragImage() const
{
    notImplemented();
    return 0;
}

void ClipboardISee::setDragImage(CachedImage*, const IntPoint&)
{
    notImplemented();
}

Node* ClipboardISee::dragImageElement()
{
    notImplemented();
    return 0;
}

void ClipboardISee::setDragImageElement(Node*, const IntPoint&)
{
    notImplemented();
}

DragImageRef ClipboardISee::createDragImage(IntPoint&) const
{
    notImplemented();
    return 0;
}

void ClipboardISee::declareAndWriteDragImage(Element*, const KURL&, const String&, Frame*)
{
    notImplemented();
}

void ClipboardISee::writeURL(const KURL&, const String&, Frame*)
{
    notImplemented();
}

void ClipboardISee::writeRange(Range*, Frame*)
{
    notImplemented();
}

bool ClipboardISee::hasData()
{
    notImplemented();
    return false;
}

}
