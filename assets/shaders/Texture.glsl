#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in int a_EntityID;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;
out float v_TilingFactor;
out flat int v_EntityID;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    v_TilingFactor = a_TilingFactor;
    v_EntityID = a_EntityID;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 460 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_Entity;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;
in flat int v_EntityID;

uniform sampler2D u_Textures[32];

void main()
{
    vec4 TexColor = v_Color;

	switch(int(v_TexIndex))
	{
		case  0: TexColor *= texture(u_Textures[ 0], v_TexCoord * v_TilingFactor); break;
		case  1: TexColor *= texture(u_Textures[ 1], v_TexCoord * v_TilingFactor); break;
		case  2: TexColor *= texture(u_Textures[ 2], v_TexCoord * v_TilingFactor); break;
		case  3: TexColor *= texture(u_Textures[ 3], v_TexCoord * v_TilingFactor); break;
		case  4: TexColor *= texture(u_Textures[ 4], v_TexCoord * v_TilingFactor); break;
		case  5: TexColor *= texture(u_Textures[ 5], v_TexCoord * v_TilingFactor); break;
		case  6: TexColor *= texture(u_Textures[ 6], v_TexCoord * v_TilingFactor); break;
		case  7: TexColor *= texture(u_Textures[ 7], v_TexCoord * v_TilingFactor); break;
		case  8: TexColor *= texture(u_Textures[ 8], v_TexCoord * v_TilingFactor); break;
		case  9: TexColor *= texture(u_Textures[ 9], v_TexCoord * v_TilingFactor); break;
		case 10: TexColor *= texture(u_Textures[10], v_TexCoord * v_TilingFactor); break;
		case 11: TexColor *= texture(u_Textures[11], v_TexCoord * v_TilingFactor); break;
		case 12: TexColor *= texture(u_Textures[12], v_TexCoord * v_TilingFactor); break;
		case 13: TexColor *= texture(u_Textures[13], v_TexCoord * v_TilingFactor); break;
		case 14: TexColor *= texture(u_Textures[14], v_TexCoord * v_TilingFactor); break;
		case 15: TexColor *= texture(u_Textures[15], v_TexCoord * v_TilingFactor); break;
		case 16: TexColor *= texture(u_Textures[16], v_TexCoord * v_TilingFactor); break;
		case 17: TexColor *= texture(u_Textures[17], v_TexCoord * v_TilingFactor); break;
		case 18: TexColor *= texture(u_Textures[18], v_TexCoord * v_TilingFactor); break;
		case 19: TexColor *= texture(u_Textures[19], v_TexCoord * v_TilingFactor); break;
		case 20: TexColor *= texture(u_Textures[20], v_TexCoord * v_TilingFactor); break;
		case 21: TexColor *= texture(u_Textures[21], v_TexCoord * v_TilingFactor); break;
		case 22: TexColor *= texture(u_Textures[22], v_TexCoord * v_TilingFactor); break;
		case 23: TexColor *= texture(u_Textures[23], v_TexCoord * v_TilingFactor); break;
		case 24: TexColor *= texture(u_Textures[24], v_TexCoord * v_TilingFactor); break;
		case 25: TexColor *= texture(u_Textures[25], v_TexCoord * v_TilingFactor); break;
		case 26: TexColor *= texture(u_Textures[26], v_TexCoord * v_TilingFactor); break;
		case 27: TexColor *= texture(u_Textures[27], v_TexCoord * v_TilingFactor); break;
		case 28: TexColor *= texture(u_Textures[28], v_TexCoord * v_TilingFactor); break;
		case 29: TexColor *= texture(u_Textures[29], v_TexCoord * v_TilingFactor); break;
		case 30: TexColor *= texture(u_Textures[30], v_TexCoord * v_TilingFactor); break;
		case 31: TexColor *= texture(u_Textures[31], v_TexCoord * v_TilingFactor); break;
	}

	if (TexColor.a == 0.0)
		discard;

    o_Color = TexColor;
    o_Entity = v_EntityID;
}
