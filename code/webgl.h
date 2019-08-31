#include "lvl5_types.h"

#define GL_ARRAY_BUFFER 34962
#define GL_FLOAT 5126
#define GL_STATIC_DRAW 35044
#define GL_TRIANGLES 4
#define GL_COLOR_BUFFER_BIT 16384
#define GL_TEXTURE_2D 3553
#define GL_TEXTURE_WRAP_S 10242
#define GL_TEXTURE_WRAP_T 10243
#define GL_TEXTURE_MIN_FILTER 10241
#define GL_CLAMP_TO_EDGE 33071
#define GL_LINEAR 9729
#define GL_RGBA 6408
#define GL_UNSIGNED_BYTE 5121


typedef i32 GLint;
typedef u32 GLsizei;
typedef u32 GLenum;
typedef void GLvoid;
typedef f32 GLfloat;
typedef b32 GLboolean;
typedef u32 GLuint;

extern i32 gl_create_program(char *vert, i32 vert_size, char *frag, i32 frag_size);
extern void glClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
extern void glClear(GLenum buffer);
extern void glGenTextures(GLint count, GLint *texture);
extern void glBindTexture(GLenum target, GLint texture);
extern void glTexParameteri(GLenum target, GLenum param, GLenum value);
extern void glTexImage2D(GLenum target,
                         GLint level,
                         GLint internalformat,
                         GLsizei width,
                         GLsizei height,
                         GLint border,
                         GLenum format,
                         GLenum type,
                         const GLvoid * data);

extern void glGenBuffers(GLint count, GLint *buffer);
extern void glBindBuffer(GLenum target, GLint buffer);
extern void glVertexAttribPointer(GLint index, GLint size, GLenum type, GLboolean normalized, GLint stride, GLuint offset);
extern void glEnableVertexAttribArray(GLint index);
extern void glBufferData(GLenum target, GLint size, GLvoid *data, GLenum usage);
extern void glUseProgram(GLint program);
extern void glDrawArrays(GLenum target, GLint offset, GLint count);
extern i32 glGetUniformLocation(GLint program, char *name);
extern void glUniformMatrix4fv(GLint location, GLint count, GLboolean transpose, GLfloat *value);
