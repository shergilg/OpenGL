/*#include <iostream>

int main()
{
	std::cout << "Hello" << std::endl;
	std::cin.get();
}*/


/*Include statements*/
#include <iostream> 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*Struct: more than one variable to represent an object*/
/*This represents source for the vertex and fragment shader files*/
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*Since we put both shaders in one file this just accepts that file and seperates both shader*/
/*We create enum class which is helps us to seprate each shader store them*/
static ShaderProgramSource ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }

    }

    return { ss[0].str(), ss[1].str() };
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*Compiles shader accepting type(Which is built in OpenGL) and the source for that shader*/
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); /*Pointer to the beginning of our data(Very first character in the string*/
    GLCall(glShaderSource(id, 1, &src, nullptr)); /*(shader, how many source codes we want, pointer to the pointer(to the string containing the source code, length)*/
    GLCall(glCompileShader(id)); /*Compiles the source code strings stored in the shader*/

    int result;
    /*Checking if the shader actually compiles*/
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));/*Returns parameter from a shader object(stored as 'int' in ''result'')*/
    /*Incase it doesn't, we write to the console which shader didn't compile*/
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);/*(Shader, Parameter name(OpenGL specified), the address of the parameter)*/
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;/*Return the compiled shader*/
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*We create program and attach both if the shaders after compiling and return the program*/
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}
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
            -0.5f, -0.5f, /*index 0*/
             0.5f, -0.5f, /*index 1*/
             0.5f,  0.5f, /*index 2*/
            -0.5f,  0.5f, /*index 3*/
        };

        /*Creating index buffer*/
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        /*Create VAO(Vertex Array object)*/
        unsigned int VAO;
        GLCall(glGenVertexArrays(1, &VAO));
        GLCall(glBindVertexArray(VAO));


        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        //making vertex buffer layout
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        

        IndexBuffer ib(indices, 6);

        ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shader)); /*Binding the shader*/

        int location = glGetUniformLocation(shader, "u_Color"); /*Get the location of the uniform by name*/
        GLCall(ASSERT(location != -1)); /*Check to see if it is being used*/
        GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f)); /*Set the values to the uniform*/

        /*Unbinding everthing*/
        GLCall(glBindVertexArray(0));
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            /*Calling these again because we unbinded these*/
            GLCall(glUseProgram(shader));
            GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));/*Upadte the color overtime*/

            va.Bind();

            ib.Bind();

            //glDrawArrays(GL_TRIANGLES, 0, 6);/*(What are we drawing, the starting point, and the number of vertrices)*/
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));/*Draw call to draw our trianles using index buffer*/

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }

        GLCall(glDeleteProgram(shader));
    }
    glfwTerminate();
    return 0;
}