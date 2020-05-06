#pragma once

#include "ez/Color.h"
#include <filesystem>

namespace ez
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