// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

#ifdef CP_GLM

template<typename T>
glm::tvec3<T> safe_normalize(const glm::tvec3<T>& v, T epsilon = T(1e-6))
{
    T lengthSquared = glm::dot(v, v);
    if (lengthSquared < epsilon * epsilon)
        return glm::tvec3<T>(0);
    return v * glm::inversesqrt(lengthSquared);
}

template<typename T, glm::qualifier Q>
glm::mat<4, 4, T, Q> to_left_up_forward(const glm::mat<4, 4, T, Q>& mat)
{
    return glm::mat4(mat[0], mat[1], -mat[2], mat[3]);
}

#endif
