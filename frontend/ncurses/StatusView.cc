#include "StatusView.h"

#include <stdio.h>

#include <string>
using namespace std;

#include <scx/CharsetHelper.hpp>
#include <scx/Conv.hpp>
#include <scx/FileInfo.hpp>

inline static string FormatTime(int ms)
{
    if (ms <= 0)
        return "00:00";

    char buf[6];
    int sec = ms / 1000;
    snprintf(buf, sizeof(buf), "%.2d:%.2d", (int)(sec/60), (int)(sec%60));
    return string(buf, 5);
}

StatusView::StatusView():
    m_NeedRefresh(0),
    m_PlayerHandler(NULL)
{
}

StatusView::~StatusView()
{
}

void StatusView::Refresh()
{
    using namespace ncurses;
    using namespace CharsetHelper;

    d.Clear();

    const int w = d.w - 3;
    //const int h = d.h - 2;
    const int x = 2, y = 1;

    int xoff = x, yoff = y;

    const int wVolLabel = 2*2;
    const int wVolSlider = 20;
    const int wCurrentItem = w - (wVolLabel + wVolSlider + 1) - 1;

    MutexLocker playerStatuslocker(&m_PlayerStatusMutex);
    const MediaItem& item = m_PlayerStatus.item;
    const MediaTag& tag = m_PlayerStatus.item.tag;

    // { title - artist (file)~~~~~~-|=======|+}
    string currentItem = tag.title + " - " + tag.artist + " (" + FileInfo(item.url).BaseName() + ")";
    currentItem = MBStrWidth(currentItem) <= wCurrentItem ?
        currentItem : MBWidthStr(currentItem, wCurrentItem-3) + "...";
    d.AttrSet(Attr::Bold);
    d.ColorOn(Color::Green, Color::Black);
    d.Print(xoff, yoff, currentItem);

    xoff = x + w - (wVolSlider + wVolLabel);
    d.AttrSet(Attr::Bold);
    d.ColorOn(Color::Yellow, Color::Black);
    d.Print(xoff, yoff, "-|");
    xoff += 2;

    d.AttrSet(Attr::Bold);
    d.ColorOn(Color::White, Color::Black);
    d.Print(xoff, yoff, string(wVolSlider/2, '='));
    xoff += wVolSlider;

    d.AttrSet(Attr::Bold);
    d.ColorOn(Color::Yellow, Color::Black);
    d.Print(xoff, yoff,  "|+");
    xoff += 2;

    // { 00:00/00:00 bitRate bps sampleRate Hz }
    xoff = x;
    yoff += 1;
    {
        const int tab = 4;
        string sp = "/";
        string ms = FormatTime(m_PlayerStatus.pos);
        string dur = FormatTime(m_PlayerStatus.duration);
        string bps = NumToStr(m_PlayerStatus.bitRate);
        string strBps = " Kbps";
        string rate = NumToStr(m_PlayerStatus.sampleRate);
        string strRate = " Hz";

        d.AttrSet(Attr::Bold);
        d.ColorOn(Color::Magenta, Color::Black);
        d.Print(xoff, yoff, ms);
        xoff += ms.size();

        d.AttrSet(Attr::Normal);
        d.ColorOn(Color::White, Color::Black);
        d.Print(xoff, yoff, sp);
        xoff += sp.size();

        d.AttrSet(Attr::Bold);
        d.ColorOn(Color::Magenta, Color::Black);
        d.Print(xoff, yoff, dur);
        xoff += dur.size();
        xoff += tab;

        d.AttrSet(Attr::Bold);
        d.ColorOn(Color::Magenta, Color::Black);
        d.Print(xoff, yoff, bps);
        xoff += bps.size();

        d.AttrSet(Attr::Normal);
        d.ColorOn(Color::White, Color::Black);
        d.Print(xoff, yoff, strBps);
        xoff += strBps.size() + tab;

        d.AttrSet(Attr::Bold);
        d.ColorOn(Color::Magenta, Color::Black);
        d.Print(xoff, yoff, rate);
        xoff += rate.size();

        d.AttrSet(Attr::Normal);
        d.ColorOn(Color::White, Color::Black);
        d.Print(xoff, yoff, strRate);
        xoff += strRate.size() + tab;
    }

    // { =====~~~~~~~~ }
    xoff = x;
    yoff += 1;
    if (m_PlayerStatus.duration != 0) {
        int wSlider = (w - 1) * m_PlayerStatus.pos / m_PlayerStatus.duration;
        string slider(wSlider, '-');

        d.AttrSet(Attr::Bold);
        d.ColorOn(Color::Yellow, Color::Black);
        d.Print(xoff, yoff, slider+">");
    }

    d.ResetAttrColor();

    d.Refresh();

    MutexLocker needRefreshLocker(&m_NeedRefreshMutex);
    if (m_NeedRefresh > 0)
        --m_NeedRefresh;
}

bool StatusView::NeedRefresh() const
{
    MutexLocker locker(&m_NeedRefreshMutex);
    return m_NeedRefresh != 0;
}

void StatusView::MoveTo(int x, int y)
{
    d.MoveTo(x, y);
}

void StatusView::Resize(int w, int h)
{
    d.Resize(w, h);
}

bool StatusView::InjectKey(int key)
{
    switch (key) {
        case ' ':
            m_PlayerHandler->Pause();
            break;

        case '>':
            break;

        case '<':
            break;

        case 'N':
            break;

        case 'P':
            break;

        case '+':
            break;

        case '-':
            break;

        default:
            return false;
    }
    return true;
}

void StatusView::Show(bool show)
{
    d.Show(show);
}

bool StatusView::IsShown() const
{
    return d.shown;
}

int StatusView::MinHeight() const
{
    return 3+2;
}

void StatusView::SetPlayerHandler(ClientPlayerHandler* handler)
{
    if (m_PlayerHandler != NULL) {
        m_PlayerHandler->SigStatus().DisconnectReceiver(this);
    }

    if (handler != NULL) {
        handler->SigStatus().Connect(&StatusView::SlotStatus, this);
    }

    m_PlayerHandler = handler;
}

void StatusView::SlotStatus(const ClientPlayerHandler::PlayerStatus& status)
{
    {
        MutexLocker locker(&m_PlayerStatusMutex);
        m_PlayerStatus = status;
    }

    {
        MutexLocker locker(&m_NeedRefreshMutex);
        ++m_NeedRefresh;
    }
}
