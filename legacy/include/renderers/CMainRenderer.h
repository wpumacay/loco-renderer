#pragma once

#include <renderers/CRenderersCommon.h>
#include <renderers/CMeshRenderer.h>
#include <renderers/CSkyboxRenderer.h>

namespace engine
{

    const int32 RENDERER_SHADOWMAP_WIDTH    = 4096;
    const int32 RENDERER_SHADOWMAP_HEIGHT   = 4096;

    class CMainRenderer
    {

    public :

        CMainRenderer();
        ~CMainRenderer();

        void begin( const CRenderOptions& renderOptions );
        void submit( const std::vector< CIRenderable* >& renderables );
        void render();

        std::string status() const { return m_status; }
        CShadowMap* shadowMap() const { return m_shadowMap.get(); }

        CMeshRenderer* meshRenderer() const { return m_rendererMeshes.get(); }
        CSkyboxRenderer* skyboxRenderer() const { return m_rendererSkybox.get(); }

    private :

        void _collectRenderablesInView( const CFrustum& frustum,
                                        const eCullingGeom& cullGeometryToUse,
                                        const std::vector< CIRenderable* >& renderablesToCheck,
                                        std::vector< CIRenderable* >& renderablesInView );

        void _collectRenderablesInView_bbox( const CFrustum& frustum,
                                             const std::vector< CIRenderable* >& renderablesToCheck,
                                             std::vector< CIRenderable* >& renderablesInView );

        void _collectRenderablesInView_bsphere( const CFrustum& frustum,
                                                const std::vector< CIRenderable* >& renderablesToCheck,
                                                std::vector< CIRenderable* >& renderablesInView );

        void _collectMeshes( const std::vector< CIRenderable* >& renderables,
                             std::vector< CMesh* >& meshes );

    private :

        bool m_hasValidInfo;
        std::string m_status;
        CRenderOptions m_renderOptions;

        std::unique_ptr< CShadowMap >       m_shadowMap;
        std::unique_ptr< CMeshRenderer >    m_rendererMeshes;
        std::unique_ptr< CSkyboxRenderer >  m_rendererSkybox;
    };

}
