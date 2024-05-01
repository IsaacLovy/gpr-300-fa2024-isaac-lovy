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
    vec3 vertPos = pos - camRight * vPos.x * scaleMult.x + camUp * vPos.z * scaleMult.y;
    return vec3(viewProjMat *  model * vec4(vertPos,1.0)).xyz;

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

//Yuuki
//X = CardNumber + lifetime? Some number for each card.
vec3 noiseGen(float x)
{
    vec3 temp = noise3(x);
    temp = remapFloat3(temp, -1, 1, 0, 1);

    return temp;
}

//Henry Foley
// https://docs.unity3d.com/Packages/com.unity.shadergraph@6.9/manual/Rotate-Node.html
vec2 rotator(vec2 UV, vec2 center, float rotation)
{
    UV -= center;
    float s = sin(rotation);
    float c = cos(rotation);
    mat2x2 rMatrix = mat2x2(c,-s,s,c);
    rMatrix *= 0.5;
    rMatrix += 0.5;
    rMatrix = rMatrix * 2.0 - 1.0;
    UV.xy = UV.xy * rMatrix;
    UV += center;
    return UV;
}