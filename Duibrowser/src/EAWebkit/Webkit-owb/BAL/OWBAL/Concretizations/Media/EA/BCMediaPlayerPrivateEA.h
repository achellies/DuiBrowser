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
// BCMediaPlayerPrivateEA.h
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////

/*
 * Copyright (C) 2007 Apple Inc.  All rights reserved.
 * Copyright (C) 2007 Collabora Ltd. All rights reserved.
 * Copyright (C) 2007 Alp Toker <alp@atoker.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * aint with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef MediaPlayerPrivateBal_h
#define MediaPlayerPrivateBal_h

#include <wtf/FastAllocBase.h>
#if ENABLE(VIDEO)

#include "MediaPlayer.h"
#include "Timer.h"

namespace OWBAL {

    class GraphicsContext;
    class IntSize;
    class IntRect;
    class String;

    class MediaPlayerPrivate : Noncopyable {
public:
#if NO_MACRO_NEW
	// Placement operator new.
        void* operator new(size_t, void* p) { return p; }
        void* operator new[](size_t, void* p) { return p; }

        void* operator new(size_t size)
        {
            void* p = fastMalloc(size);
            fastMallocMatchValidateMalloc(p, WTF::Internal::AllocTypeClassNew);
            return p;
        }

        void operator delete(void* p)
        {
            fastMallocMatchValidateFree(p, WTF::Internal::AllocTypeClassNew);
            fastFree(p);  // We don't need to check for a null pointer; the compiler does this.
        }

        void* operator new[](size_t size)
        {
            void* p = fastMalloc(size);
            fastMallocMatchValidateMalloc(p, WTF::Internal::AllocTypeClassNewArray);
            return p;
        }

        void operator delete[](void* p)
        {
            fastMallocMatchValidateFree(p, WTF::Internal::AllocTypeClassNewArray);
            fastFree(p);  // We don't need to check for a null pointer; the compiler does this.
        }
#endif //NO_MACRO_NEW

    public:
        MediaPlayerPrivate(MediaPlayer*);
        ~MediaPlayerPrivate();

        IntSize naturalSize();
        bool hasVideo();

        void load(String url);
        void cancelLoad();

        void play();
        void pause();

        bool paused() const;
        bool seeking() const;

        float duration();
        float currentTime() const;
        void seek(float);
        void setEndTime(float);

        void setRate(float);
        void setVolume(float);
        void setMuted(bool);

        int dataRate() const;

        MediaPlayer::NetworkState networkState();
        MediaPlayer::ReadyState readyState();

        float maxTimeBuffered();
        float maxTimeSeekable();
        unsigned bytesLoaded();
        bool totalBytesKnown();
        unsigned totalBytes();

        void setVisible(bool);
        void setRect(const IntRect&);

        void loadStateChanged();
        void rateChanged();
        void sizeChanged();
        void timeChanged();
        void volumeChanged();
        void didEnd();
        void loadingFailed();

        void repaint();
        void paint(GraphicsContext*, const IntRect&);
        static void getSupportedTypes(HashSet<String>&);
        static bool isAvailable() { return true; }

    private:

        void updateStates();
        void cancelSeek();
        void endPointTimerFired(Timer<MediaPlayerPrivate>*);
        float maxTimeLoaded();
        void startEndPointTimerIfNeeded();

        void createGSTPlayBin(String url);

    private:
        MediaPlayer* m_player;
        float m_rate;
        float m_endTime;
        bool m_isEndReached;
        double m_volume;
        MediaPlayer::NetworkState m_networkState;
        MediaPlayer::ReadyState m_readyState;
        bool m_startedPlaying;
        bool m_isStreaming;
        IntRect m_rect;
        bool m_visible;
    };
}

#endif
#endif
