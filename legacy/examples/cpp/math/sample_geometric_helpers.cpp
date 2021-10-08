
#include <CEngine.h>

struct ComparatorDistPlane
{
    engine::CPlane plane;

    bool operator() ( const engine::CVec3& p1, const engine::CVec3& p2 )
    {
        return engine::distToPlane( p1, plane ) < engine::distToPlane( p2, plane ) ;
    }
};

struct ComparatorSignedDistPlane
{
    engine::CPlane plane;

    bool operator() ( const engine::CVec3& p1, const engine::CVec3& p2)
    {
        return engine::signedDistToPlane( p1, plane ) < engine::signedDistToPlane( p2, plane );
    }
};

auto g_plane_size = engine::CVec2( 2.0f, 2.0f );
auto g_plane_normal = engine::CVec3( 0.0f, 1.0f, 0.0f );
auto g_plane_position = engine::CVec3( 1.0f, 1.0f, 1.0f );
engine::CPlane g_plane = { g_plane_normal, g_plane_position };

ComparatorDistPlane g_comparator = { g_plane };
ComparatorSignedDistPlane g_signedComparator = { g_plane };

auto g_box_size = engine::CVec3( 0.8f, 0.8f, 0.8f );
auto g_box_position = engine::CVec3( 3.0f, 3.0f, 3.0f );
auto g_box_rotation = engine::CVec3( ENGINE_PI / 6.0f, ENGINE_PI / 6.0f, ENGINE_PI / 6.0f );
auto g_box_transform = engine::translation( g_box_position ) * engine::rotation( tinymath::rotation( g_box_rotation ) );

auto g_sphere_size = 0.5f;
auto g_sphere_position = engine::CVec3( -1.0f, 1.0f, -1.0f );
auto g_sphere_rotation = engine::CVec3( ENGINE_PI / 6.0f, ENGINE_PI / 6.0f, ENGINE_PI / 6.0f );
auto g_sphere_transform = engine::translation( g_sphere_position ) * engine::rotation( tinymath::rotation( g_sphere_rotation ) );

bool g_use_signed_distance = false;
bool g_show_comparison_with_frustum = false;

class GeometricHelpersLayer : public engine::CImGuiLayer
{

public :

    GeometricHelpersLayer( const std::string& name )
        : engine::CImGuiLayer( name ) {}

    ~GeometricHelpersLayer() {}


    void render() override
    {
        m_wantsToCaptureMouse = false;

        _menuUiHandleBox();
        _menuUiHandleSphere();
        _menuUiHandlePlane();

        ImGuiIO& io = ImGui::GetIO();
        m_wantsToCaptureMouse = io.WantCaptureMouse;
    }

    bool onEvent( const engine::CInputEvent& event ) override
    {
        if ( event.type() == engine::eEventType::MOUSE_PRESSED )
            return m_wantsToCaptureMouse;

        return false;
    }

private :

    void _menuUiHandleBox()
    {
        ImGui::Begin( "Play around with the box" );

        ImGui::Checkbox( "comparator-use-signed-dist", &g_use_signed_distance );
        ImGui::Checkbox( "show-comparison-with-frustum", &g_show_comparison_with_frustum );

        float _bSize[3]     = { g_box_size.x(), g_box_size.y(), g_box_size.z() };
        float _bPosition[3] = { g_box_position.x(), g_box_position.y(), g_box_position.z() };
        float _bRotation[3] = { g_box_rotation.x(), g_box_rotation.y(), g_box_rotation.z() };

        ImGui::SliderFloat3( "bSize", _bSize, 0.2f, 2.0f );
        ImGui::SliderFloat3( "bPosition", _bPosition, -5.0f, 5.0f );
        ImGui::SliderFloat3( "bRotation", _bRotation, -ENGINE_PI / 2.0f, ENGINE_PI / 2.0f );

        g_box_size      = { _bSize[0], _bSize[1], _bSize[2] };
        g_box_position  = { _bPosition[0], _bPosition[1], _bPosition[2] };
        g_box_rotation  = { _bRotation[0], _bRotation[1], _bRotation[2] };
        g_box_transform = engine::translation( g_box_position ) * tinymath::rotation( g_box_rotation );

        ImGui::Spacing();
        ImGui::Text( "world-transform:" );
        ImGui::Text( engine::toString( g_box_transform ).c_str() );
        ImGui::End();
    }

