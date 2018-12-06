//
// Created by dong on 2018/11/25.
//

#ifndef GLTFDEMO_SHADER_H
#define GLTFDEMO_SHADER_H

#include <string>
std::string vert_shader =

        "attribute vec3 aPosition;\n"
        "attribute vec3 aNormal;\n"
        "uniform mat4 uProjectionMatrix;\n"
        "uniform mat4 uViewMatrix;"
        "uniform mat4 uModelMatrix;"
        "varying vec3 viewPosNormal;\n"
        "varying vec3 viewFragPos;"
        "void main() {\n"
        "  viewPosNormal = normalize(vec3(uViewMatrix * uModelMatrix * vec4(aNormal, 0.0)));\n"
        "  gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1);\n"
        "  viewFragPos = (uViewMatrix * uModelMatrix * vec4(aPosition, 1.0)).xyz;"
        "}\n";


std::string frag_shader = "precision mediump float;\n"
                          "    uniform vec3 uLightColor;\n"
                          "    uniform vec3 uLightPos;\n"
                          "    uniform mat4 uViewMatrix;\n"
                          "    uniform mat4 uModelMatrix;\n"
                          "    varying vec3 viewPosNormal;\n"
                          "    varying vec3 viewFragPos;\n"
                          "\n"
                          "\n"
                          "    void main() {\n"
                          "        vec3 viewLightPos = (uViewMatrix * vec4(uLightPos, 1.0)).xyz;"
                          "        vec3 objColor = vec3(1.0, 1.0, 0);\n"
                          "        vec3 viewLightDir = normalize(vec3(vec4(viewLightPos - viewFragPos, 0.0)));\n"
                          "        float ambientStrength = 0.3;\n"
                          "        vec3 ambient = ambientStrength * uLightColor;\n"
                          "        float diff = max(dot(viewPosNormal, viewLightDir), 0.0);\n"
                          "        vec3 diffuse = diff * uLightColor;\n"
                          "        //计算镜面反射\n"
                          "        float specularStrength = 0.5;\n"
                          "        vec3 reflectDir = reflect(-viewLightDir, viewPosNormal);\n"
                          "        vec3 viewEyeDir = normalize(-viewFragPos);//眼位置在0\n"
                          "        float spec = pow(max(dot(viewEyeDir, reflectDir), 0.0), 32.0);\n"
                          "        vec3 specular = specularStrength * spec * uLightColor;\n"
                          "\n"
                          "        vec3 lightResult = ambient + specular;\n"
                          "        gl_FragColor = vec4(lightResult * objColor, 1.0);\n"
                          "    }";

#endif //GLTFDEMO_SHADER_H
