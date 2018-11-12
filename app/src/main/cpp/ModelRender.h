//
// Created by dong on 2018/11/6.
//

#ifndef TESTCPP_MODELLOADER_H
#define TESTCPP_MODELLOADER_H

#include <string>
#include <vector>
#include <map>
#include <gl_utils.h>
#include <GLES2/gl2.h>
#include "tiny_gltf.h"

using namespace tinygltf;
using namespace std;

class ModelRender {
    string fileName;
    Model model;
    bool loadSuccess;
    vector<GLuint> glBuffers;
private:
    GLuint glProgram;
    array<float, 16> projectionMartix;
    array<float, 16> modelMatrix;
    map<string, GLint> glShaderVariable;

public:
    ModelRender() {

    }

private:
    void drawMesh(const Mesh &mesh);
    bool isLoadSuccess();
    void loadBinary(string fileName);
    void setupBuffers();

public:
    void init();
    void drawFrame();
    void onProjectionChanged(array<float, 16> projectionMatrix);
};


#endif //TESTCPP_MODELLOADER_H
