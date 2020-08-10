#pragma once
#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*Assert to make sure the code breaks and doesn't continue after it fails()*/
#define ASSERT(x) if (!(x)) __debugbreak();
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*This make checks if there are errors for the function(x)*/
/*If there are any errors it check for them and give the code for the error*/
/*Not that reliable*/
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*This make sures that there are no errors*/
void GLClearError();

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*Since ''glGetError'' should only be used inside the loop we check if theres an error*/
/*But ''glGetError'' doesn't specify which function, line caused the error or where the error was since it only returns the error code*/
/*We pass in the function, file isn't as important but tells which file contains the function, and the line to tell which line has the function in the file*/
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};

