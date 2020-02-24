#pragma once

#ifdef ENGINE_USE_FFMPEG

#include <utils/CVideoCommon.h>

namespace engine
{
    enum class eDecodingMode : uint8_t
    {
        SYNC = 0,
        ASYNC
    };

    std::string ToString( const eDecodingMode& mode );

    enum class eDecodingState : uint8_t
    {
        IDLE = 0,
        READY,
        DECODING,
        FINISHED
    };

    std::string ToString( const eDecodingState& state );

    std::unique_ptr<uint8_t[]> DecodeOneFrame( AVFormatContext* format_ctx,
                                               AVCodecContext* codec_ctx,
                                               ssize_t video_frame_index );

    class CVideoDecoder
    {
    public :

        CVideoDecoder( const eDecodingMode& mode, const std::string& filepath = "" );
        CVideoDecoder( const CVideoDecoder& other ) = delete;
        CVideoDecoder& operator=( const CVideoDecoder& other ) = delete;
        ~CVideoDecoder();

        void Reset( const std::string& video_filepath );
        void BeginDecoding();
        void EndDecoding();

        std::unique_ptr<uint8_t[]> GetNextFrame();

        bool loop() const { return m_loop; }

        eDecodingMode mode() const { return m_mode; }

        eDecodingState state() const { return m_state; }

        std::string filepath() const { return m_filepath; }

    private :

        std::unique_ptr<AVCodecContext, AVCodecContextDeleter> m_codecContext;

        std::unique_ptr<AVFormatContext, AVFormatContextDeleter> m_formatContext;

        ssize_t m_videoStreamIndex;

        std::unique_ptr<uint8_t[]> m_lastFrameData;

        eDecodingMode m_mode;

        eDecodingState m_state;

        bool m_loop;

        std::string m_filepath;

        static bool s_InitializedFFMPEG;
    };

}

#endif /* ENGINE_USE_FFMPEG */