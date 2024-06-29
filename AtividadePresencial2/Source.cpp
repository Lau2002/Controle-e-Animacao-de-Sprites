
#include <iostream>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Sprite.h"

const GLint WIDTH = 800, HEIGHT = 600;
glm::mat4 matrix = glm::mat4(1);

float fw = 0.25f;
float fh = 0.25f;
int frameAtual = 3;
int acao = 0;
float offsetx = fw * (float)frameAtual, offsety = fh * (float)acao;
float previous = glfwGetTime();
int sign = 1;
int FPS = 8;
float frameTime = 1.0f / FPS;

void mouse(double mx, double my) {
    double dx = mx - WIDTH / 2;
    double dy = my - HEIGHT / 2;

    matrix = glm::translate(glm::mat4(1), glm::vec3(dx, dy, 0));
}


int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

#pragma region Basic Setup
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Controle e Animacao de Sprites - Laura Skorupski", nullptr, nullptr);


    if (window == nullptr) {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return EXIT_FAILURE;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#pragma endregion

    const char* vertex_shader =
        "#version 410\n"
        "layout ( location = 0 ) in vec3 vPosition;"
        "layout ( location = 1 ) in vec3 vColor;"
        "layout ( location = 2 ) in vec2 vTexture;"
        "uniform mat4 proj;"
        "uniform mat4 matrix;"
        "out vec2 text_map;"
        "out vec3 color;"
        "void main() {"
        "    color = vColor;"
        "    text_map = vTexture;"
        "    gl_Position = proj * matrix * vec4 ( vPosition, 1.0);"
        "}";


    const char* fragment_shader =
        "#version 410\n"
        "in vec2 text_map;"
        "in vec3 color;"
        "uniform sampler2D basic_texture;"
        "uniform float offsetx;"
        "uniform float offsety;"
        "uniform bool applyOffset;"
        "out vec4 frag_color;"
        "void main(){"
        "   vec2 offset_text_map = text_map;"
        "   if (applyOffset) {"
        "       offset_text_map = vec2(text_map.x + offsetx, text_map.y + offsety);"
        "   }\n"
        "   frag_color = texture(basic_texture, offset_text_map);"
        "}";

    int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    int shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);


    GLfloat vertices[] = {
        // Positions           // Colors          // Texture Coords
        0.0f,   0.0f,  0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // Bottom-left
        WIDTH,  0.0f,  0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // Bottom-right
        WIDTH, HEIGHT, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // Top-right
        0.0f,  HEIGHT, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f   // Top-left
    };

    GLfloat scale2 = 1.0f;

    GLfloat vertices6[] = {
        // Positions                             // Colors           // Texture Coords
        180.0f * scale2, 30.0f * scale2, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,     // Bottom-left
        280.0f * scale2, 30.0f * scale2, 0.0f,   1.0f, 1.0f, 0.0f,   0.25f, 0.0f, // Bottom-right
        180.0f * scale2, 130.0f * scale2, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.25f,         // Top-left
        280.0f * scale2, 130.0f * scale2, 0.0f,   0.0f, 1.0f, 0.0f,  0.25f, 0.25f      // Top-right
    };

    float sprite6PosX = 280.0f;
    float sprite6PosY = 30.0f;

    glm::mat4 proj = glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);

    Sprite sprite1(vertices, sizeof(vertices) / sizeof(vertices[0]), "background.png", proj);
    Sprite sprite6(vertices6, sizeof(vertices6) / sizeof(vertices6[0]), "character.png", proj);


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        double current_seconds = glfwGetTime();

#pragma region Input Handling

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            sprite6PosY -= 0.2f; // Move para baixo
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            sprite6PosY += 0.2f; // Move para baixo
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            sprite6PosX -= 0.2f; // Move para a esquerda
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            sprite6PosX += 0.2f; // Move para a direita
        }

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
#pragma endregion

        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int screenWidth, screenHeight;
        glfwGetWindowSize(window, &screenWidth, &screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);

        glUseProgram(shader_programme);
        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "matrix"), 1, GL_FALSE, glm::value_ptr(matrix));
        glUniform1f(glGetUniformLocation(shader_programme, "offsetx"), offsetx);
        glUniform1f(glGetUniformLocation(shader_programme, "offsety"), offsety);
        glUniform1i(glGetUniformLocation(shader_programme, "applyOffset"), GL_FALSE);
       

        sprite1.render(true);

       
        glm::mat4 model = glm::mat4(1.0f); // Matriz de identidade
        model = glm::translate(model, glm::vec3(sprite6PosX, sprite6PosY, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader_programme, "matrix"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(glGetUniformLocation(shader_programme, "offsetx"), offsetx);
        glUniform1f(glGetUniformLocation(shader_programme, "offsety"), offsety);
        glUniform1i(glGetUniformLocation(shader_programme, "applyOffset"), GL_TRUE);
        sprite6.render(false);

        if ((current_seconds - previous >= frameTime) && (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)) {
            previous = current_seconds;
            acao = 3;

            frameAtual = (frameAtual + 1) % 4;

            offsetx = fw * (float)frameAtual;
            offsety = fh * (float)acao;

        } else if ((current_seconds - previous >= frameTime) && (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)) {
            previous = current_seconds;
            acao = 0;

            frameAtual = (frameAtual + 1) % 4;

            offsetx = fw * (float)frameAtual;
            offsety = fh * (float)acao;

        } else if ((current_seconds - previous >= frameTime) && (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)) {
            previous = current_seconds;
            acao = 2;

            frameAtual = (frameAtual + 1) % 4;

            offsetx = fw * (float)frameAtual;
            offsety = fh * (float)acao;
        } else if ((current_seconds - previous >= frameTime) && (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)) {
            previous = current_seconds;
            acao = 1;

            frameAtual = (frameAtual + 1) % 4;

            offsetx = fw * (float)frameAtual;
            offsety = fh * (float)acao;
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}
