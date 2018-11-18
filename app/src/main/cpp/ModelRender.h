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
    std::array<float, 16> viewMatrix;
    std::array<float, 3> lightColor;
    std::array<float, 3> lightPos;

    std::map<std::string, GLint> glAttributeLocations;
    GLint projectionMatrixLocation;
    GLint viewMatrixLocation;
    GLint modelMatrixLocation;
    GLint lightColorLocation;
    GLint lightPosLocation;       //方向光，与原点的方向

public:
    ModelRender() {

    }

private:
    void drawModel();
    void drawMesh(const tinygltf::Mesh &mesh);
    void beforeDrawMesh();
    bool isLoadSuccess();
    void loadBinary(std::string fileName);
    void setupBuffers();
    void initAttributeLocations();

public:
    void init(const std::string fileName);
    void drawFrame();
    void onProjectionMatrixChanged(std::array<float, 16> projectionMatrix);
    void onViewMatrixChanged(std::array<float, 16> viewMatrix);
    void onModelMatrixChanged(std::array<float, 16> modelMatrix);
};


#endif //TESTCPP_MODELLOADER_H
