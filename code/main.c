#include "webgl.h"


extern void js_print(char *data, i32 count);
extern void js_print_number(int n);
extern unsigned char __heap_base;
extern unsigned char __data_end;


typedef struct {
  char *data;
  i32 count;
} String;



void *memcpy(void *dst, const void *src, unsigned long count) {
  for (i32 i = 0; i < count; i++) {
    ((byte *)dst)[i] = ((byte *)src)[i];
  }
  return dst;
}


void print(String str) {
  js_print(str.data, str.count);
}


String make_string(char *data, i32 count) {
  String result;
  result.data = data;
  result.count = count;
  return result;
}
#define const_string(str) make_string((char *)(str), sizeof(str)-1)

i32 c_string_length(char *str) {
  i32 result = 0;
  while (*str++) result++;
  return result;
}

String c_string_to_string(char *str) {
  String result = make_string(str, c_string_length(str));
  return result;
}

#define assert(cond, message) _assert(cond, (char *)message, (char *)__FILE__, (char *)__FUNCTION__, __LINE__);
void _assert(b32 cond, char *message, char *file, char *function, i32 line) {
  if (!cond) {
    print(const_string("Assert failed!"));
    print(c_string_to_string(message));
    print(c_string_to_string(file));
    print(c_string_to_string(function));
    js_print_number(line);
  }
}


#include "math.h"





typedef struct {
  byte *data;
  u32 size;
  u32 capacity;
} Arena;

void arena_init(Arena *arena, void *data, u32 capacity) {
  arena->data = data;
  arena->size = 0;
  arena->capacity = capacity;
}

byte *arena_push_size(Arena *arena, u32 size) {
  assert(arena->capacity >= arena->size + size, "Arena memory limit reached");
  byte *result = arena->data + arena->size;
  arena->size += size;
  return result;
}


i32 shader;
u32 img_width, img_height;
u32 *pixels;
b32 img_loaded = false;

#define CALLBACK(name) void name(void *data)
typedef CALLBACK(Callback);

extern void js_load_image(char *name, u32 *pixels, u32 *width, u32 *height);
extern void js_http_request(char *endpoint, Callback cb);


typedef enum {
  Http_Request_State_NONE,
  Http_Request_State_SIZE,
  Http_Request_State_DATA,
} Http_Request_State;

typedef struct {
  Http_Request_State state;
  void *data;
  u32 size;
  Arena *arena;
} Http_Request_Data;


void handle_http_request(Http_Request_Data *data) {
  switch (data->state) {
    case Http_Request_State_SIZE: {
      data->data = arena_push_size(data->arena, data->size);
    } break;
    case Http_Request_State_DATA: {
      
    } break;
  }
}


int main() {
  char *heap_base = (char *)&__heap_base;
  
  Arena _main_arena;
  Arena *main_arena = &_main_arena;
  arena_init(main_arena, heap_base, 1024*1024*64);
  
  char image_src[] = "./kitten.jpg";
  
  pixels = (u32 *)(main_arena->data + main_arena->size);
  
  js_load_image(image_src, pixels, &img_width, &img_height);
  //arena_push_size(main_arena, sizeof(u32)*width*height);
  
  
  char vert[] = "#version 300 es\n uniform mat4x4 u_model; layout (location = 0) in vec4 a_position; layout (location = 1) in vec2 a_tex_coord; out vec2 v_tex_coord; void main() { gl_Position = u_model*a_position; v_tex_coord = a_tex_coord; }";
  char frag[] = "#version 300 es\n precision mediump float; in vec2 v_tex_coord; out vec4 FragColor; uniform sampler2D tex_image; void main() { FragColor = texture(tex_image, v_tex_coord); }";
  
  shader = gl_create_program(vert, sizeof(vert)-1, frag, sizeof(frag)-1);
  
  
  i32 vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(f32)*4, 0);
  glEnableVertexAttribArray(0);
  
  glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(f32)*4, sizeof(f32)*2);
  glEnableVertexAttribArray(1);
  
  f32 vertices[] = {
    -1, -1,    0, 0,
    -1, 1,     0, 1,
    1, 1,      1, 1,
    
    1, 1,      1, 1,
    -1, -1,    0, 0,
    1, -1,     1, 0,
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glUseProgram(shader);
  
  js_http_request("./test.txt", handle_text_request);
  
  return 0;
}

#define PIXELS_PER_METER 128

void update(f32 width, f32 height, f32 dt) {
  if (img_width) {
    if (!img_loaded) {
      img_loaded = true;
      i32 texture;
      glGenTextures(1, &texture);
      js_print_number(texture);
      
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    }
    
    
    static f32 angle = 0;
    i32 u_model = glGetUniformLocation(shader, "u_model");
    
    v2 screen_size = v2_div_s(V2(width, height), PIXELS_PER_METER);
    
    mat4x4 model = mat4x4_identity();
    model = mat4x4_scale(model, V3(1, 1.5f, 1));
    model = mat4x4_rotate(model, angle);
    model = mat4x4_scale(model, v3_invert(v2_to_v3(screen_size, 1.0f)));
    angle += 0.01f;
    
    glUniformMatrix4fv(u_model, 1, false, (f32 *)&model);
    
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
}


