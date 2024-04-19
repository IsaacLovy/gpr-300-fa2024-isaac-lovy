
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

/*
vec3 billboarding(vec3 obj, float objScale, vec3 camPos, mat4 invView)
{
	vec3 pos;
	float scale;
	vec4 billboard;

	scale *= objScale;
	obj * objScale;

	invView *= obj;

	obj+= invView;

	return billboard;
}
*/
	uvec3 v = round(p);