attribute vec3 a_posL, a_normL, a_binormL, a_tgtL;
attribute vec2 a_uv;
varying vec2 v_uv;

attribute vec3 a_color;
varying vec3 v_color;
uniform mat4 u_MVP;

varying vec3 v_pos;
varying vec3 v_norm;
varying vec3 v_binorm;
varying vec3 v_tgt;


uniform mat4 u_Model, u_View, u_Projection;


void main() {
	vec4 position = vec4(a_posL, 1.0);
	v_color = a_color;
	v_uv = a_uv;


	v_pos		= (u_Model * position).xyz;
	v_norm		= (normalize(u_Model * vec4(a_normL, 1.0)).xyz);
	v_binorm	= (normalize(u_Model * vec4(a_binormL, 1.0)).xyz);
	v_tgt		= (normalize(u_Model * vec4(a_tgtL, 1.0)).xyz);

	vec4 posL	= u_MVP * position;

	gl_Position = posL;

}

  