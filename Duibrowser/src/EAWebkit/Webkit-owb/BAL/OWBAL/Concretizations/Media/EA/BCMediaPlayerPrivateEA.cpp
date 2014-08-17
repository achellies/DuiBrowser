///////////////////////////////////////////////////////////////////////////////
// BCMediaPlayerPrivateEA.cpp
//
// Copyright (c) 2008 Electronic Arts. All Rights Reserved.
// By Paul Pedriana
///////////////////////////////////////////////////////////////////////////////


#include "config.h"

#if ENABLE(VIDEO)

#include "MediaPlayerPrivateEA.h"

#include "CString.h"
#include "GraphicsContext.h"
#include "IntRect.h"
#include "KURL.h"
#include "MIMETypeRegistry.h"
#include "MediaPlayer.h"
#include "NotImplemented.h"
#include "ScrollView.h"
#include "Widget.h"

#include <cstdio>
#include <limits>
#include <math.h>

using namespace std;

namespace OWBAL {

MediaPlayerPrivate::MediaPlayerPrivate(MediaPlayer* player)
    : m_player(player)
    , m_rate(1.0f)
    , m_endTime(numeric_limits<float>::infinity())
    , m_isEndReached(false)
    , m_volume(0.5f)
    , m_networkState(MediaPlayer::Empty)
    , m_readyState(MediaPlayer::DataUnavailable)
    , m_startedPlaying(false)
    , m_isStreaming(false)
    , m_rect(IntRect())
    , m_visible(true)
{
    // OWB_PRINTF("MediaPlayerPrivate::MediaPlayerPrivate\n");
}

MediaPlayerPrivate::~MediaPlayerPrivate()
{
    // OWB_PRINTF("MediaPlayerPrivate::~MediaPlayerPrivate\n");
}

void MediaPlayerPrivate::load(String url)
{
    // OWB_PRINTF("MediaPlayerPrivate::load\n");
}

void MediaPlayerPrivate::play()
{
    // OWB_PRINTF("MediaPlayerPrivate::play\n");
}

void MediaPlayerPrivate::pause()
{
    // OWB_PRINTF("MediaPlayerPrivate::pause\n");
}

float MediaPlayerPrivate::duration()
{
    // OWB_PRINTF("MediaPlayerPrivate::duration\n");
    return 0.0;
}

float MediaPlayerPrivate::currentTime() const
{
    // OWB_PRINTF("MediaPlayerPrivate::currentTime\n");
    return 0.0;
}

void MediaPlayerPrivate::seek(float time)
{
    // OWB_PRINTF("MediaPlayerPrivate::seek\n");
}

void MediaPlayerPrivate::setEndTime(float time)
{
    // OWB_PRINTF("MediaPlayerPrivate::setEndTime\n");
}

void MediaPlayerPrivate::startEndPointTimerIfNeeded()
{
    // OWB_PRINTF("MediaPlayerPrivate::startEndPointTimerIfNeeded\n");
    notImplemented();
}

void MediaPlayerPrivate::cancelSeek()
{
    // OWB_PRINTF("MediaPlayerPrivate::cancelSeek\n");
    notImplemented();
}

void MediaPlayerPrivate::endPointTimerFired(Timer<MediaPlayerPrivate>*)
{
    // OWB_PRINTF("MediaPlayerPrivate::endPointTimerFired\n");
    notImplemented();
}

bool MediaPlayerPrivate::paused() const
{
    // OWB_PRINTF("MediaPlayerPrivate::paused\n");
    return !m_startedPlaying;
}

bool MediaPlayerPrivate::seeking() const
{
    // OWB_PRINTF("MediaPlayerPrivate::seeking\n");
    return false;
}

// Returns the size of the video
IntSize MediaPlayerPrivate::naturalSize()
{
    // OWB_PRINTF("MediaPlayerPrivate::naturalSize\n");
    return IntSize();
}

bool MediaPlayerPrivate::hasVideo()
{
    // OWB_PRINTF("MediaPlayerPrivate::hasVideo\n");
    return false;
}

void MediaPlayerPrivate::setVolume(float volume)
{
    // OWB_PRINTF("MediaPlayerPrivate::setVolume\n");
}

void MediaPlayerPrivate::setMuted(bool b)
{
    // OWB_PRINTF("MediaPlayerPrivate::setMuted\n");

}

void MediaPlayerPrivate::setRate(float rate)
{
    // OWB_PRINTF("MediaPlayerPrivate::setRate\n");

}

int MediaPlayerPrivate::dataRate() const
{
    // OWB_PRINTF("MediaPlayerPrivate::dataRate\n");
    notImplemented();
    return 1;
}

MediaPlayer::NetworkState MediaPlayerPrivate::networkState()
{
    // OWB_PRINTF("MediaPlayerPrivate::networkState\n");
    return m_networkState;
}

MediaPlayer::ReadyState MediaPlayerPrivate::readyState()
{
    // OWB_PRINTF("MediaPlayerPrivate::readyState\n");
    return m_readyState;
}

float MediaPlayerPrivate::maxTimeBuffered()
{
    // OWB_PRINTF("MediaPlayerPrivate::maxTimeBuffered\n");
    notImplemented();
    // rtsp streams are not buffered
    return m_isStreaming ? 0 : maxTimeLoaded();
}

float MediaPlayerPrivate::maxTimeSeekable()
{
    // OWB_PRINTF("MediaPlayerPrivate::maxTimeSeekable\n");
    // TODO
    if (m_isStreaming)
        return numeric_limits<float>::infinity();
    // infinite duration means live stream
    return maxTimeLoaded();
}

float MediaPlayerPrivate::maxTimeLoaded()
{
    // OWB_PRINTF("MediaPlayerPrivate::maxTimeLoaded\n");
    // TODO
    notImplemented();
    return duration();
}

unsigned MediaPlayerPrivate::bytesLoaded()
{
    // OWB_PRINTF("MediaPlayerPrivate::bytesLoaded\n");
    notImplemented();

    /*if (!m_playBin)
        return 0;
    float dur = duration();
    float maxTime = maxTimeLoaded();
    if (!dur)
        return 0;*/

    return 1; // totalBytes() * maxTime / dur;
}

bool MediaPlayerPrivate::totalBytesKnown()
{
    // OWB_PRINTF("MediaPlayerPrivate::totalBytesKnown\n");
    notImplemented();
    return totalBytes() > 0;
}

unsigned MediaPlayerPrivate::totalBytes()
{
    // OWB_PRINTF("MediaPlayerPrivate::totalBytes\n");
    notImplemented();

    // Do something with m_source to get the total bytes of the media

    return 100;
}

void MediaPlayerPrivate::cancelLoad()
{
    // OWB_PRINTF("MediaPlayerPrivate::cancelLoad\n");
    notImplemented();
}

void MediaPlayerPrivate::updateStates()
{
    // OWB_PRINTF("MediaPlayerPrivate::updateStates\n");
}

void MediaPlayerPrivate::loadStateChanged()
{
    // OWB_PRINTF("MediaPlayerPrivate::loadStateChanged\n");
    updateStates();
}

void MediaPlayerPrivate::rateChanged()
{
    // OWB_PRINTF("MediaPlayerPrivate::rateChanged\n");
    updateStates();
}

void MediaPlayerPrivate::sizeChanged()
{
    // OWB_PRINTF("MediaPlayerPrivate::sizeChanged\n");
    notImplemented();
}

void MediaPlayerPrivate::timeChanged()
{
    // OWB_PRINTF("MediaPlayerPrivate::timeChanged\n");
    updateStates();
    m_player->timeChanged();
}

void MediaPlayerPrivate::volumeChanged()
{
    // OWB_PRINTF("MediaPlayerPrivate::volumeChanged\n");
    m_player->volumeChanged();
}

void MediaPlayerPrivate::didEnd()
{
    // OWB_PRINTF("MediaPlayerPrivate::didEnd\n");
    m_isEndReached = true;
    pause();
    timeChanged();
}

void MediaPlayerPrivate::loadingFailed()
{
    // OWB_PRINTF("MediaPlayerPrivate::loadingFailed\n");
    if (m_networkState != MediaPlayer::LoadFailed) {
        m_networkState = MediaPlayer::LoadFailed;
        m_player->networkStateChanged();
    }
    if (m_readyState != MediaPlayer::DataUnavailable) {
        m_readyState = MediaPlayer::DataUnavailable;
        m_player->readyStateChanged();
    }
}

void MediaPlayerPrivate::setRect(const IntRect& rect)
{
    // OWB_PRINTF("MediaPlayerPrivate::setRect\n");
    m_rect = rect;
}

void MediaPlayerPrivate::setVisible(bool visible)
{
    // OWB_PRINTF("MediaPlayerPrivate::setVisible\n");
    m_visible = visible;
}

void MediaPlayerPrivate::repaint()
{
    // OWB_PRINTF("MediaPlayerPrivate::repaint\n");
    m_player->repaint();
}

void MediaPlayerPrivate::paint(GraphicsContext* context, const IntRect& rect)
{
        // OWB_PRINTF("MediaPlayerPrivate::paint\n");
}

void MediaPlayerPrivate::getSupportedTypes(HashSet<String>& types)
{
    // OWB_PRINTF("MediaPlayerPrivate::getSupportedTypes\n");
    // FIXME: do the real thing
    notImplemented();
    types.add(String("video/x-theora+ogg"));
}

void MediaPlayerPrivate::createGSTPlayBin(String url)
{
    // OWB_PRINTF("MediaPlayerPrivate::createGSTPlayBin\n");
}

}

#endif
