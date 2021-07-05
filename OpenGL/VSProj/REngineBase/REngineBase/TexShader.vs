#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vcolor;
layout (location = 2) in vec2 uv;

out vec4 vertexColor; // 为片段着色器指定一个颜色输出
out vec2 TexUV; //指定一个UV坐标输出

uniform mat4 transform;//transform矩阵，从物体空间 ---> 世界空间
uniform mat4 view;//观察矩阵 摄像机 (RT)-1
uniform mat4 perspective;//透视投影矩阵

void main()
{
gl_Position = perspective * view * transform * vec4(aPos, 1.0f); //列向量右乘, 最终输出顶点位于齐次裁剪空间，进入(硬件)裁剪阶段

vertexColor = vec4(vcolor.rgb, 1.0); //输出颜色
TexUV = uv;//输出UV
}