struct Attributes
{
    float3 pos : POSITION;
};

struct Varyings
{
    float4 pos : SV_POSITION;
};

Varyings main(Attributes attribs)
{
    Varyings varyings;
    varyings.pos = float4(attribs.pos, 1.0);
    return varyings;
}