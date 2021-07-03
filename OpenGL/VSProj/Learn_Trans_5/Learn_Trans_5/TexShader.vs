#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vcolor;
layout (location = 2) in vec2 uv;

out vec4 vertexColor; // 为片段着色器指定一个颜色输出
out vec2 TexUV; //指定一个UV坐标输出

uniform mat4 transform;

void main()
{
gl_Position = transform * vec4(aPos, 1.0f); //列向量右乘

vertexColor = vec4(vcolor.rgb, 1.0); //输出颜色
TexUV = uv;//输出UV
}