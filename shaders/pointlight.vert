#version 330

uniform mat4 m_pvm;
uniform mat4 m_viewModel;
uniform mat3 m_normal;
uniform vec4 l_dir;	   // camera space

in vec4 position;	// local space
in vec4 normal;		// local space
in vec4 texCoord;

out Data {
	vec3 normal;
	vec3 eye;
	vec4 pos;
	vec2 tex_coord;
} DataOut;

void main () {
	vec4 pos = m_viewModel * position;
	DataOut.pos = pos;
	
	DataOut.normal = normalize(m_normal * normal.xyz);
	DataOut.eye = normalize(vec3(-pos));
	DataOut.tex_coord = texCoord.st;

	gl_Position = m_pvm * position;	
}