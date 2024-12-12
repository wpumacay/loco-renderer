#include <memory>
#include <vector>

#include <renderer/engine/graphics/geometry_factory_t.hpp>

// Implementation based on ThreeJS geometries
// https://github.com/mrdoob/three.js/tree/dev/src/geometries

namespace renderer {

auto CreatePlane(float width, float depth, size_t widthSegments,
                 size_t depthSegments) -> Geometry::uptr {
    const float WIDTH_HALF = width / 2;
    const float DEPTH_HALF = depth / 2;

    const float SEGMENT_WIDTH = width / static_cast<float>(widthSegments);
    const float SEGMENT_DEPTH = depth / static_cast<float>(depthSegments);

    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec2> texcoords;
    std::vector<uint32_t> indices;

    for (size_t iy = 0; iy <= depthSegments; ++iy) {
        float y = static_cast<float>(iy) * SEGMENT_DEPTH - DEPTH_HALF;
        for (size_t ix = 0; ix <= widthSegments; ++ix) {
            float x = static_cast<float>(ix) * SEGMENT_WIDTH - WIDTH_HALF;
            float u =
                static_cast<float>(ix) / static_cast<float>(widthSegments);
            float v = 1.0F - static_cast<float>(iy) /
                                 static_cast<float>(depthSegments);

            positions.emplace_back(x, -y, 0.0F);
            normals.emplace_back(0.0F, 0.0F, 1.0F);
            texcoords.emplace_back(u, v);
        }
    }

    for (size_t iy = 0; iy < depthSegments; ++iy) {
        for (size_t ix = 0; ix < widthSegments; ++ix) {
            auto a = ix + (widthSegments + 1) * iy;
            auto b = ix + (widthSegments + 1) * (iy + 1);
            auto c = (ix + 1) + (widthSegments + 1) * (iy + 1);
            auto d = (ix + 1) + (widthSegments + 1) * iy;

            indices.push_back(static_cast<uint32_t>(a));
            indices.push_back(static_cast<uint32_t>(b));
            indices.push_back(static_cast<uint32_t>(d));

            indices.push_back(static_cast<uint32_t>(b));
            indices.push_back(static_cast<uint32_t>(c));
            indices.push_back(static_cast<uint32_t>(d));
        }
    }

    const auto N_VERTICES = positions.size();
    const auto N_INDICES = indices.size();
    return std::make_unique<Geometry>(
        N_VERTICES, positions.front().data(), normals.front().data(),
        texcoords.front().data(), N_INDICES, indices.data());
}

auto CreateBox(float width, float depth, float height) -> Geometry::uptr {
    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec2> texcoords;
    std::vector<uint32_t> indices;

    // clang-format off
    std::vector<Vec3> normals_source = {
        { 0.0F,  0.0F,  1.0F},
        { 0.0F,  0.0F, -1.0F},
        { 0.0F,  1.0F,  0.0F},
        { 0.0F, -1.0F,  0.0F},
        { 1.0F,  0.0F,  0.0F},
        {-1.0F,  0.0F,  0.0F}};
    // clang-format on

    Vec3 scale(width / 2, depth / 2, height / 2);
    for (auto n : normals_source) {
        indices.push_back(static_cast<uint32_t>(positions.size()));
        indices.push_back(static_cast<uint32_t>(positions.size() + 1));
        indices.push_back(static_cast<uint32_t>(positions.size() + 2));
        indices.push_back(static_cast<uint32_t>(positions.size()));
        indices.push_back(static_cast<uint32_t>(positions.size() + 2));
        indices.push_back(static_cast<uint32_t>(positions.size() + 3));

        // Form a right hand system based on the current normal
        Vec3 s1 = {n.y(), n.z(), n.x()};
        Vec3 s2 = ::math::cross(n, s1);

        auto v0 = (n - s1 - s2) * (scale);
        auto v1 = (n + s1 - s2) * (scale);
        auto v2 = (n + s1 + s2) * (scale);
        auto v3 = (n - s1 + s2) * (scale);

        positions.push_back(v0);
        positions.push_back(v1);
        positions.push_back(v2);
        positions.push_back(v3);

        normals.push_back(n);
        normals.push_back(n);
        normals.push_back(n);
        normals.push_back(n);

        texcoords.emplace_back(0.0F, 0.0F);
        texcoords.emplace_back(1.0F, 0.0F);
        texcoords.emplace_back(1.0F, 1.0F);
        texcoords.emplace_back(0.0F, 1.0F);
    }

    const auto N_VERTICES = positions.size();
    const auto N_INDICES = indices.size();
    return std::make_unique<Geometry>(
        N_VERTICES, positions.front().data(), normals.front().data(),
        texcoords.front().data(), N_INDICES, indices.data());
}

}  // namespace renderer
