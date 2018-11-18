//
// Created by dong on 2018/11/6.
//

#include "gl_utils.h"
#include "log.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ModelRender.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace tinygltf;

const std::string TAG = "fyd";

void ModelRender::loadBinary(std::string fileName) {
    TinyGLTF gltf_ctx;
    std::string err;
    std::string warn;
    loadSuccess = gltf_ctx.LoadBinaryFromFile(&model, &err, &warn, fileName);
    if(!loadSuccess) {
        if(!err.empty()) {
            LOGE("loadBinary: %s" ,err.c_str());
        }
        if(!warn.empty()) {
            LOGE("loadBinary: %s", warn.c_str());
        }
    }
}

void ModelRender::setupBuffers() {
    if(!loadSuccess || model.bufferViews.size() == 0) {
        return;
    }
    for(size_t i = 0; i < model.bufferViews.size(); ++i) {
        const BufferView &bufferView = model.bufferViews[i];
        if(bufferView.target == 0) {
            LOGE("bufferView target is zero: %d", i);
            continue;
        }
        const Buffer &buffer = model.buffers[bufferView.buffer];
        GLuint bufferId;
        glGenBuffers(1, &bufferId);
        glBindBuffer(bufferView.target, bufferId);
        glBufferData(bufferView.target, bufferView.byteLength, &buffer.data[0] + bufferView.byteOffset, GL_STATIC_DRAW);
        glBuffers.push_back(bufferId);
        glBindBuffer(bufferView.target, 0);
    }
}

bool ModelRender::isLoadSuccess() {
    return loadSuccess;
}

void ModelRender::drawMesh(const Mesh &mesh) {
    beforeDrawMesh();

    for(int i = 0; i < mesh.primitives.size(); ++i) {
        const Primitive &primitive = mesh.primitives[i];
        for(auto it = primitive.attributes.cbegin(); it != primitive.attributes.cend(); ++it) {
            if(glAttributeLocations.at(it->first) == -1) {
                continue;
            }

            int accessorIndex = it->second;
            Accessor &positionAccessor = model.accessors[accessorIndex];
            glBindBuffer(GL_ARRAY_BUFFER, glBuffers[positionAccessor.bufferView]);

            int byteStride = positionAccessor.ByteStride(model.bufferViews[positionAccessor.bufferView]);

            int size = GetTypeSizeInBytes(positionAccessor.type);
            glVertexAttribPointer(glAttributeLocations[it->first], size, positionAccessor.componentType,
                                  positionAccessor.normalized ? GL_TRUE : GL_FALSE, byteStride,
                                  BUFFER_OFFSET(positionAccessor.byteOffset));
            glEnableVertexAttribArray((GLuint)glAttributeLocations[it->first]);
        }

        const Accessor &indexAccessor = model.accessors[primitive.indices];
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffers[indexAccessor.bufferView]);

        glDrawElements(primitive.mode, indexAccessor.count, indexAccessor.componentType,
                       BUFFER_OFFSET(indexAccessor.byteOffset));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        for(auto it = primitive.attributes.cbegin(); it != primitive.attributes.cend(); ++it) {
            if(glAttributeLocations.at(it->first) == -1) {
                continue;
            }
            int accessorIndex = it->second;
            Accessor &positionAccessor = model.accessors[accessorIndex];
            glDisableVertexAttribArray((GLuint)glAttributeLocations[it->first]);
        }

        checkGlError("draw");
    }
}

void ModelRender::init(const std::string fileName) {
    auto gVertexShader =
            "attribute vec3 aPosition;\n"
            "attribute vec3 aNormal;\n"
            "uniform mat4 uProjectionMatrix;\n"
            "uniform mat4 uViewMatrix;"
            "uniform mat4 uModelMatrix;"
            "varying vec3 posNormal;\n"
            "varying vec3 fragPos;"
            "void main() {\n"
            "  posNormal = aNormal;\n"
            "  gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1);\n"
            "  fragPos = aPosition;"
            "}\n";

    auto gFragmentShader =
            "precision mediump float;\n"
            "uniform vec3 uLightColor;\n"
            "uniform vec3 uLightPos;\n"
            "uniform mat4 uViewMatrix;"
            "varying vec3 posNormal;\n"
            "varying vec3 fragPos;"
            "void main() {\n"
            "  vec3 objColor = vec3(1.0, 1.0, 0);"
            "  vec3 lightDir = normalize(uLightPos - fragPos);"
            "  float ambientStrength = 0.3;"
            "  vec3 ambient = ambientStrength * uLightColor;"
            "  float diff = max(dot(posNormal, lightDir), 0.0);"
            "  vec3 diffuse = diff * uLightColor;"
            "  vec3 lightResult = ambient + diffuse;"
            "  gl_FragColor = vec4(lightResult * objColor, 1.0);\n"
            "}\n";
    loadBinary(fileName);
    if(!isLoadSuccess()) {
        return;
    }
    lightColor.fill(1.0f);
    lightPos.fill(0);
    lightPos[1] = 3.0f;
    lightPos[0] = 0.0f;

    setupBuffers();
    glProgram = createProgram(gVertexShader, gFragmentShader);
    projectionMatrixLocation = glGetUniformLocation(glProgram, "uProjectionMatrix");
    viewMatrixLocation = glGetUniformLocation(glProgram, "uViewMatrix");
    modelMatrixLocation = glGetUniformLocation(glProgram, "uModelMatrix");
    lightColorLocation = glGetUniformLocation(glProgram, "uLightColor");
    lightPosLocation = glGetUniformLocation(glProgram, "uLightPos");
    initAttributeLocations();
    glUseProgram(glProgram);
    checkGlError("init");
}

void ModelRender::drawModel() {
    if(!isLoadSuccess()) {
        return;
    }
    Mesh &mesh = model.meshes[0];
    drawMesh(mesh);
}

void ModelRender::drawFrame() {
    drawModel();
}

void ModelRender::onProjectionMatrixChanged(std::array<float, 16> projectionMatrix) {
    for(int i = 0; i < 16; ++i) {
        this->projectionMartix[i] = projectionMatrix[i];
    }
}

void ModelRender::onViewMatrixChanged(std::array<float, 16> viewMatrix) {
    for(int i = 0; i < 16; ++i) {
        this->viewMatrix[i] = viewMatrix[i];
    }
}

void ModelRender::onModelMatrixChanged(std::array<float, 16> modelMatrix) {
    for(int i = 0; i < 16; ++i) {
        this->modelMatrix[i] = modelMatrix[i];
    }
}

/**
 * 绘制前设置环境属性
 */
void ModelRender::beforeDrawMesh() {
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, projectionMartix.data());
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, viewMatrix.data());
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, modelMatrix.data());
    glUniform3fv(lightColorLocation, 1, lightColor.data());
    glUniform3fv(lightPosLocation, 1, lightPos.data());
}

void ModelRender::initAttributeLocations() {
    GLint aPositionLocation = glGetAttribLocation(glProgram, "aPosition");
    glAttributeLocations["POSITION"] = aPositionLocation;
    GLint aNormalLocation = glGetAttribLocation(glProgram,"aNormal");
    glAttributeLocations["NORMAL"] = aNormalLocation;

    glAttributeLocations["TEXCOORD_0"] = -1;
    glAttributeLocations["TEXCOORD_1"] = -1;
    glAttributeLocations["COLOR_0"] = -1;
    glAttributeLocations["JOINTS_0"] = -1;
    glAttributeLocations["WEIGHTS_0"] = -1;
}



