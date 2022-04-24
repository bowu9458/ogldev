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
#include "ogldev_sprite_technique.h"


SpriteTechnique::SpriteTechnique()
{
}

bool SpriteTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "../Common/Shaders/sprite.vs")) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, "../Common/Shaders/sprite.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    m_samplerLoc = GetUniformLocation("gSampler");

    if (m_samplerLoc == INVALID_UNIFORM_LOCATION) {
        return false;
    }

    char name[200] = { 0 };

    /*    for (int i = 0 ; i < SPRITE_TECH_MAX_QUADS ; i++) {
        SNPRINTF(name, sizeof(name), "gQuads[%d].BasePos", i);
        m_quadsLoc[i].BasePos = GetUniformLocation(name);
        SNPRINTF(name, sizeof(name), "gQuads[%d].WidthHeight", i);
        m_quadsLoc[i].WidthHeight = GetUniformLocation(name);
        SNPRINTF(name, sizeof(name), "gQuads[%d].TexCoords", i);
        m_quadsLoc[i].TexCoords = GetUniformLocation(name);
        SNPRINTF(name, sizeof(name), "gQuads[%d].TexWidthHeight", i);
        m_quadsLoc[i].TexWidthHeight = GetUniformLocation(name);

        if ((m_quadsLoc[i].BasePos == INVALID_UNIFORM_LOCATION) ||
            (m_quadsLoc[i].WidthHeight == INVALID_UNIFORM_LOCATION) ||
            (m_quadsLoc[i].TexCoords == INVALID_UNIFORM_LOCATION) ||
            (m_quadsLoc[i].TexWidthHeight == INVALID_UNIFORM_LOCATION)) {
            return false;
        }
        }*/

    Enable();

    GLuint BlockIndex = glGetUniformBlockIndex(m_shaderProg, "QuadInfo");
    printf("BlockIndex %d\n", BlockIndex);

    glGetActiveUniformBlockiv(m_shaderProg, BlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &m_blockSize);

    printf("Size %d\n", m_blockSize);

    m_quadInfoBuffer = (GLubyte*)malloc(m_blockSize);

    const GLchar* Names[] = { "BasePos", "WidthHeight", "TexCoords", "TexWidthHeight" };
    GLuint Indices[4] = { 0 };
    glGetUniformIndices(m_shaderProg, 4, Names, Indices);

    GLint Offsets[4];
    glGetActiveUniformsiv(m_shaderProg, 4, Indices, GL_UNIFORM_OFFSET, Offsets);

    m_quadInfoOffsets.BasePos        = Offsets[0];
    m_quadInfoOffsets.WidthHeight    = Offsets[1];
    m_quadInfoOffsets.TexCoords      = Offsets[2];
    m_quadInfoOffsets.TexWidthHeight = Offsets[3];

    for (uint i = 0 ; i < 4 ; i++) {
        printf("%s: %d %d\n", Names[i], Indices[i], Offsets[i]);
    }

    glGenBuffers(1, &m_uniformBuffer);
    printf("Uniform buffer %d\n", m_uniformBuffer);

    return true;
}


void SpriteTechnique::SetTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_samplerLoc, TextureUnit);
}


void SpriteTechnique::SetQuad(int Index,
                              float NDCX, float NDCY, float Width, float Height,
                              float u, float v, float TexWidth, float TexHeight)
{
    assert(Index < SPRITE_TECH_MAX_QUADS);

    Vector2f* pBasePos        = (Vector2f*)(m_quadInfoBuffer + m_quadInfoOffsets.BasePos);
    Vector2f* pWidthHeight    = (Vector2f*)(m_quadInfoBuffer + m_quadInfoOffsets.WidthHeight);
    Vector2f* pTexCoords      = (Vector2f*)(m_quadInfoBuffer + m_quadInfoOffsets.TexCoords);
    Vector2f* pTexWidthHeight = (Vector2f*)(m_quadInfoBuffer + m_quadInfoOffsets.TexWidthHeight);

    pBasePos[Index].x = NDCX;
    pBasePos[Index].y = NDCY;
    pWidthHeight[Index].x = Width;
    pWidthHeight[Index].y = Height;
    pTexCoords[Index].x = u;
    pTexCoords[Index].y = v;
    pTexWidthHeight[Index].x = TexWidth;
    pTexWidthHeight[Index].y = TexHeight;

    /*    glUniform2f(m_quadsLoc[Index].BasePos, NDCX, NDCY);
    glUniform2f(m_quadsLoc[Index].WidthHeight, Width, Height);

    glUniform2f(m_quadsLoc[Index].TexCoords, u, v);
    glUniform2f(m_quadsLoc[Index].TexWidthHeight, TexWidth, TexHeight);*/
}


void SpriteTechnique::UpdateProgram()
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffer);
    glBufferData(GL_UNIFORM_BUFFER, m_blockSize, m_quadInfoBuffer, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uniformBuffer);
}
