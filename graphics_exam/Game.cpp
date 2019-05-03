#include "Game.h"

Camera camera;

Game::Game(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;

    m_last_time = 0.0f;

    m_first_mouse = true;
    m_last_mouse_pos = glm::vec2(m_width / 2, m_height / 2);

}

Game::~Game()
{
}

void Game::start()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(
        m_width,
        m_height,
        "EXAM OH SHIT",
        nullptr,
        nullptr
    );
    if (m_window == nullptr)
    {
        glfwTerminate();
        std::cout << "ERROR::GAME::Failed to create window.\n";
    }
    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR::GAME::Failed to load gl functions.\n";
    }

    glViewport(0, 0, m_width, m_height);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(m_window, mouse_callback);
    glEnable(GL_DEPTH_TEST);

    m_view = glm::mat4(1.0f);
    m_projection = glm::perspective(
        glm::radians(90.0f), 
        (float)m_width / (float)m_height, 
        0.1f, 
        500.0f
    );

    Shader cube_shader("vertex.shader", "fragment.shader");
    m_game_objects.push_back(new Cube(cube_shader, "container2.jpg"));

    Shader shader("vertex.shader", "fragment.shader");
    m_game_objects.push_back(new Heightmap(shader, 500, 500));

    while (!glfwWindowShouldClose(m_window))
    {
        // Handle time
        float current_time = glfwGetTime();
        float delta_time = current_time - m_last_time;
        m_last_time = current_time;

        // Process input
        process_input(m_window, delta_time);

        // Update stuff
        update(delta_time);

        // Draw stuff
        render();

        glfwPollEvents();
    }
}

void Game::update(float delta_time)
{
    for (GameObject *go : m_game_objects)
    {
        go->update(delta_time);
    }
}

void Game::render()
{
    glClearColor(0.2f, 0.3, 0.3, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_view = camera.get_view_matrix();

    for (GameObject *go : m_game_objects)
    {
        go->render(m_projection, m_view);
    }

    glfwSwapBuffers(m_window);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    static bool s_first_mouse = true;
    static glm::vec2 s_last_mouse_pos = { 0.0f, 0.0f };
    if (s_first_mouse)
    {
        s_last_mouse_pos = { xpos, ypos };
        s_first_mouse = false;
    }
    float x_offset = xpos - s_last_mouse_pos.x;
    float y_offset = s_last_mouse_pos.y - ypos;
    s_last_mouse_pos = { xpos, ypos };

    camera.process_mouse_movement(x_offset, y_offset);
}

void Game::process_input(GLFWwindow *window, float delta_time)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.process_keyboard(FORWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.process_keyboard(BACKWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.process_keyboard(LEFT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.process_keyboard(RIGHT, delta_time);
    }
}