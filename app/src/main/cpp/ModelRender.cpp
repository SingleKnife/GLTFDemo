//
// Created by dong on 2018/11/6.
//

#include "ModelRender.h"
#include "gl_utils.h"
#include "log.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

const string TAG = "fyd";

void ModelRender::loadBinary(string fileName) {
    TinyGLTF gltf_ctx;
    string err;
    string warn;
    loadSuccess = gltf_ctx.LoadBinaryFromFile(&model, &err, &warn, fileName);
    if(!loadSuccess) {
        if(!err.empty()) {
            LogE(TAG, "loadBinary: " + err);
        }
        if(!warn.empty()) {
            LogE(TAG, "loadBinary: " + warn);
        }
    }
}

void ModelRender::setupBuffers() {
    if(!loadSuccess || model.bufferViews.size() < 0) {
        return;
    }
    for(size_t i = 0; i < model.bufferViews.size(); ++i) {
        const BufferView &bufferView = model.bufferViews[i];
        if(bufferView.target == 0) {
            LogE(TAG, "bufferView target is zero: " + i);
            continue;
        }
        const Buffer &buffer = model.buffers[bufferView.buffer];
        GLuint bufferId;
        glGenBuffers(1, &bufferId);
        glBindBuffer(bufferView.target, bufferId);
        glBufferData(bufferView.target, bufferView.byteLength, &buffer.data[0], GL_STATIC_DRAW);
        glBuffers.push_back(bufferId);
    }
}

bool ModelRender::isLoadSuccess() {
    return loadSuccess;
}

void ModelRender::drawMesh(const Mesh &mesh) {
    for(int i = 0; i < mesh.primitives.size(); ++i) {
        Primitive &primitive = mesh.primitives[i];
        int positionAccessorIndex = primitive.attributes["POSITION"];
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
    }
}

void ModelRender::init() {
    auto gVertexShader =
            "attribute vec4 aPosition;\n"
            "void main() {\n"
            "  gl_Position = aPosition;\n"
            "}\n";

    auto gFragmentShader =
            "precision mediump float;\n"
            "void main() {\n"
            "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
            "}\n";
    glProgram = createProgram(gVertexShader, gFragmentShader);
    GLint aPositionLocation = glGetAttribLocation(glProgram, "aPosition");
    glShaderVariable["POSITION"] = aPositionLocation;
}

void ModelRender::drawFrame() {
}

void onProjectionChanged(array<float, 16> projectionMatrix) {

}

