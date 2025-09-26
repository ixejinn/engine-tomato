#ifndef ORANGE_GAMEMANAGER_H
#define ORANGE_GAMEMANAGER_H

#endif //ORANGE_GAMEMANAGER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

class GameManager {
private:
    GLFWwindow* window{nullptr};

    const int windowWidth{1600};
    const int windowHeight{900};

    void processInput();
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

public:
    GameManager();
    ~GameManager();

    void Run();
};