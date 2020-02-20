precision highp float;

varying vec3 v_color;
uniform sampler2D u_texture_0, u_texture_1;

varying vec3 v_norm, v_binorm, v_tgt;

//fog
varying vec3 v_pos;
uniform vec3 u_colorFog;
//raza zonei de claritate, la cat se termina zona de tranzitie
uniform float u_r, u_R;

//light
uniform vec3 u_positionLight;
uniform vec3 u_ambiental;
uniform vec3 u_specularColor;
uniform vec3 u_diffuseColor;

uniform float u_specularCoef;
uniform float u_specularPower;
uniform float u_diffuseCoef;
uniform float u_shininess;

uniform vec3 u_cam;
uniform vec3 u_ratio;

uniform float u_attenuation;
uniform float u_type;

varying vec2 v_uv;

void main()
{
	vec4 color = texture2D(u_texture_0, v_uv);
	if(color.a < 0.7){
		discard;
	}
	vec3 c_normalMap = texture2D(u_texture_1, v_uv).rgb;

	vec3 normal2	= normalize(2.0 * v_norm -1.0);
	vec3 binorm2     = normalize(v_binorm);
	vec3 v_tgt2      = normalize(v_tgt);

	mat3 TBN = mat3(vec3(v_tgt2.x, v_binorm2.x, normal2.x),
					vec3(v_tgt2.y, v_binorm2.y, normal2.y),
					vec3(v_tgt2.z, v_binorm2.z, normal2.z));
	
	vec3 normal3 = normalize(c_normalMap * TBN + v_norm);

	

	if(color.a == 0.0){
		discard;
	}

	vec3 vecLightDirectional	= normalize(-u_positionLight);
	vec3 vecLightPoint			= normalize(v_pos - u_positionLight);
	vec3 vecLight				= u_type * (vecLightPoint) + (1.0 - u_type) * vecLightDirectional;
	
	
	vec3 toeye		= normalize(u_cam - v_pos);
	vec3 reflec		= normalize(reflect(vecLight, normal3));
	//vec3 reflectCam = reflect(toeye , normalize(normal3));

	float distPos	= normalize(distance(v_pos, u_positionLight));


	float attenuation = 1.0 / (1.0 + (u_attenuation * distPos));

	vec3 ambiental = color.xyz * u_ambiental;
	vec3 Cdiff = color.xyz * u_diffuseColor * max(dot(normal3, -vecLight), 0.0) * u_diffuseCoef;
	vec3 Cspec = u_specularColor * pow(max(dot(reflec,toeye),0.0), u_specularPower);


	vec3 newColor = ambiental + attenuation * (Cdiff + Cspec);

	float dist = distance(v_pos, u_cam);
	float alpha = clamp((dist - u_r)/(u_R - u_r),0.0,1.0);
	vec4 c_Final = alpha * vec4(u_colorFog,1.0) + (1.0 - alpha) * vec4(newColor, 1.0);

	
	gl_FragColor = c_Final;

}