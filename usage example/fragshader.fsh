struct MaterialProperty {

    vec3 diffuseColor;
    vec3 ambienceColor;
    vec3 specularColor;

    float shines;
};

uniform sampler2D u_diffuseMap;
uniform sampler2D u_normalMap;

uniform highp vec4 u_lightPosition;
uniform highp float u_lightPower;

varying highp vec4 v_position;
varying highp vec2 v_textcoord;
varying highp vec3 v_normal;

uniform bool u_usingNormalMap;
uniform bool u_usingDiffuseMap;

uniform highp MaterialProperty u_MaterialProperty;


void main(void)
{
    vec4 resultColor = vec4(0.0, 0.0, 0.0, 0.0); // результирующий цвет чёрный

    vec4 eyePosition = vec4(0.0, 0.0, 0.0, 1.0); // позиция наблюдателя

    vec4 diffMatColor = (u_usingDiffuseMap) ? texture2D(u_diffuseMap, v_textcoord) : vec4(u_MaterialProperty.diffuseColor, 1.0); // диффузный цвет

    vec3 usingNormal = v_normal;

    vec3 eyeVec = normalize(v_position.xyz - eyePosition.xyz); // направление взгляда

    vec3 lightVec = normalize(v_position.xyz - u_lightPosition.xyz); // вектор освещения

    vec3 reflectLight = normalize(reflect(lightVec, v_normal)); // отражённый свет

    float len = length(v_position.xyz - eyePosition.xyz); // расстояние от наблюдателя до точки

    float specularFactor = u_MaterialProperty.shines; // размер пятна блика

    float ambientFactor = 0.1; // светимость материала (emission)

    vec4 reflectionColor = vec4(1.0, 1.0, 1.0, 1.0); //цвет блика

    vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(v_normal, -lightVec));
    resultColor += diffColor;

    resultColor *= vec4(u_MaterialProperty.ambienceColor, 1.0);
    vec4 ambientColor = ambientFactor * diffMatColor;
    resultColor += ambientColor;
    resultColor *= vec4(u_MaterialProperty.ambienceColor, 1.0);


    vec4 specularColor = reflectionColor * u_lightPower * pow(max(0.0, dot(reflectLight, -eyeVec)), specularFactor);
    resultColor += specularColor;

    if (u_usingNormalMap)
    {
        resultColor *= vec4(u_MaterialProperty.specularColor, 1.0);

        usingNormal = normalize(texture2D(u_normalMap, v_textcoord).rgb * 2.0 - 1.0);

        vec3 LightVect = normalize(v_position.xyz - u_lightPosition.xyz); //вектор света. направлен из точки,

        diffColor = diffMatColor * u_lightPower * max(0.0, dot(usingNormal, -LightVect));
        reflectLight = normalize(reflect(LightVect, usingNormal));
        vec4 specularColor_n = /* цвет блика: */ vec4(1.0, 1.0, 1.0, 1.0) * u_lightPower *
                    pow(max(0.0, dot(reflectLight, -eyeVec)), specularFactor) /*/ (1.0 + 0.025 * len * len)*/;
        resultColor += specularColor_n;
    }

    gl_FragColor = resultColor;
}
