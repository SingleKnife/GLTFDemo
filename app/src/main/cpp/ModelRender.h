//
// Created by dong on 2018/11/6.
//

#ifndef TESTCPP_MODELLOADER_H
#define TESTCPP_MODELLOADER_H

#include <string>
#include <vector>
#include <map>
#include <GLES2/gl2.h>
#include "gl_utils.h"

#include "tiny_gltf.h"

class ModelRender {
    std::string fileName;
    tinygltf::Model model;
    bool loadSuccess;
    std::vector<GLuint> glBuffers;
private:
    GLuint glProgram;
    std::array<float, 16> projectionMartix;
    std::array<float, 16> modelMatrix;
    std::map<std::string, GLint> glShaderVariable;

public:
    ModelRender() {

    }

private:
    void drawModel();
    void drawMesh(const tinygltf::Mesh &mesh);
    bool isLoadSuccess();
    void loadBinary(std::string fileName);
    void setupBuffers();

public:
    void init(const std::string fileName);
    void drawFrame();
    void onProjectionChanged(std::array<float, 16> projectionMatrix);
};


#endif //TESTCPP_MODELLOADER_H
