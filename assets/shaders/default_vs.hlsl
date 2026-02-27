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

Varyings main(Attributes attribs)
{
    Varyings varyings;
    varyings.pos = float4(attribs.pos, 1.0);
    varyings.uv = attribs.uv;
    return varyings;
}