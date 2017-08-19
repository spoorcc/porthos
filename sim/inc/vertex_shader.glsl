// Vertex Shade - file "minimal.vert"

#version 130

in  vec3 in_Position;
in  vec3 in_Color;

out vec3 ex_Color;

uniform mat4 model = mat4(1.0);

void main(void)
{
    gl_Position = model * vec4(in_Position, 1.0);
    ex_Color = in_Color;
}
