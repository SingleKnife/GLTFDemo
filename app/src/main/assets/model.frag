precision mediump float;
uniform vec3 uLightColor;
uniform vec3 uLightPos;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
varying vec3 posNormal;
varying vec3 fragPos;


void main() {
    vec3 objColor = vec3(1.0, 1.0, 0);
    vec3 lightDir = normalize(vec3(uViewMatrix * vec4(uLightPos - fragPos, 0.0)));
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * uLightColor;
    float diff = max(dot(posNormal, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;
    //计算镜面反射
    float specularStrength = 0.5;
    vec3 reflectDir = reflect(-lightDir, posNormal);
    vec3 viewDir = normalize(-fragPos);//眼位置在0
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * uLightColor;

    vec3 lightResult = ambient + diffuse + specular;
    gl_FragColor = vec4(lightResult * objColor, 1.0);
}
