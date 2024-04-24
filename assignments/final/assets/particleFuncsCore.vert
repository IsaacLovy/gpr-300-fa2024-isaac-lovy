
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
vec3 billboarding(vec3 pos, float scaleMult, vec3 objScale, mat4 invView, mat4 model)
{
    pos = ((objScale * scaleMult) * pos);
    vec4 holder;
    holder = (vec4(pos.x,pos.y,pos.z,0.0f) * invView);
    pos = holder + pos;
    
    pos *= inverse(model);
    return pos;

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