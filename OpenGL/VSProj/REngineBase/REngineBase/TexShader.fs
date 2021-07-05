#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexUV;

uniform sampler2D ourTexture; //��������
uniform sampler2D ourTexture2; //��������

void main() 
{

//FragColor = texture(ourTexture, TexUV) * vertexColor; //����������ɫ * ������ɫ

FragColor = mix(texture(ourTexture, TexUV), texture(ourTexture2, TexUV), 0.2); 
//mix��һ���������Բ�ֵ������mix(a,b,c%); ���� a-->b ���Թ��ɵ�c%λ�õ���ɫ ��aȡ1-c%,bȡc%��

}
