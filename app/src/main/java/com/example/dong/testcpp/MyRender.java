package com.example.dong.testcpp;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.os.Environment;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MyRender implements GLSurfaceView.Renderer {
    private Context context;
    private float[] projectionMatrix = new float[6];

    MyRender(Context context) {
        this.context = context;
    }

    public void onDrawFrame(GL10 gl) {
        GL2JNILib.onDrawFrame();
    }

    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES20.glViewport(0, 0, width, height);
        Matrix.perspectiveM(projectionMatrix, 0, 45, (float)width / (float)height, 1, 10);
        GL2JNILib.onProjectionChanged(projectionMatrix);
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        // Do nothing.
        String gltfFile = Environment.getExternalStorageDirectory().getAbsolutePath() + "/box01.glb";
        GL2JNILib.init(gltfFile);
    }
}
