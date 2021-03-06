#include "FlacDecoder.h"

FlacDecoder::FlacDecoder()
{
    m_pDecoder = FLAC__stream_decoder_new();
}

FlacDecoder::~FlacDecoder()
{
    Close();
    FLAC__stream_decoder_delete(m_pDecoder);
}

vector<string> FlacDecoder::FileSuffix() const
{
    return { "flac" };
}

ErrorCode FlacDecoder::Open(const string& url)
{
    FLAC__stream_decoder_set_md5_checking(m_pDecoder, false);
    FLAC__stream_decoder_init_file(
            m_pDecoder,
            url.c_str(),
            &WriteCallback,
            nullptr,        // metadata_callback
            &ErrorCallback,
            &m_dctx);

    if (!FLAC__stream_decoder_process_until_end_of_metadata(m_pDecoder))
        return ErrorCode::DecoderFailedToOpen;

    if (!FLAC__stream_decoder_process_single(m_pDecoder))
        return ErrorCode::DecoderFailedToOpen;

    m_Channels = FLAC__stream_decoder_get_channels(m_pDecoder);
    m_SampleRate = FLAC__stream_decoder_get_sample_rate(m_pDecoder);
    m_SamplesPerFrame = FLAC__stream_decoder_get_blocksize(m_pDecoder);
    m_BitsPerSample = FLAC__stream_decoder_get_bits_per_sample(m_pDecoder);

    m_SampleCount = FLAC__stream_decoder_get_total_samples(m_pDecoder);
    m_Duration = m_SampleCount / FLAC__stream_decoder_get_sample_rate(m_pDecoder) * 1000.f;
    m_BitRate = m_SampleRate * 8 * m_Channels / 1000.f;
    m_SampleIndex = 0;

    return ErrorCode::Ok;
}

void FlacDecoder::Close()
{
    if (m_pDecoder != nullptr)
        FLAC__stream_decoder_finish(m_pDecoder);
}

bool FlacDecoder::IsFormatVaild() const
{
    return true;
}

ErrorCode FlacDecoder::DecodeUnit(char* data, uint32_t& used, uint32_t& unitCount)
{
    m_dctx.buf = data;
    m_dctx.buf_used = 0;
    m_dctx.samples_read = 0;

    if (FLAC__stream_decoder_process_single(m_pDecoder)) {
        used = m_dctx.buf_used;
        unitCount = m_dctx.samples_read;
        m_SampleIndex += m_dctx.samples_read;
    } else {
        used = 0;
        unitCount = 0;
    }
    return ErrorCode::Ok;
}

ErrorCode FlacDecoder::SetUnitIndex(uint64_t index)
{
    m_SampleIndex = index;
    FLAC__stream_decoder_seek_absolute(m_pDecoder, m_SampleIndex);
    return ErrorCode::Ok;
}

uint32_t FlacDecoder::MaxBytesPerUnit() const
{
    return FLAC__MAX_BLOCK_SIZE * m_Channels * sizeof(uint32_t);
}

uint64_t FlacDecoder::UnitIndex() const
{
    return m_SampleIndex;
}

uint64_t FlacDecoder::UnitCount() const
{
    return m_SampleCount;
}

AudioMode FlacDecoder::AudioMode() const
{
    return AudioMode::Stereo;
}

int32_t FlacDecoder::Channels() const
{
    return m_Channels;
}

int32_t FlacDecoder::BitsPerSample() const
{
    return m_BitsPerSample;
}

int32_t FlacDecoder::SampleRate() const
{
    return m_SampleRate;
}

int32_t FlacDecoder::BitRate() const
{
    return m_BitRate;
}

uint64_t FlacDecoder::Duration() const
{
    return m_Duration;
}

FLAC__StreamDecoderWriteStatus FlacDecoder::WriteCallback(
        const FLAC__StreamDecoder *decoder, 
        const FLAC__Frame *frame, 
        const FLAC__int32 * const buffer[], 
        void *client_data)
{
    auto pctx = static_cast<decoder_context_t*>(client_data);

    const size_t samples = frame->header.blocksize;
    const size_t channels = frame->header.channels;

    if (pctx->buf != nullptr) {
        for (size_t i = 0, sample = 0; sample < samples; ++sample) {
            for (size_t channel = 0; channel < channels; ++channel, i+=2) {
                pctx->buf[i+1] = (buffer[channel][sample] >> 8);
                pctx->buf[i] = buffer[channel][sample];
            }
        }
        pctx->buf_used = (samples * channels) * 2 ;
        pctx->samples_read = samples;
    }

    return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

void FlacDecoder::ErrorCallback(
        const FLAC__StreamDecoder *decoder, 
        FLAC__StreamDecoderErrorStatus status, 
        void *client_data)
{

}

