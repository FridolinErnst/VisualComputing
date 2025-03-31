#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

//#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
//#include <learnopengl/model.h>

//#include <iostream>

//#include <fstream>
#include <vector>

struct PointData {
    float x, y, z;       // Position
    float r, g, b;       // Color
    float nx, ny, nz;    // Normal (optional)
};

std::vector<PointData> LoadPTS(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open .pts file: " << filename << std::endl;
        return {};
    }


    // Read the integer (number of points)
    int numPoints = 0;
    file >> numPoints;
    std::cout << "num of points: " << numPoints << std::endl;

    std::vector<PointData> points;
    points.reserve(numPoints);

    // For debugging, track how many lines are successfully read
    int successfulReads = 0;

    for (int i = 0; i < numPoints; i++)
    {
        PointData p;
        // Attempt to read 9 floats
        if (!(file >> p.x >> p.y >> p.z
            >> p.r >> p.g >> p.b
            >> p.nx >> p.ny >> p.nz))
        {
            // If reading fails, print the line index and stop
            std::cerr << "Parsing failed at data line " << i + 1
                << " (which might be file line " << (i + 2)
                << " if the first line is not numeric)."
                << std::endl;
            break;
        }

        points.push_back(p);
        successfulReads++;

        // Optional: periodically print progress
        if (i % 100000 == 0 && i > 0) {
            std::cout << "Read " << i << " points so far..." << std::endl;
        }
    }

    file.close();

    std::cout << "Loaded " << successfulReads << " points out of "
        << numPoints << " expected." << std::endl;

    return points;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 20.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    // we don't need this for the dinosaur model since we dont use textures
    //stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

    // Load the .pts file
    // Enter your path to the point source and make sure that the first line is the number of total points and from the second line on its the points (no comment at top)
    std::string path = "C:/Users/YOURNAME/Documents/Uni/VisualComputing/task 1/saurier-normal.pts";
    std::cout << "Path: " << path << std::endl;
    auto points = LoadPTS(path);

    std::cout << "Loaded " << points.size() << " points.\n";



    // find out where model is in the world
    glm::vec3 minPt(FLT_MAX), maxPt(-FLT_MAX);
    for (const auto& p : points) {
        minPt.x = std::min(minPt.x, p.x);
        minPt.y = std::min(minPt.y, p.y);
        minPt.z = std::min(minPt.z, p.z);

        maxPt.x = std::max(maxPt.x, p.x);
        maxPt.y = std::max(maxPt.y, p.y);
        maxPt.z = std::max(maxPt.z, p.z);
    }
    glm::vec3 center = (minPt + maxPt) * 0.5f;
    std::cout << "Model center: " << center.x << ", " << center.y << ", " << center.z << std::endl;

    // Create and configure the VAO/VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        points.size() * sizeof(PointData),
        points.data(),
        GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PointData), (void*)offsetof(PointData, x));
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PointData), (void*)offsetof(PointData, r));
    glEnableVertexAttribArray(1);

    // Normal attribute (optional)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PointData), (void*)offsetof(PointData, nx));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     
    
    // Enable programmatic control of point size
    glEnable(GL_PROGRAM_POINT_SIZE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // 3. Adjust the point size using UP/DOWN keys
        static float pointSize = 10.0f; // setting here is fine its only set in the first entrance to 10
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            pointSize += 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            pointSize -= 0.1f;
            if (pointSize < 1.0f) pointSize = 1.0f;
        }

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();
        ourShader.setFloat("uPointSize", pointSize);
        // here we set our single light and add colour to it
        ourShader.setVec3("lightPos", glm::vec3(0.0f, 10.0f, 0.0f));
        //ourShader.setVec3("lightPos", glm::vec3(2.25f, 16.0f, -29.0f)); // adjusted to the center of model and then up

        ourShader.setVec3("lightColor", glm::vec3(1.0, 1.0, 1.0));
        ourShader.setBool("roundPoints", true);

        // Set transform uniforms (projection, view, model)

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        ourShader.setMat4("model", model);

        // Now draw the points
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, points.size());
        // Were using GPU point rendering (one VAO, one draw call).
        // it’s a single call to glDrawArrays(GL_POINTS, …) with hardware-accelerated point sprite
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
