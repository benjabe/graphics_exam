#include "Game.h"

Camera camera;
std::mt19937 gen;

Game::Game(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;


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
        1000.0f
    );

    // Add game objects
    //Shader cube_shader("vertex.shader", "fragment.shader");
    //m_game_objects.push_back(new Cube(cube_shader, "container2.jpg"));

    Shader shader("vertex.shader", "fragment.shader");
    Heightmap *heightmap = new Heightmap(shader, 800, 800);
    m_game_objects.push_back(heightmap);
    m_game_objects.push_back(m_deer = new Deer(
        heightmap,
        glm::vec3(0.0f, heightmap->map_height(0, 0), 0.0f)
    ));

    // Add light
    m_directional_light = {
        { 0.2f, -0.4f, 0.0f },
        { 0.2f, 0.2f, 0.2f },
        { 0.5f, 0.5f, 0.5f },
        { 0.6f, 0.6f, 0.6f }
    };

    m_last_time = glfwGetTime();
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

void Game::add_point_light(glm::vec3 position)
{
    m_point_lights.push_back({
        position,
        { 0.2f, 0.2f, 0.2f },
        { 0.9f, 0.2f, 0.9f },
        { 1.0f, 0.3f, 1.0f },
        1.0f,
        0.09f,
        0.032f
    });
}

void Game::add_raindrop()
{
    std::uniform_real_distribution<float> dis(-50.0f, 50.0f);
    glm::vec3 position = { 200.0f + dis(gen), 100, 200.0f + dis(gen) };
    m_game_objects.push_back(new Raindrop(position));
}

void Game::update(float delta_time)
{
    for (GameObject *go : m_game_objects)
    {
        go->update(delta_time);
    }

    if (glfwGetTime() >= m_next_spawn_time)
    {
        add_raindrop();
        m_next_spawn_time = glfwGetTime() + 1.0f / m_spawn_rate;
    }
}

void Game::render()
{
    glClearColor(0.2f, 0.3, 0.3, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_view = camera.get_view_matrix();

    for (GameObject *go : m_game_objects)
    {
        go->render(
            m_projection,
            m_view,
            camera.get_position(),
            m_directional_light,
            m_point_lights
        );
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

    // Camera controls
    Direction dir = NONE;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        dir = FORWARD;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        dir = BACKWARD;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        dir = LEFT;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        dir = RIGHT;
    }

    if (dir != NONE)
    {
        if (m_camera_mode == FREELOOK)
        {
            camera.process_keyboard(dir, delta_time);
        }
        else
        {
            glm::vec3 front = glm::normalize(glm::vec3(camera.get_front().x, 0.0f, camera.get_front().z));
            glm::vec3 right = glm::normalize(glm::vec3(camera.get_right().x, 0.0f, camera.get_right().z));
            m_deer->process_keyboard(dir, delta_time, front, right);
            camera.set_position(
                m_deer->get_position() + glm::vec3(0.0f, 5.3f, 0.0f) //+ front * 3.5f
            );
        }
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        add_point_light(camera.get_position());
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        switch (m_camera_mode)
        {
        case FIRST_PERSON:
            m_camera_mode = THIRD_PERSON;
            break;
        case THIRD_PERSON:
            m_camera_mode = FREELOOK;
            break;
        case FREELOOK:
            m_camera_mode = FIRST_PERSON;
            break;
        default:
            m_camera_mode = FREELOOK;
        }
    }
}