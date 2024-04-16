
//Isaac Lovy 
//   https://docs.unity3d.com/Packages/com.unity.shadergraph@6.9/manual/Remap-Node.html
float remapFloat(float inValue, float inMin, float inMax, float outMin, float outMax)
{
	return outMin + (inValue - inMin) * (outMax - outMin) / (inMax - inMin);
}

//Isaac Lovy
//   https://docs.unity3d.com/Packages/com.unity.shadergraph@6.9/manual/Remap-Node.html
float2 remapFloat2(float2 inValue, float inMin, float inMax, float outMin, float outMax)
{
	return outMin + (inValue - inMin) * (outMax - outMin) / (inMax - inMin);
}

//Isaac Lovy
//   https://docs.unity3d.com/Packages/com.unity.shadergraph@6.9/manual/Remap-Node.html
float3 remapFloat3(float3 inValue, float inMin, float inMax, float outMin, float outMax)
{
	return outMin + (inValue - inMin) * (outMax - outMin) / (inMax - inMin);
}
