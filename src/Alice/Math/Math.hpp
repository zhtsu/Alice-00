#pragma once

#include <glm/glm.hpp>

namespace Alice
{

namespace Math
{

bool DecomposeTransform(const Mat4& transform, Vec3& out_translation, Vec3& out_ratation, Vec3& out_scale);

} // namespace Math

} // namespace Alice
