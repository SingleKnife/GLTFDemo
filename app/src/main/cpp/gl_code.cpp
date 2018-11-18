/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code

#include <jni.h>
#include "ModelRender.h"

ModelRender *modelRender;
bool isInit;


void renderFrame() {
    if(!isInit) {
        return;
    }
    modelRender->drawFrame();
}

void init(std::string fileName) {
    if(modelRender == nullptr) {
        modelRender = new ModelRender();
        isInit = true;
    }
    modelRender->init(fileName);
}

void onProjectionMatrixChanged(std::array<float, 16> projectionMatrix) {
    if(!isInit) {
        return;
    }
    modelRender->onProjectionMatrixChanged(projectionMatrix);
}

void onViewMatrixChanged(std::array<float, 16> viewMatrix) {
    if(!isInit) {
        return;
    }
    modelRender->onViewMatrixChanged(viewMatrix);
}

void onModelMatrixChanged(std::array<float, 16> modelMatrix) {
    if(!isInit) {
        return;
    }
    modelRender->onModelMatrixChanged(modelMatrix);
}

extern "C" {
    JNIEXPORT void JNICALL Java_com_example_dong_testcpp_GL2JNILib_onProjectionMatrixChanged(JNIEnv * env, jclass type, jfloatArray projectionMatrix);
    JNIEXPORT void JNICALL Java_com_example_dong_testcpp_GL2JNILib_onViewMatrixChanged(JNIEnv * env, jclass type, jfloatArray viewMatrix);
    JNIEXPORT void JNICALL Java_com_example_dong_testcpp_GL2JNILib_onModelMatrixChanged(JNIEnv * env, jclass type, jfloatArray jModelMatrix);
    JNIEXPORT void JNICALL Java_com_example_dong_testcpp_GL2JNILib_onDrawFrame(JNIEnv * env, jclass type);
    JNIEXPORT void JNICALL Java_com_example_dong_testcpp_GL2JNILib_init(JNIEnv *env, jclass type, jstring fileName);
};

JNIEXPORT void JNICALL Java_com_example_dong_testcpp_GL2JNILib_onProjectionMatrixChanged(JNIEnv * env, jclass type, jfloatArray jProjectionMatrix) {
    if(!isInit) {
        return;
    }

    std::array<float, 16> projectionMatrix;
    jfloat *jArrayData = env->GetFloatArrayElements(jProjectionMatrix, 0);
    int arrayLength = env->GetArrayLength(jProjectionMatrix);

    for(int i = 0; i < arrayLength; ++i) {
        projectionMatrix[i] = jArrayData[i];
    }

    onProjectionMatrixChanged(projectionMatrix);

    env->ReleaseFloatArrayElements(jProjectionMatrix, jArrayData, 0);
}

JNIEXPORT void JNICALL Java_com_example_dong_testcpp_GL2JNILib_onViewMatrixChanged(JNIEnv * env, jclass type, jfloatArray jViewMatrix) {
    if(!isInit) {
        return;
    }

    std::array<float, 16> viewMatrix;
    jfloat *jArrayData = env->GetFloatArrayElements(jViewMatrix, 0);
    int arrayLength = env->GetArrayLength(jViewMatrix);

    for(int i = 0; i < arrayLength; ++i) {
        viewMatrix[i] = jArrayData[i];
    }

    onViewMatrixChanged(viewMatrix);

    env->ReleaseFloatArrayElements(jViewMatrix, jArrayData, 0);

}

JNIEXPORT void JNICALL Java_com_example_dong_testcpp_GL2JNILib_onModelMatrixChanged(JNIEnv * env, jclass type, jfloatArray jModelMatrix) {
    if(!isInit) {
        return;
    }

    std::array<float, 16> modelMatrix;
    jfloat *jArrayData = env->GetFloatArrayElements(jModelMatrix, 0);
    int arrayLength = env->GetArrayLength(jModelMatrix);

    for(int i = 0; i < arrayLength; ++i) {
        modelMatrix[i] = jArrayData[i];
    }

    onModelMatrixChanged(modelMatrix);

    env->ReleaseFloatArrayElements(jModelMatrix, jArrayData, 0);

}

JNIEXPORT void JNICALL Java_com_example_dong_testcpp_GL2JNILib_onDrawFrame(JNIEnv * env, jclass type) {
    renderFrame();
}

JNIEXPORT void JNICALL Java_com_example_dong_testcpp_GL2JNILib_init(JNIEnv *env, jclass type, jstring jFileName) {
    const char *fileName = env->GetStringUTFChars(jFileName, 0);
    init(fileName);
    env->ReleaseStringUTFChars(jFileName, fileName);
}