struct Varyings
{
    float4 pos : SV_POSITION;
};

float4 main(Varyings varyings) : SV_TARGET
{
    return float4(0.2, 0.0, 0.2, 0.2);
}