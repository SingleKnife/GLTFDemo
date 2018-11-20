attribute vec3 aPosition;
attribute vec3 aNormal;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
varying vec3 posNormal;
varying vec3 fragPos;
void main() {
    posNormal = normalize(vec3(uViewMatrix * uModelMatrix * vec4(aNormal, 0.0)));
    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1);
    fragPos = (uModelMatrix * vec4(aPosition, 1.0)).xyz;
}
