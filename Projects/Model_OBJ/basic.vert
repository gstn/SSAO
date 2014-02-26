varying vec3 N;
varying vec3 v;

attribute vec4 a_Position;

void main(void)
{

   v = vec3(gl_ModelViewMatrix * a_Position);       
   N = normalize(gl_NormalMatrix * gl_Normal);

   gl_Position = gl_ModelViewProjectionMatrix * a_Position;

}
          