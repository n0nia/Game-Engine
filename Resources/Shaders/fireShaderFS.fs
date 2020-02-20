precision mediump float;

varying vec3 v_color;
uniform sampler2D u_texture_0, u_texture_1, u_texture_2;
uniform float u_Time;
uniform float u_DispMax;

varying vec2 v_uv;

//fog
varying vec3 v_pos;
uniform float u_r, u_R;
uniform vec3 u_colorFog;
uniform vec3 u_cam;

void main()
{	
	vec2 disp = texture2D(u_texture_2, vec2(v_uv.x, v_uv.y + u_Time)).rg;
	
	vec2 v_uvDisplaced = v_uv + (2.0 * disp - 1.0) * u_DispMax;
	
	vec4 c_alpha = texture2D(u_texture_0, vec2(v_uv.x, v_uv.y));
	
	vec4 c_fire = texture2D(u_texture_1, v_uvDisplaced);
	

	vec4 c_final = c_fire;
	c_final.a = c_fire.a *  c_alpha.r;

	if(c_final.a < 0.1){
		discard;
	}

	float dist = distance(v_pos, u_cam);
	float alpha = clamp((dist - u_r)/(u_R - u_r), 0.0, 1.0);
	vec4 finalColor = alpha * vec4(u_colorFog, 1.0) + (1.0 - alpha) * c_final;

	gl_FragColor = finalColor;
}