    void _menuUiHandleSphere()
    {
        ImGui::Begin( "Play around with the sphere" );

        float _sPosition[3] = { g_sphere_position.x(), g_sphere_position.y(), g_sphere_position.z() };
        float _sRotation[3] = { g_sphere_rotation.x(), g_sphere_rotation.y(), g_sphere_rotation.z() };

        ImGui::SliderFloat( "sSize", &g_sphere_size, 0.2f, 2.0f );
        ImGui::SliderFloat3( "sPosition", _sPosition, -5.0f, 5.0f );
        ImGui::SliderFloat3( "sRotation", _sRotation, -ENGINE_PI / 2.0f, ENGINE_PI / 2.0f );

        g_sphere_position  = { _sPosition[0], _sPosition[1], _sPosition[2] };
        g_sphere_rotation  = { _sRotation[0], _sRotation[1], _sRotation[2] };
        g_sphere_transform = engine::translation( g_sphere_position ) * tinymath::rotation( g_sphere_rotation );

        ImGui::Spacing();
        ImGui::Text( "world-transform:" );
        ImGui::Text( engine::toString( g_sphere_transform ).c_str() );
        ImGui::End();
    }

    void _menuUiHandlePlane()
    {
        ImGui::Begin( "Play around with the plane" );

        float _pSize[2]     = { g_plane_size.x(), g_plane_size.y() };
        float _pNormal[3]   = { g_plane_normal.x(), g_plane_normal.y(), g_plane_normal.z() };
        float _pPosition[3] = { g_plane_position.x(), g_plane_position.y(), g_plane_position.z() };

        ImGui::SliderFloat2( "pSize", _pSize, 0.2f, 2.0f );
        ImGui::SliderFloat3( "pNormal", _pNormal, -1.0f, 1.0f );
        ImGui::SliderFloat3( "pPosition", _pPosition, -5.0f, 5.0f );

        g_plane_size     = { _pSize[0], _pSize[1] };
        g_plane_normal   = { _pNormal[0], _pNormal[1], _pNormal[2] };
        g_plane_position = { _pPosition[0], _pPosition[1], _pPosition[2] };

        g_plane.normal = g_plane_normal;
        g_plane.position = g_plane_position;
        g_comparator.plane = g_plane;
        g_signedComparator.plane = g_plane;

        ImGui::Spacing();
        ImGui::Text( ( "normal      : " + engine::toString( g_plane_normal ) ).c_str() );
        ImGui::Text( ( "position    : " + engine::toString( g_plane_position ) ).c_str() );
        ImGui::Text( ( "size        : " + engine::toString( g_plane_size ) ).c_str() );
        ImGui::End();
    }

    bool m_wantsToCaptureMouse;
};

