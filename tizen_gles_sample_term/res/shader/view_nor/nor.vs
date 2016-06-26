
uniform mat4 worldMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform vec3 eyePos;
uniform vec3 lightPos;

attribute vec3 position;
attribute vec3 normal;				// z-axis of tangentSpace
attribute vec2 texCoord;
attribute vec3 tangent;				// vector on tangentFace

varying vec2 v_texCoord;
varying vec3 v_lightDir, v_viewDir; // light and camera
varying mat3 tangentToWorldSpace;	// TangentSpace to WorldSpace matrix
									// used in Fragment Shader 

//for compute on World Space, we do not need move light and camera on TangentSpace
//varying vec3 v_lightTS, v_viewTS;

void main() {
   // you should fill in this function
   // gl_Position = projMat * viewMat * worldMat * vec4(position, 1.0);
   vec4 posWS = worldMat * vec4(position, 1.0);
   gl_Position = projMat * viewMat * posWS;		// view object
   v_texCoord = texCoord;
   
   
   vec3 nor = mat3(worldMat) * normal;	// move surfaceNormal(=z-axis of TS) 
   										// on WorldSpace 	
   vec3 tan = mat3(worldMat) * normalize(tangent);

   // normal and tangent compute binormal by crossProduct
   vec3 bin = cross(nor,tan);
   
   //mat3 worldToTangentSpace = mat3(tan.x,bin.x,nor.x,
   //								tan.y,bin.y,nor.y,
   //									tan.z,bin.z,nor.z);
   
   //matrix which move object on TS to WS
   // in openGL it means Inverse of (T T T )
   //					 			(B B B )
   //					 			(N B B ) 
   tangentToWorldSpace = mat3(tan.x,tan.y,tan.z,
   								bin.x,bin.y,bin.z,
   								nor.x,nor.y,nor.z);
   
   // light and camera on the WorldSpace
   v_lightDir = normalize(lightPos - posWS.xyz);
   v_viewDir = normalize(eyePos - posWS.xyz);
   
   //v_lightTS = worldToTangentSpace(lightPos - posWS.xyz);
   //v_viewTS = worldToTangentSpace(eyePos - posWS.xyz);
}

