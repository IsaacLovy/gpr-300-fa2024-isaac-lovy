
//Isaac Lovy 
//   https://docs.unity3d.com/Packages/com.unity.shadergraph@6.9/manual/Remap-Node.html
float remapFloat(float inValue, float inMin, float inMax, float outMin, float outMax)
{
	return outMin + (inValue - inMin) * (outMax - outMin) / (inMax - inMin);
}

//Isaac Lovy
//   https://docs.unity3d.com/Packages/com.unity.shadergraph@6.9/manual/Remap-Node.html
vec2 remapFloat2(vec2 inValue, float inMin, float inMax, float outMin, float outMax)
{
	return outMin + (inValue - inMin) * (outMax - outMin) / (inMax - inMin);
}

//Isaac Lovy
//   https://docs.unity3d.com/Packages/com.unity.shadergraph@6.9/manual/Remap-Node.html
vec3 remapFloat3(vec3 inValue, float inMin, float inMax, float outMin, float outMax)
{
	return outMin + (inValue - inMin) * (outMax - outMin) / (inMax - inMin);
}

//Yuuki Endo
vec3 billboarding(vec3 pos, vec3 vPos,  vec2 scaleMult, mat4 viewMat, mat4 viewProjMat, mat4 model)
{
    vec3 camRight = vec3 (viewMat[0][0],viewMat[1][0],viewMat[2][0]);
    vec3 camUp =  vec3 (viewMat[0][1],viewMat[1][1],viewMat[2][1]);
    vec3 vertPos = vec3(0.0, 1.0, 0.0) - camRight * vPos.x * scaleMult.x + camUp * vPos.z * scaleMult.y;
    return viewProjMat *  model * vec4(vertPos,1.0);

}

//	uvec3 v = round(p);

//Henry Foley
// https://docs.unity3d.com/Packages/com.unity.shadergraph@6.9/manual/Flipbook-Node.html
vec2 flipbook(vec2 UV, float width, float height, float tile)
{
    tile = mod(tile, width * height);
    vec2 tileCount = vec2(1.0) / vec2(width,height);
    float tileX = abs(height - floor(tile * tileCount.x));
    float tileY = abs(height - floor(tile * tileCount.y));
    return (UV + vec2(tileX, tileY)) * tileCount;
}

vec3 cellNoise(int x)
{
    vec3 temp = noise3(x);
    temp.x = (temp.x - 0.0f)/(1.0f-0.0f);
    temp.y = (temp.y - 0.0f)/(1.0f-0.0f);
    temp.z = (temp.z - 0.0f)/(1.0f-0.0f);

    return temp;
}