int main()
{
    auto _app = std::make_unique<engine::CApplication>();
    auto _uiLayer = std::make_unique<GeometricHelpersLayer>( "GeometricHelpers-utils" );
    auto _uiLayerRef = dynamic_cast<GeometricHelpersLayer*>( _app->addGuiLayer( std::move( _uiLayer ) ) );

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = _app->window()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 50.0f;

    auto _camera = std::make_unique<engine::COrbitCamera>( "orbit",
                                                           engine::CVec3( 0.0f, 0.0f, 3.0f ),
                                                           engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                                           engine::eAxis::Y,
                                                           _cameraProjData );

    auto _cameraRef = _app->scene()->AddCamera( std::move( _camera ) );

    auto _cameraProjDataTest = engine::CCameraProjData();
    _cameraProjDataTest.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjDataTest.fov         = 45.0f;
    _cameraProjDataTest.aspect      = _app->window()->aspect();
    _cameraProjDataTest.zNear       = 1.0f;
    _cameraProjDataTest.zFar        = 3.0f;

    auto _cameraTest = std::make_unique<engine::CFixedCamera>( "fixed",
                                                               engine::CVec3( -3.0f, 3.0f, -3.0f ),
                                                               engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                                               engine::eAxis::Y,
                                                               _cameraProjDataTest );

    engine::CVec3 _point = { 2.0f, 3.0f, 2.0f };

    while( _app->active() )
    {
        tinyutils::Clock::Tick();
        if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_ESCAPE ) )
            break;
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_SPACE ) )
            _cameraRef->SetActiveMode( false );
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_ENTER ) )
            _cameraRef->SetActiveMode( true );

        if ( _cameraRef->type() == engine::CFpsCamera::GetStaticType() )
        {
            if ( _cameraRef->active() )
                _app->window()->disableCursor();
            else
                _app->window()->enableCursor();
        }

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        engine::CDebugDrawer::DrawClipVolume( _cameraTest->mat_proj() * _cameraTest->mat_view(), { 1.0f, 1.0f, 0.0f } );

        _app->update();
        _app->begin();

        if ( !g_show_comparison_with_frustum )
        {
            engine::CDebugDrawer::DrawPlane( g_plane, g_plane_size, { 0.7f, 0.5f, 0.3f } );
            engine::CDebugDrawer::DrawLine( _point, g_plane.position, { 0.1f, 0.8f, 0.1f } );
            engine::CDebugDrawer::DrawLine( _point, engine::projInPlane( _point, g_plane ), { 0.1f, 0.1f, 0.8f } );
            engine::CDebugDrawer::DrawLine( g_plane.position, engine::projInPlane( _point, g_plane ), { 0.8f, 0.1f, 0.1f } );

            auto _boxCorners = engine::computeBoxCorners( { g_box_size, g_box_transform } );
            if ( g_use_signed_distance )
                std::sort( _boxCorners.begin(), _boxCorners.end(), g_signedComparator );
            else
                std::sort( _boxCorners.begin(), _boxCorners.end(), g_comparator );

            auto _minVertex = _boxCorners.front();
            auto _maxVertex = _boxCorners.back();

            //// engine::CVec3 _minVertex, _maxVertex;
            //// engine::computeMinMaxVertexToPlane( g_plane, { g_box_size, g_box_transform }, _minVertex, _maxVertex );

            engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, _minVertex, { 0.8f, 0.8f, 0.2f } );
            engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, _maxVertex, { 0.8f, 0.2f, 0.8f } );

            engine::CDebugDrawer::DrawBox( g_box_size, g_box_transform, { 0.2f, 0.8f, 0.8f } );
        }
        else
        {
            engine::CFrustum _frustum( _cameraTest->mat_proj() * _cameraTest->mat_view() );
            for ( size_t i = 0; i < _frustum.planes.size(); i++ )
                engine::CDebugDrawer::DrawArrow( _frustum.planes[i].position,
                                                 _frustum.planes[i].position + 0.2f * _frustum.planes[i].normal,
                                                 { 0.5f, 0.7f, 0.9f } );

            // check if cube is outside the frustum of the test-cam with high chance
            if ( engine::certainlyOutsideFrustum( _frustum, { g_box_size, g_box_transform } ) )
                engine::CDebugDrawer::DrawBox( g_box_size, g_box_transform, { 0.1f, 0.8f, 0.1f } );
            else
                engine::CDebugDrawer::DrawBox( g_box_size, g_box_transform, { 0.1f, 0.1f, 0.8f } );
        }

        {
            engine::CFrustum _frustum( _cameraTest->mat_proj() * _cameraTest->mat_view() );
            // check if sphere is outside the frustum of the test-cam with high chance
            if ( engine::certainlyOutsideFrustum( _frustum, { g_sphere_size, g_sphere_position } ) )
                engine::CDebugDrawer::DrawSphere( g_sphere_size, g_sphere_transform, { 0.1f, 0.8f, 0.1f } );
            else
                engine::CDebugDrawer::DrawSphere( g_sphere_size, g_sphere_transform, { 0.1f, 0.1f, 0.8f } );
        }

        _app->render();
        _app->end();
        tinyutils::Clock::Tock();
    }

    return 0;
}
