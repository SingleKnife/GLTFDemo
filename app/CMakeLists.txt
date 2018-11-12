# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)
aux_source_directory(src/main/cpp SRC_DIR)

add_library( # Sets the name of the library.
        model

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${SRC_DIR}
        )


target_link_libraries( # Specifies the target library.
        model
        android
        GLESv2
        EGL
        log
        )