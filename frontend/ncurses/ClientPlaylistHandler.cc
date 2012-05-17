#include "ClientPlaylistHandler.h"

#include <iostream>

#include "Protocol.h"
using namespace Protocol;

#define SEND_PACKET(stream)  \
{\
    int payloadSize = (BufObj(NULL) stream).Offset();   \
    char* buf = fnGetPayloadBuffer(                     \
            Group::Playlist, payloadSize);              \
    BufObj(buf) stream;                                 \
}\
    fnSendOut()

ClientPlaylistHandler::ClientPlaylistHandler()
{
}

ClientPlaylistHandler::~ClientPlaylistHandler()
{
}

void ClientPlaylistHandler::Handle(char* buf, int len)
{
    if (len < (int)sizeof(char))
        return;

    char op = Op::Playlist::None;

    BufObj bufObj(buf);
    bufObj >> op;
    switch (op) {
        case Op::Playlist::Append:
        {
            char playlist;
            int32_t count;
            bufObj >> playlist >> count;

            deque<MediaItem*> list;
            for (int i = 0; i < count; ++i) {
                MediaItem* item = new MediaItem;
                *item << bufObj;
                list.push_back(item);
            }

            m_SigAppend(playlist, list);
        }
            break;

        case Op::Playlist::Remove:
        {
            char playlist;
            int32_t pos;
            bufObj >> playlist >> pos;
            m_SigRemove(playlist, pos);
        }
            break;

        case Op::Playlist::Clear:
        {
            m_SigClear(bufObj.Fetch<char>());
        }
            break;

        default:
            break;
    }
}

void ClientPlaylistHandler::Append(int playlist, const string& path)
{
    SEND_PACKET(<< (char)Op::Playlist::Append << (char)playlist << path);
}

void ClientPlaylistHandler::Remove(int playlist, int pos)
{
    SEND_PACKET(<< (char)Op::Playlist::Remove << (char)playlist << (int32_t)pos);
}

void ClientPlaylistHandler::Clear(int playlist)
{
    SEND_PACKET(<< (char)Op::Playlist::Clear << (char)playlist);
}

void ClientPlaylistHandler::Sync(int playlist)
{
    SEND_PACKET(<< (char)Op::Playlist::Sync << (char)playlist);
}

void ClientPlaylistHandler::MoveItem(int playlist, int oldPos, int newPos)
{
}

#undef SEND_PACKET