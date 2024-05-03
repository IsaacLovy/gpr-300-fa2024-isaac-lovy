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

//Henry Foley
// https://docs.unity3d.com/Packages/com.unity.shadergraph@6.9/manual/Flipbook-Node.html
vec2 flipbook(vec2 UV, float width, float height, float tile, vec2 invert)
{
    tile = mod(tile, width * height);
    vec2 tileCount = vec2(1.0) / vec2(width,height);
    float tileX = abs(invert.x * width - ((tile - width * floor(tile * tileCount.x)) + invert.x * 1));
    float tileY = abs(invert.y * height - (floor(tile * tileCount.x) + invert.y * 1));
    return (UV + vec2(tileX, tileY)) * tileCount;
}

//Yuuki
//X = CardNumber + lifetime? Some number for each card.

vec3 noiseGen(vec3 x)
{
    float phi = 1.61803398874989484820459;
    
    float rand1 = fract(tan(distance(vec2(x.z,x.y)*phi, vec2(x.z,x.y))*x.x)*vec2(x.z,x.y).x);
    float rand2 = fract(tan(distance(vec2(x.x,x.z)*phi, vec2(x.x,x.z))*x.y)*vec2(x.x,x.z).x);
    float rand3 = fract(tan(distance(vec2(x.x,x.y)*phi, vec2(x.x,x.y))*x.z)*vec2(x.x,x.y).x);

    vec3 temp = vec3(rand1,rand2,rand3);
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


//Yuuki Endo & Isaac Lovy
vec3 billboarding(vec3 pos, vec3 vPos,  vec2 scaleMult, mat4 viewMat, float rotation)
{
    vec3 camRight = vec3 (viewMat[0][0],viewMat[1][0],viewMat[2][0]);
    vec3 camUp =  vec3 (viewMat[0][1],viewMat[1][1],viewMat[2][1]);

    vec2 rotPos = rotator(vPos.xy, vec2(0.5, 0.5), rotation);

    vec3 vertPos = pos + camRight * rotPos.x * scaleMult.x + camUp * rotPos.y * scaleMult.y;
    return vertPos;
}
