#pragma once

#include "Color.h"
#include <filesystem>

namespace egl
{
template <typename T>
class Image2D;

class ImageIO final
{
public:
  static void Read(const std::filesystem::path& inPath, Image2D<Color4f>& outImage);
  static void Write(const Image2D<Color4f> &inImage, const std::filesystem::path& inPath);

  ImageIO() = delete;
};
}