#ifndef SPRITE_H
#define SPRITE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Sprite {
public:
    Sprite(GLfloat* vertices, GLsizei vertexCount, const char* texturePath, glm::mat4 proj);
    ~Sprite();
    void render(bool isBackground);

private:
    GLuint VAO, VBO, texture;
    GLsizei vertexCount;
    glm::mat4 proj;

    void setupObject(GLfloat* vertices);
    void loadTexture(const char* texturePath);
};

#endif // SPRITE_H
