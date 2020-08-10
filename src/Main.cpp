/*#include <iostream>

int main()
{
	std::cout << "Hello" << std::endl;
	std::cin.get();
}*/


/*Include statements*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream> 
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*Main Program*/
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (GLEW_OK != glewInit())
    {
        std::cout << "Error" << std::endl;
    }

    /*Prints the version of opengl and the drivers version of the GPU*/
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        /*creating the buffer data*/
        float positions[] = { /*You can also call this a buffer*/
            -0.5f, -0.5f, 0.0f, 0.0f,/*index 0*/
             0.5f, -0.5f, 1.0f, 0.0f,/*index 1*/
             0.5f,  0.5f, 1.0f, 1.0f,/*index 2*/
            -0.5f,  0.5f, 0.0f, 1.0f/*index 3*/
        };

        /*Creating index buffer*/
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        
        /*Create VAO(Vertex Array object)*/
        unsigned int VAO;
        GLCall(glGenVertexArrays(1, &VAO));
        GLCall(glBindVertexArray(VAO));


        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        //making vertex buffer layout
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        

        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        Texture texture("res/textures/kTjDnJ8.jpg");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);


        /*Unbinding everthing*/
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            renderer.Draw(va, ib, shader);

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}