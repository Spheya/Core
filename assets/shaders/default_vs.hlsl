struct Attributes
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct Varyings
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

cbuffer CameraBuffer : register(b0)
{
    matrix matrix_v;
    matrix matrix_p;
}

Varyings main(Attributes attribs)
{
    Varyings varyings;
    varyings.pos = mul(mul(float4(attribs.pos, 1.0), matrix_v), matrix_p);
    varyings.uv = attribs.uv;
    return varyings;
}