#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <core/CVertexBuffer.h>

namespace engine
{

    class CIndexBuffer
    {

    public :
        CIndexBuffer( const eBufferUsage& bufferUsage,
                      uint32 bufferCount,
                      uint32* bufferData,
                      bool track = false );

        ~CIndexBuffer();

        void updateData( uint32 bufferCount, uint32* bufferData );
        void bind();
        void unbind();

        eBufferUsage usage() const { return m_bufferUsage; }
        uint32 count() const { return m_bufferCount; }
        uint32 openglId() const { return m_openglId; }

    private :

        eBufferUsage    m_bufferUsage;
        uint32          m_bufferCount;
        uint32          m_openglId;
        bool            m_track; // @debug (checking python-side refs)
    };

}