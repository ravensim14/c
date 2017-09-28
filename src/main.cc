#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    std::cout << "siim ei ole jumal" << std::endl;
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    printf("OpenGL Version: %s.\n", glGetString(GL_VERSION));
    printf("OpenGL GLSL Version: %s.\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("OpenGL Vendor: %s.\n", glGetString(GL_VENDOR));

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0f, 0.3f, 0.3f);

    static float point=5;
    static float angle=0;
    
    
	glPointSize(point);
    
    //point+=1;

	//glBegin(GL_POINTS);
	//glVertex2f(0.0f, 0.0f);
	//glEnd();

	//glBegin(GL_TRIANGLES);
	//glVertex2f(-0.5f, -0.5f);
	//glVertex2f(+0.5f, -0.5f);
	//glVertex2f(+0.0f, +0.5f);
	//glEnd();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 600.0f, 600.0f, 0.0f, 0.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glBegin(GL_TRIANGLES);
	//glVertex2f(0.0f, 0.0f);
	//glVertex2f(200.0f, 0.0f);
	//glVertex2f(100.0f, 200.0f);
	//glEnd();
    
    glBegin(GL_POINTS);
	static float hyp=40;
    angle+=1;
    static float x,y;
    static float cx=300,cy=300;
    static float pos[2] = {300, 300};
    // pos[0]
    // pos[1]
    
    
    x=cx+hyp*sin(angle);
    y=cy+hyp*cos(angle);
   
    glVertex2f(x, y);
     angle+=.01;
    
	glEnd();
    
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
