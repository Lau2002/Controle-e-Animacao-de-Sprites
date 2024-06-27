#include "Sprite.h"
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "stb_image.h"

// Fun��o auxiliar para carregar texturas
bool load_texture(const char* file_name, GLuint* tex) {
    int x, y, n;
    int force_channels = 4;
    glEnable(GL_TEXTURE_2D);
    unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);
    if (!image_data) {
        return false;
    }

    glGenTextures(1, tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    GLfloat max_aniso = 0.0f;

    return true;
}

Sprite::Sprite(GLfloat* vertices, GLsizei vertexCount, const char* texturePath, glm::mat4 proj)
    : vertexCount(vertexCount), proj(proj) {
    setupObject(vertices);
    loadTexture(texturePath);
}

Sprite::~Sprite() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture);
}

void Sprite::setupObject(GLfloat* vertices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Sprite::loadTexture(const char* texturePath) {
    if (!load_texture(texturePath, &texture)) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
    }
}

void Sprite::render(bool isBackground) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    if (isBackground) {
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount / 8);
    }
    else {
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCount / 8);
    }
    glBindVertexArray(0);
}

