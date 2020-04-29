#include "ezgl/ImageIO.h"
#include "ezgl/Image2D.h"
#include "ezgl/Macros.h"
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "ezgl/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ezgl/stb_image_write.h"

namespace egl
{
void ImageIO::Read(const std::filesystem::path& inPath, Image2D<Color4f>& outImage)
{
  if (!std::filesystem::exists(inPath) || std::filesystem::is_directory(inPath))
    THROW_EXCEPTION("Path '" << inPath << "' does not exist.");

  int read_width = 0;
  int read_height = 0;
  int non_forced_read_num_channels = 0;

  stbi_set_flip_vertically_on_load(true);
  const uint8_t* read_data = stbi_load(inPath.c_str(), &read_width, &read_height, &non_forced_read_num_channels, STBI_rgb_alpha);

  constexpr auto ForcedNumChannels = 4;
  outImage.Create(read_width, read_height);
  for (int y = 0; y < read_height; ++y)
  {
    for (int x = 0; x < read_width; ++x)
    {
      const auto r = (read_data[(x + y * read_width) * ForcedNumChannels + 0] / 255.0f);
      const auto g = (read_data[(x + y * read_width) * ForcedNumChannels + 1] / 255.0f);
      const auto b = (read_data[(x + y * read_width) * ForcedNumChannels + 2] / 255.0f);
      const auto a = (read_data[(x + y * read_width) * ForcedNumChannels + 3] / 255.0f);
      const auto color = Color4f(r, g, b, a);
      outImage.Get(x, y) = color;
    }
  }

  stbi_image_free((void*)(read_data));
}

void ImageIO::Write(const Image2D<Color4f>& inImage, const std::filesystem::path& inPath)
{
  const auto extension = inPath.extension().string();
  std::string lower_case_extension;
  std::transform(extension.cbegin(), extension.cend(), std::back_inserter(lower_case_extension), ::tolower);

  const auto width = inImage.GetWidth();
  const auto height = inImage.GetHeight();

  Image2D<Color4<uint8_t>> image_uint;
  image_uint.Create(width, height);
  for (std::size_t y = 0; y < height; ++y)
  {
    for (std::size_t x = 0; x < width; ++x)
    {
      const auto r = static_cast<uint8_t>(inImage.Get(x, y)[0] * 255.0f);
      const auto g = static_cast<uint8_t>(inImage.Get(x, y)[1] * 255.0f);
      const auto b = static_cast<uint8_t>(inImage.Get(x, y)[2] * 255.0f);
      const auto a = static_cast<uint8_t>(inImage.Get(x, y)[3] * 255.0f);
      image_uint.Get(x, y) = Color4<uint8_t>(r, g, b, a);
    }
  }

  const auto write_data = static_cast<const void*>(image_uint.GetData().GetData());
  if (lower_case_extension == ".png")
  {
    const auto row_stride = 4 * sizeof(uint8_t) * width;
    stbi_write_png(inPath.c_str(), width, height, 4, write_data, row_stride);
  }
  else if (lower_case_extension == ".bmp")
  {
    stbi_write_bmp(inPath.c_str(), width, height, 4, write_data);
  }
  else if (lower_case_extension == ".tga")
  {
    stbi_write_tga(inPath.c_str(), width, height, 4, write_data);
  }
  else if (lower_case_extension == ".jpg")
  {
    stbi_write_jpg(inPath.c_str(), width, height, 4, write_data, 100);
  }
  else
  {
    THROW_EXCEPTION("Write image extension '" << extension << "' not supported.");
  }
}

}