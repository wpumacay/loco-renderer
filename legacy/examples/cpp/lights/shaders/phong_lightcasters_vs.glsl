#version 330 core

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;
layout ( location = 2 ) in vec2 texCoord;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewProjMatrix;
uniform mat4 u_normalMatrix;

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexcoord;

void main()
{
    gl_Position = u_viewProjMatrix * u_modelMatrix * vec4( position, 1.0f );

    fPosition = vec3( u_modelMatrix * vec4( position, 1.0f ) );
    fNormal = vec3( u_normalMatrix * vec4( normal, 0.0f ) );
    fTexcoord = texCoord;
}
