#ifndef __SHADERCONSTANTS_H
#define __SHADERCONSTANTS_H

const char* basicVert = 
{
    "#version 330\n"\

    "uniform mat4 viewMatrix, projectionMatrix, modelMatrix;\n"\
 
    "layout(location=0) in vec4 in_Position;\n"\
    "layout(location=1) in vec4 in_Normal;\n"\
    "layout(location=2) in vec2 in_TexCoord0;\n"\
    "out vec4 ex_Normal;\n"\
    "out vec2 ex_TexCoord0;\n"\
    "out float ex_diffuse;\n"\
 
    "void main(void)\n"\
    "{\n"\
    "   vec4 vlightPos = vec4(-30, 30, 30, 1);\n"\
    "   ex_diffuse = max(dot(in_Normal, vlightPos), 0.0);\n"\

    "   gl_Position = projectionMatrix*viewMatrix*modelMatrix*in_Position;\n"\
    "   ex_Normal = in_Normal;\n"\
    "   ex_TexCoord0 = in_TexCoord0;\n"\
    "}\n"
};

const GLchar* basicFrag = 
{
    "#version 330\n"\
 
    "in vec4 ex_Normal;\n"\
    "in vec2 ex_TexCoord0;\n"\
    "out vec4 out_Color;\n"\
    "in float ex_diffuse;\n"\

    "uniform sampler2D canis_texture0;\n"\
 
    "void main(void)\n"\
    "{\n"\
    "   vec4 lightPos = vec4(-30, 30, -30, 1);\n"\
    "   float diffuse = max(dot(ex_Normal, lightPos), 0.0);\n"\
    "   out_Color = vec4(0.1,0.1,0.1,1.0)+(texture(canis_texture0, ex_TexCoord0)*(diffuse*0.025));\n"\
    "   //out_Color = texture(canis_texture0, ex_TexCoord0);\n"\
    "}\n"\
};

#endif
