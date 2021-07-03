#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vcolor;
layout (location = 2) in vec2 uv;

out vec4 vertexColor; // ΪƬ����ɫ��ָ��һ����ɫ���
out vec2 TexUV; //ָ��һ��UV�������

uniform mat4 transform;

void main()
{
gl_Position = transform * vec4(aPos, 1.0f); //�������ҳ�

vertexColor = vec4(vcolor.rgb, 1.0); //�����ɫ
TexUV = uv;//���UV
}