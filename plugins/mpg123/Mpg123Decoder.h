#ifndef MPG123DECODER_H
#define MPG123DECODER_H

#include <plugin/IDecoder.h>
#include <mpg123.h>
using namespace std;
using namespace mous;

class Mpg123Decoder: public IDecoder
{
public:
    Mpg123Decoder();
    virtual ~Mpg123Decoder();

    virtual std::vector<std::string> FileSuffix() const;

    virtual ErrorCode Open(const std::string& url);
    virtual void Close();

    virtual bool IsFormatVaild() const;

    virtual ErrorCode DecodeUnit(char* data, uint32_t& used, uint32_t& unitCount);
    virtual ErrorCode SetUnitIndex(uint64_t index);
    virtual uint32_t MaxBytesPerUnit() const;
    virtual uint64_t UnitIndex() const;
    virtual uint64_t UnitCount() const;

    virtual enum AudioMode AudioMode() const;
    virtual int32_t Channels() const;
    virtual int32_t BitsPerSample() const;
    virtual int32_t SampleRate() const;
    virtual int32_t BitRate() const;
    virtual uint64_t Duration() const;

private:
    mpg123_handle* m_pHandle;

    uint32_t m_MaxBytesPerUnit;
    uint64_t m_UnitIndex;
    uint64_t m_UnitCount;

    int32_t m_Channels;
    int32_t m_BitsPerSample;
    int32_t m_SampleRate;
    int32_t m_BitRate;
    uint64_t m_Duration;
};

#endif
