#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexUV;

uniform sampler2D ourTexture; //声明纹理
uniform sampler2D ourTexture2; //声明纹理

void main() 
{

//FragColor = texture(ourTexture, TexUV) * vertexColor; //采样纹理颜色 * 顶点颜色

FragColor = mix(texture(ourTexture, TexUV), texture(ourTexture2, TexUV), 0.2); 
//mix是一个经典线性差值方法，mix(a,b,c%); 返回 a-->b 线性过渡到c%位置的颜色 （a取1-c%,b取c%）

}
