#include <iostream>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <vector>
using namespace std;

class Triangle {
    private:
        // Event loop flag
        bool quit = false;
        // Declare SDL Window
        SDL_Window* sdlWindow = nullptr;
        // Declare GL context
        SDL_GLContext GLContext = nullptr;
        // Declare VAO, VBO
        GLuint VAO, VBO, IBO;
        // Declare shader program
        GLuint shaderProgram;
    
    public:
        void setupSDLWindow();
        void setupGLContext();
        void setupGlad();
        void setupVertextSpecification();
        void startLoop();
        void quitApp();
        void createGraphicsPipeline();
        string getShaderSource(const string&);
        GLuint compileShader(int, string);
};

void GLErrorHandling(const char* fn, int line) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        cout << "function: " << fn << endl << "\tline: " << line << endl << "\terror: " << err << endl;
    }
}

#define check(x) x; GLErrorHandling(#x, __LINE__)

string Triangle::getShaderSource(const string& filepath) {
    string result = "";
    string line = "";

    ifstream myFile(filepath);
    if (myFile.is_open()) {
        while (getline(myFile, line)) {
            result += line + "\n";
        }
        myFile.close();
    }

    return result;
}

GLuint Triangle::compileShader(int type, string src) {
    const char* source = src.c_str();
    GLuint shader = glCreateShader(type);
    glShaderSource(
                shader,
                1,
                &source,
                nullptr
            );
    glCompileShader(shader);
    return shader;
}

void Triangle::createGraphicsPipeline() {
    shaderProgram = glCreateProgram();
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, getShaderSource("classes/shaders/vertex.glsl"));
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, getShaderSource("classes/shaders/fragment.glsl"));
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);
}

void Triangle::setupVertextSpecification() {
    vector<GLfloat> vertexData = {
        // position info    // color info
        // First triangle
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
        // Second triangle
        // 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        // -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    vector<GLuint> vertexIndices = { 0, 1, 2, 1, 3, 2 };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
                GL_ARRAY_BUFFER,
                vertexData.size() * sizeof(GLfloat),
                vertexData.data(),
                GL_STATIC_DRAW
            );

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                vertexIndices.size() * sizeof(GLuint),
                vertexIndices.data(),
                GL_STATIC_DRAW
            );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
                        0,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(GLfloat) * 6,
                        (GLvoid*)0
                    );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
                        1,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(GLfloat) * 6,
                        (GLvoid*)(sizeof(GLfloat) * 3)
                    );

    /*
    
        glGenBuffers(1, &VBOCol);
        glBindBuffer(GL_ARRAY_BUFFER, VBOCol);
        glBufferData(
                    GL_ARRAY_BUFFER,
                    vertexColors.size() * sizeof(GLfloat),
                    vertexColors.data(),
                    GL_STATIC_DRAW
                );

    */

    glBindVertexArray(0);
    // glDisableVertexAttribArray(0);
    // glDisableVertexAttribArray(1);
}

void Triangle::setupSDLWindow() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL init failed" << endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    sdlWindow = SDL_CreateWindow(
                        "SDL Window",
                        SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED,
                        640,
                        480,
                        SDL_WINDOW_OPENGL
                );

    if (sdlWindow == nullptr) {
        cout << "SDL window creation failed" << endl;
        exit(1);
    }
}

void Triangle::setupGLContext() {
    GLContext = SDL_GL_CreateContext(sdlWindow);

    if (GLContext == nullptr) {
        cout << "SDL context creation failed" << endl;
        exit(1);
    }
}

void Triangle::setupGlad() {
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        cout << "Glad initialize failed" << endl;
        exit(1);
    }
}

void Triangle::startLoop() {
    createGraphicsPipeline();
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                cout << "Bye!" << endl;
                quit = true;
            }
        }

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // glDrawArrays(GL_TRIANGLES, 0, 6);
        check(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        glUseProgram(0);

        SDL_GL_SwapWindow(sdlWindow);
    }
}

void Triangle::quitApp() {
    SDL_Quit();
    SDL_DestroyWindow(sdlWindow);
}
