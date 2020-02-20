attribute vec3 a_posL;
varying vec3 v_coord;

uniform mat4 u_MVP;

void main() {

	v_coord = a_posL;
	vec4 posL = u_MVP * vec4(a_posL, 1.0);
	gl_Position = posL;
}
   