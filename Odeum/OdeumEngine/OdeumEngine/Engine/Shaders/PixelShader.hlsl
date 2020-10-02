cbuffer LightData : register(b1)
{
	float3 lightPos;
	float radiusSq;
	float3 lightColour;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 worldPos : WorldPos;
	float2 uv : TexCoord0;
	float3 viewDir : TexCoord1;
	float3 normal : Normal;
	float3 tangent : Tangent;
	float3 bitangent : Bitangent;
};

void FresnelLighting(inout float3 specular, inout float3 diffuse, float3 lightDir, float3 halfVec)
{
    float fresnel = pow(1.0 - saturate(dot(lightDir, halfVec)), 5.0);
    specular = lerp(specular, 1, fresnel);
    diffuse = lerp(diffuse, 0, fresnel);
}

float3 ApplyLightCommon(
    float3    diffuseColour,    // Diffuse albedo
    float3    specularColour,    // Specular albedo
    float     gloss,            // Specular power
    float3    normal,            // World-space normal
    float3    viewDir,        // World-space vector from eye to point
    float3    lightDir,        // World-space vector from point to light
    float3    lightColour        // Radiance of directional light
)
{
    float3 halfVec = normalize(lightDir - viewDir);
    float nDotH = saturate(dot(halfVec, normal));

    FresnelLighting(diffuseColour, specularColour, lightDir, halfVec);

    float specularFactor = pow(nDotH, gloss) * (gloss + 2) / 8;

    float nDotL = saturate(dot(normal, lightDir));

    return nDotL * lightColour * (diffuseColour + specularFactor * specularColour);
}

float3 ApplyPointLight(
    float3    diffuseColor,     // Diffuse albedo
    float3    specularColor,    // Specular albedo
    float     gloss,            // Specular power
    float3    normal,           // World-space normal
    float3    viewDir,          // World-space vector from eye to point
    float3    objWorldPos,      // World-space fragment position
    float3    lightPos,         // World-space light position
    float     lightRadiusSq,
    float3    lightColor        // Radiance of directional light
)
{
    float3 lightDir = lightPos - objWorldPos;
    float lightDistSq = dot(lightDir, lightDir);
    float invLightDist = rsqrt(lightDistSq);
    lightDir *= invLightDist;

    float distanceFalloff = lightRadiusSq * (invLightDist * invLightDist);
    distanceFalloff = max(0, distanceFalloff - rsqrt(distanceFalloff));

    return distanceFalloff * ApplyLightCommon(
        diffuseColor,
        specularColor,
        gloss,
        normal,
        viewDir,
        lightDir,
        lightColor
    );
}

float4 main(PS_INPUT input) : SV_Target0
{
    float3 diffuseColour = float3(0.9f, 0.2f, 0.5f);
    float3 specularColour = float3(1.0f, 0.0f, 0.4f);
    float gloss = 128.0f;

    float3 colour = ApplyPointLight(diffuseColour, specularColour, gloss, input.normal, 
        input.viewDir, input.worldPos, lightPos, radiusSq, lightColour);
	return float4(colour, 1.0f);
}