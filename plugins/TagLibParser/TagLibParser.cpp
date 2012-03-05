#include "TagLibParser.h"

TagLibParser::TagLibParser():
    m_pFileRef(NULL),
    m_pTag(NULL),
    m_pProp(NULL)
{

}

TagLibParser::~TagLibParser()
{
    Close();
}

void TagLibParser::GetFileSuffix(vector<string>& list) const
{
    list.clear();
    list.push_back("*");
}

EmErrorCode TagLibParser::Open(const string& path)
{
    m_pFileRef = new FileRef(path.c_str(), true, AudioProperties::Accurate);
    if (!m_pFileRef->isNull() && m_pFileRef->tag() != NULL) {
        m_pTag = m_pFileRef->tag();
        m_pProp = m_pFileRef->audioProperties();
    }
    return ErrorCode::Ok;
}

void TagLibParser::Close()
{
    if (m_pFileRef != NULL) {
        delete m_pFileRef;
        m_pFileRef = NULL;
        m_pTag = NULL;
        m_pProp = NULL;
    }
}

string TagLibParser::GetTitle()
{
    if (m_pTag != NULL) {
        return m_pTag->title().toCString();
    } else {
        return "";
    }
}

string TagLibParser::GetArtist()
{
    if (m_pTag != NULL) {
        return m_pTag->artist().toCString();
    } else {
        return "";
    }
}

string TagLibParser::GetAlbum()
{
    if (m_pTag != NULL) {
        return m_pTag->album().toCString();
    } else {
        return "";
    }
}

string TagLibParser::GetComment()
{
    if (m_pTag != NULL) {
        return m_pTag->comment().toCString();
    } else {
        return "";
    }
}

string TagLibParser::GetGenre()
{
    if (m_pTag != NULL) {
        return m_pTag->genre().toCString();
    } else {
        return "";
    }
}

int32_t TagLibParser::GetYear()
{
    if (m_pTag != NULL) {
        return m_pTag->year();
    } else {
        return -1;
    }
}

int32_t TagLibParser::GetTrack()
{
    if (m_pTag != NULL) {
        return m_pTag->track();
    } else {
        return -1;
    }
}

int32_t TagLibParser::GetDuration()
{
    if (m_pProp != NULL) {
        return m_pProp->length();
    } else {
        return 0;
    }
}

int32_t TagLibParser::GetBitRate()
{
    if (m_pProp != NULL) {
        return m_pProp->bitrate();
    } else {
        return 0;
    }
}

void TagLibParser::SetTitle(const string& title)
{

}

void TagLibParser::SetArtist(const string& artist)
{

}

void TagLibParser::SetAlbum(const string& album)
{

}

void TagLibParser::SetComment(const string& comment)
{

}

void TagLibParser::SetGenre(const string& genre)
{

}

void TagLibParser::SetYear(int32_t year)
{

}

void TagLibParser::SetTrack(int32_t track)
{

}

bool TagLibParser::HasTag()
{
    return (m_pTag != NULL) ? !m_pTag->isEmpty() : false;
}

bool TagLibParser::HasProperties()
{
    return (m_pProp != NULL) ? true : false;
}
