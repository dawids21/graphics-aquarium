#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 kd;
uniform vec4 ka;
uniform vec4 ks;
uniform float alpha;
uniform float ambient;

//Attributes
in vec4 vertex; //Vertex coordinates in model space
in vec4 normal; //Vertex normal in model space

//Varying variables
out vec4 l1;
out vec4 l2;
out vec4 n;
out vec4 v;
out vec4 i_kd;
out vec4 i_ka;
out vec4 i_ks;
out float i_alpha;
out float i_ambient;

void main(void) {
    vec4 lp1 = vec4(-14, 5, 0, 1); //light position, world space
    vec4 lp2 = vec4(14, 5, 0, 1); //light position, world space
    l1 = normalize(V * lp1 - V * M * vertex); //vector towards the light in eye space
    l2 = normalize(V * lp2 - V * M * vertex); //vector towards the light in eye space
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); //vector towards the viewer in eye space
    n = normalize(V * M * normal); //normal vector in eye space

    i_kd = kd;
    i_ka = ka;
    i_ks = ks;
    i_alpha = alpha;
    i_ambient = ambient;

    gl_Position = P * V * M * vertex;
}
