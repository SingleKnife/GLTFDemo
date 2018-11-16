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
    for(int i = 0; i < mesh.primitives.size(); ++i) {
        const Primitive &primitive = mesh.primitives[i];
        int positionAccessorIndex = primitive.attributes.at("POSITION");
        Accessor &positionAccessor = model.accessors[positionAccessorIndex];
        glBindBuffer(GL_ARRAY_BUFFER, glBuffers[positionAccessor.bufferView]);

        int byteStride = positionAccessor.ByteStride(model.bufferViews[positionAccessor.bufferView]);

        int size = 1;
        if (positionAccessor.type == TINYGLTF_TYPE_SCALAR) {
            size = 1;
        } else if (positionAccessor.type == TINYGLTF_TYPE_VEC2) {
            size = 2;
        } else if (positionAccessor.type == TINYGLTF_TYPE_VEC3) {
            size = 3;
        } else if (positionAccessor.type == TINYGLTF_TYPE_VEC4) {
            size = 4;
        } else {
            assert(0);
        }
        glVertexAttribPointer(glShaderVariable["POSITION"], size, positionAccessor.componentType,
                              positionAccessor.normalized ? GL_TRUE : GL_FALSE, byteStride,
                              BUFFER_OFFSET(positionAccessor.byteOffset));
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, projectionMartix.data());
        glEnableVertexAttribArray((GLuint)glShaderVariable["POSITION"]);

        const Accessor &indexAccessor = model.accessors[primitive.indices];
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffers[indexAccessor.bufferView]);

        GLenum mode = 0;
        if (primitive.mode == TINYGLTF_MODE_TRIANGLES) {
            mode = GL_TRIANGLES;
        } else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_STRIP) {
            mode = GL_TRIANGLE_STRIP;
        } else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_FAN) {
            mode = GL_TRIANGLE_FAN;
        } else if (primitive.mode == TINYGLTF_MODE_POINTS) {
            mode = GL_POINTS;
        } else if (primitive.mode == TINYGLTF_MODE_LINE) {
            mode = GL_LINES;
        } else if (primitive.mode == TINYGLTF_MODE_LINE_LOOP) {
            mode = GL_LINE_LOOP;
        } else {
            assert(0);
        }

        glDrawElements(mode, indexAccessor.count, indexAccessor.componentType,
                       BUFFER_OFFSET(indexAccessor.byteOffset));
        glDisableVertexAttribArray(glShaderVariable["POSITION"]);
        checkGlError("draw");
    }
}

void ModelRender::init(const std::string fileName) {
    auto gVertexShader =
            "attribute vec3 aPosition;\n"
            "uniform mat4 uProjectionMatrix;\n"
            "varying vec3 pos;"
            "void main() {\n"
            "  pos = aPosition;"
            "  gl_Position = uProjectionMatrix * vec4(aPosition, 1);\n"
            "}\n";

    auto gFragmentShader =
            "precision mediump float;\n"
            "varying vec3 pos;"
            "void main() {\n"
            "  gl_FragColor = vec4(pos.x, pos.y, pos.z, 1.0);\n"
            "}\n";
    loadBinary(fileName);
    if(!isLoadSuccess()) {
        return;
    }
    setupBuffers();
    glProgram = createProgram(gVertexShader, gFragmentShader);
    GLint aPositionLocation = glGetAttribLocation(glProgram, "aPosition");
    projectionMatrixLocation = glGetUniformLocation(glProgram, "uProjectionMatrix");
    glShaderVariable["POSITION"] = aPositionLocation;
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

void ModelRender::onProjectionChanged(std::array<float, 16> projectionMatrix) {
    for(int i = 0; i < 16; ++i) {
        this->projectionMartix[i] = projectionMatrix[i];
    }
}

