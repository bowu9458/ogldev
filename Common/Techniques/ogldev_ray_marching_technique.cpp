/*
    Copyright 2022 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ogldev_util.h"
#include "Techniques\ogldev_ray_marching_technique.h"


RayMarchingTechnique::RayMarchingTechnique()
{
}

bool RayMarchingTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "../Common/Shaders/ray_marching.vs")) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, "../Common/Shaders/color.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    m_colorLoc = GetUniformLocation("gColor");

    if (m_colorLoc == INVALID_UNIFORM_LOCATION) {
        return false;
    }

    Enable();

    // default is white
    SetColor(1.0f, 1.0f, 1.0f);

    return true;
}


void RayMarchingTechnique::SetColor(float r, float g, float b)
{
    glUniform4f(m_colorLoc, r, g, b, 1.0f);
}
