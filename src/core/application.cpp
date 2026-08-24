#include "../../headers/core/application.hpp"
#include "../../headers/scene/camera.hpp"

#include "../../headers/imgui/imgui.h"
#include "../../headers/imgui/imgui_impl_glfw.h"
#include "../../headers/imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../headers/core/stb_image.h"
#include "../../headers/core/stb_image_write.h"

#include <iostream>

GLFWwindow* openGlInit() {

  if (!glfwInit())
    std::cout << "Failed to initialize GLFW" << std::endl;

  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

  GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Solar System Renderer", NULL, NULL);
  glfwSetWindowPos(window, 0, 0);
  glfwMakeContextCurrent(window); 

  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK)
    std::cout << "Failed to initialize GLEW" << std::endl;

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, Camera::mouseCallback);

  return window;
}

GLuint textureFromFile(const char* path, bool isColorTex) {
  unsigned int textureId;
  glGenTextures(1, &textureId);

  int width, height, channels;
  unsigned char* image = stbi_load(path, &width, &height, &channels, 0);
  std::cout << "The image on path: " << path << " -> Height: " << height << "; Width -> " << width << std::endl;

  if(image) {
    GLenum format = GL_RGB;

    switch(channels) {
      case 1:
        format = GL_RED;
        break;
      case 3:
        format = GL_RGB;
        break;
      case 4:
        format = GL_RGBA;
        break;
    }

    GLenum innerFormat = isColorTex
    ? (channels == 4 ? GL_SRGB8_ALPHA8 : GL_SRGB8)
    : (channels == 4 ? GL_RGBA8 : (channels == 1 ? GL_R8 : GL_RGB8));

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, innerFormat, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(image);
  } else {
    std::cout << "loading image failed for path: " << path << "\n" << stbi_failure_reason() << std::endl;
    unsigned char whitePixel[3] = {255, 255, 255};
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, whitePixel);
  }

  return textureId;
}

void takeScreenShot(GLFWwindow* window, const char* path) {
  int w, h;
  glfwGetFramebufferSize(window, &w, &h);
  std::vector<unsigned char> pixels(w * h * 3);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
  stbi_flip_vertically_on_write(true);
  if(!stbi_write_png(path, w, h, 3, pixels.data(), w * 3))
    std::cout << "Failed to write with stbi_write_png" << std::endl;
}

void processInput(GLFWwindow* window, std::unique_ptr<Camera>& camera) {
  static bool tabPrev = false;
  static bool f12Prev = false;

  bool tabPressed = glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS;
  if(tabPressed && !tabPrev) {
    camera->cursorEnabled = !camera->cursorEnabled;
    glfwSetInputMode(window, GLFW_CURSOR, 
        camera->cursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    ImGuiIO& io = ImGui::GetIO();
    if(!camera->cursorEnabled) {
      camera->_firstMouse = true;
      io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
    } else {
      io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    }
  }
  tabPrev = tabPressed;

  bool f12Pressed = glfwGetKey(window, GLFW_KEY_F12) == GLFW_PRESS;

  if(f12Pressed && !f12Prev) {
    std::cout << "Pressed f12" << std::endl;
    takeScreenShot(window, "screenshot.png");
  }
  f12Prev = f12Pressed;

  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    glfwSetWindowShouldClose(window, GL_TRUE);

}

void setupDearImGui(GLFWwindow* window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();
}

GLuint loadCubemap(std::vector<std::string>& paths) {

  stbi_set_flip_vertically_on_load(false);

  unsigned int texId;
  glGenTextures(1, &texId);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

  int width, height, nChannels;

  for(unsigned int i = 0; i < paths.size(); i++) {

    unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &nChannels, 0);

    if(data) {
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      GLenum format = (nChannels == 4) ? GL_RGBA : GL_RGB;
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB8, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
      std::cout << "Successfully loaded texture at path: " << paths[i] << std::endl;
    } else {
      std::cout << "Failed to load cubemap texture at path " << paths[i] << std::endl;
      stbi_image_free(data);
    }
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return texId;
}

void setupDebuggingMessages() {
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    (void)source;
    (void)type;
    (void)id;
    (void)length;
    (void)userParam;

    if(severity == GL_DEBUG_SEVERITY_HIGH) {
        std::cerr << "GL ERROR: " << message << std::endl;
        std::abort();
    }
  }, nullptr);
}
