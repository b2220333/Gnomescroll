/* Gnomescroll, Copyright (c) 2013 Symbolic Analytics
 * Licensed under GPLv3 */
#include "shader_loader.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <common/compat_gl.h>

//info log print
void print_shader_info_log(GLhandleARB obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog = NULL;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        if (infoLog[0] != '\0')
        {
            printf("%s",infoLog);
            if (charsWritten > 0 && infoLog[charsWritten-1] != '\n')
                printf("\n");
        }
        free(infoLog);
    }
}

void print_program_info_log(GLhandleARB obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        if (infoLog[0] != '\0')
        {
            printf("%s",infoLog);
            if (charsWritten > 0 && infoLog[charsWritten-1] != '\n')
                printf("\n");
        }
        free(infoLog);
    }
}

// loads a vertex and fragment shader into a shader
void load_shaders(const char *vert, const char* frag, GLhandleARB* prog)
{
    GLhandleARB v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    GLhandleARB f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

    size_t size = 0;
    char* vs = read_file_to_buffer(vert, &size);
    IF_ASSERT(vs == NULL) return;
    char* fs = read_file_to_buffer(frag, &size);
    IF_ASSERT(fs == NULL)
    {
        free(vs);
        return;
    }

    glShaderSourceARB(v, 1, (const char**)&vs, NULL);
    glShaderSourceARB(f, 1, (const char**)&fs, NULL);

    free(vs);
    free(fs);

    glCompileShaderARB(v); print_shader_info_log(v); ///diag
    glCompileShaderARB(f); print_shader_info_log(f); ///diag

    GLhandleARB p = glCreateProgramObjectARB();

    glAttachObjectARB(p,v);
    glAttachObjectARB(p,f);

    glLinkProgramARB(p);
    print_program_info_log(p); // print diagonostic information
    *prog = p;
}

bool shader_compiler_error(GLhandleARB shader, const char* name)
{
    if (shader == 0)
    {
        printf("shader compiler error: shader creation failed [%s]\n", name);
        return true;
    }

    GLint status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);  // this function does not assign any value to &status if a shader was not compiled
    glGetError();   // clear error
    if (status == GL_FALSE)
    {
        printf("shader compiler error: Shader compilation failed! [%s]\n", name);
        return true;
    }
    return false;
}

bool shader_linking_error(GLhandleARB shader, const char* name)
{
    if (shader <= 0)
    {
        printf("shader linking error: shader creation failed [%s]\n", name);
        return true;
    }
    GLint status = GL_FALSE;
    glGetProgramiv(shader, GL_LINK_STATUS, &status);
    glGetError();   // clear error
    if (status == GL_FALSE)
    {
        printf("shader linking error: Shader linking failed! [%s]\n", name);
        return true;
    }
    return false;
}
