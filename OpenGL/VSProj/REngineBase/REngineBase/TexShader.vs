#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vcolor;
layout (location = 2) in vec2 uv;

out vec4 vertexColor; // ΪƬ����ɫ��ָ��һ����ɫ���
out vec2 TexUV; //ָ��һ��UV�������

uniform mat4 transform;//transform���󣬴�����ռ� ---> ����ռ�
uniform mat4 view;//�۲���� ����� (RT)-1
uniform mat4 perspective;//͸��ͶӰ����

void main()
{
gl_Position = perspective * view * transform * vec4(aPos, 1.0f); //�������ҳ�, �����������λ����βü��ռ䣬����(Ӳ��)�ü��׶�

vertexColor = vec4(vcolor.rgb, 1.0); //�����ɫ
TexUV = uv;//���UV
}