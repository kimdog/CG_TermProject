precision mediump float;

uniform sampler2D s_tex0;
uniform sampler2D s_texNor;


// same with HW2. but not using vertexNormal, using NormalMap(computed)

uniform vec3 materialSpec, materialAmbi, materialEmit;  // Md, Ms, Ma, Me
			  
uniform float materialSh;
uniform vec3 sourceDiff, sourceSpec, sourceAmbi;  // Sd, Ss, Sa


varying mat3 tangentToWorldSpace; // already computed in VertexShader

varying vec2 v_texCoord;
varying vec3 v_lightDir, v_viewDir;	// light and camera in WS

// not use below things which are in TS
// because we will compute Lighting equation in WS
// varying vec3 v_lightTS, v_viewTS;

struct Material {
	float sh; 					 // shininess (specular power)
	vec3 diff, spec, ambi, emit; // material colors
};

struct Light {
	vec3 dir, diff, spec, ambi;  // light direction and colors
};

vec3 phongLight(vec3 view, vec3 normal, // view direction and normal
				Material M, Light S) {  // material and source light
	float diff = max(dot(normal, S.dir), 0.0);
	vec3 refl = 2.0 * normal * dot(normal, S.dir) - S.dir; // reflection vector
	float spec = 0.0;
	if(diff > 0.0) spec = pow(max(dot(refl, view), 0.0), M.sh);

	vec3 sum = vec3(0.0);
	sum += diff * S.diff * M.diff;  	// add diffuse term
	sum += spec * S.spec * M.spec;  	// add specular term
	sum += S.ambi * M.ambi + M.emit; 	// add ambient and emissive term

	return sum;
}


void main() {
   // you should fill in this function
   
   vec3 materialDiff = texture2D(s_tex0,v_texCoord).xyz;
   
   // make normalMap. range [-1, 1]
   // normal in normalMap are not parallel with z-axis in TS
   vec3 norTS = normalize(texture2D(s_texNor,v_texCoord).xyz*2.0 - vec3(1.0));
   
   // move normalMap on TangentSpace to WorldSpace 
   vec3 norWS = tangentToWorldSpace * norTS;
   
   Material material =
		Material(materialSh, 
				 materialDiff, materialSpec, materialAmbi, materialEmit);
   
   // use light in WS
   Light source =
		Light(normalize(v_lightDir), sourceDiff, sourceSpec, sourceAmbi);

   // use view(=camera) in WS and normalMap in WS
   vec3 color = 
    	phongLight(normalize(v_viewDir), normalize(norWS), material, source);

   gl_FragColor = vec4(color, 1.0);
	
}
