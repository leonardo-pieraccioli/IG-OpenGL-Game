#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

//prova commit

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// window colors
const float COLOR_BLACK[3] = { 0.0f, 0.0f, 0.0f };
const float COLOR_RED[3] = { 1.0f, 0.0f, 0.0f };
const float COLOR_GREEN[3] = { 0.0f, 1.0f, 0.0f };
const float COLOR_BLUE[3] = { 0.0f, 0.0f, 1.0f };

float currentWindowColor[3];

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Example_00", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(currentWindowColor[0], currentWindowColor[1], currentWindowColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // color change
    // -------------------------------------------------------------------------------------------------

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE
        && glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE
        && glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
    {
        std::copy(std::begin(COLOR_BLACK), std::end(COLOR_BLACK), std::begin(currentWindowColor));
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        std::cout << "R pressed" << std::endl;
        std::copy(std::begin(COLOR_RED), std::end(COLOR_RED), std::begin(currentWindowColor));
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        std::cout << "G pressed" << std::endl;
        std::copy(std::begin(COLOR_GREEN), std::end(COLOR_GREEN), std::begin(currentWindowColor));
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        std::cout << "B pressed" << std::endl;
        std::copy(std::begin(COLOR_BLUE), std::end(COLOR_BLUE), std::begin(currentWindowColor));
    }

}