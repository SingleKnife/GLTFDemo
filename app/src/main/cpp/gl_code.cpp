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

void onProjectionChanged(std::array<float, 16> projectionMatrix) {
    if(!isInit) {
        return;
    }
    modelRender->onProjectionChanged(projectionMatrix);
}

extern "C" {
    JNIEXPORT void JNICALL Java_com_example_dong_testcpp_GL2JNILib_onProjectionChanged(JNIEnv * env, jclass type, jfloatArray projectionMatrix);
    JNIEXPORT void JNICALL Java_com_example_dong_testcpp_GL2JNILib_onDrawFrame(JNIEnv * env, jclass type);
    JNIEXPORT void JNICALL Java_com_example_dong_testcpp_GL2JNILib_init(JNIEnv *env, jclass type, jstring fileName);
};

JNIEXPORT void JNICALL Java_com_example_dong_testcpp_GL2JNILib_onProjectionChanged(JNIEnv * env, jclass type, jfloatArray jProjectionMatrix) {
    if(!isInit) {
        return;
    }

    std::array<float, 16> projectionMatrix;
    jfloat *jArrayData = env->GetFloatArrayElements(jProjectionMatrix, 0);
    int arrayLength = env->GetArrayLength(jProjectionMatrix);

    for(int i = 0; i < arrayLength; ++i) {
        projectionMatrix[i] = jArrayData[i];
    }

    onProjectionChanged(projectionMatrix);

    env->ReleaseFloatArrayElements(jProjectionMatrix, jArrayData, 0);
}

JNIEXPORT void JNICALL Java_com_example_dong_testcpp_GL2JNILib_onDrawFrame(JNIEnv * env, jclass type) {
    renderFrame();
}

JNIEXPORT void JNICALL Java_com_example_dong_testcpp_GL2JNILib_init(JNIEnv *env, jclass type, jstring jFileName) {
    const char *fileName = env->GetStringUTFChars(jFileName, 0);
    init(fileName);
    env->ReleaseStringUTFChars(jFileName, fileName);
}