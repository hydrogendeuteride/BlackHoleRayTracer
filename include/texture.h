#ifndef BLACKHOLERAYTRACER_TEXTURE_H
#define BLACKHOLERAYTRACER_TEXTURE_H

#include <string>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <future>

unsigned int loadTexture(const std::string& file);

unsigned int loadCubeMap(const std::vector<std::string>& faces);

unsigned int loadHDRCubeMap(const std::vector<std::string>& faces);

#endif //BLACKHOLERAYTRACER_TEXTURE_H
