#include <ez/Font.h>
#include <ez/Vec.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include "ez/Image2D.h"
#include <ez/MathSwizzling.h>
#include <ez/Mesh.h>
#include "ez/stb_truetype.h"
#include <fstream>
#include <numeric>
#include <vector>

namespace ez
{
Font::Font(const std::filesystem::path& inFontPath, const uint64_t inFontSize)
{
  // Read font into font buffer
  {
    std::ifstream ifs(inFontPath.c_str(), std::ios::binary);
    if (!ifs)
      THROW_EXCEPTION("Can not open font file " << inFontPath);

    const auto font_file_bytes_size = std::filesystem::file_size(inFontPath);
    mFontBuffer.resize(font_file_bytes_size);
    ifs.read(reinterpret_cast<char*>(mFontBuffer.data()), font_file_bytes_size);
  }

  // Init font
  if (!stbtt_InitFont(&mFontInfo, mFontBuffer.data(), 0))
    THROW_EXCEPTION("Can not init font read from file " << inFontPath);
  mFontSize = inFontSize;
  mFontScale = stbtt_ScaleForPixelHeight(&mFontInfo, mFontSize);

  const auto atlas_characters = std::string {
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789=;,:.`() []{}\"-_?!+*/\\%&$#@|^<>'"
  };

  // Create single-byte image
  const auto atlas_image_size = ComputeAtlasCharactersPlacement(atlas_characters);
  auto atlas_image_byte = Image2D<uint8_t>(atlas_image_size, atlas_image_size);
  for (uint64_t i = 0; i < atlas_characters.size(); ++i)
  {
    const auto& atlas_character = atlas_characters[i];
    const auto& atlas_character_placement = mCharactersAtlasRects[atlas_character];

    const auto atlas_character_min = atlas_character_placement.GetMin();
    const auto byteOffset = atlas_character_min[0] + (atlas_character_min[1] * atlas_image_byte.GetWidth());
    stbtt_MakeCodepointBitmap(&mFontInfo,
        atlas_image_byte.GetData().GetData() + byteOffset,
        atlas_character_placement.GetSize()[0],
        atlas_character_placement.GetSize()[1],
        atlas_image_byte.GetWidth(),
        mFontScale,
        mFontScale,
        atlas_character);
  }

  // Create atlas texture
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  mAtlasTexture = std::make_unique<Texture2D>(atlas_image_byte);
  mAtlasTexture->SetMinFilterMode(GL::EMinFilterMode::LINEAR_MIPMAP_LINEAR);
  mAtlasTexture->GenerateMipMap();
}

Vec2ul Font::GetCharacterSize(const char inCharacter) const { return Vec2ul(GetCharacterRect(inCharacter).GetSize()); }

AARecti Font::GetCharacterRect(const char inCharacter) const
{
  int char_min_x, char_min_y, char_max_x, char_max_y;
  stbtt_GetCodepointBitmapBox(&mFontInfo,
      inCharacter,
      mFontScale,
      mFontScale,
      &char_min_x,
      &char_min_y,
      &char_max_x,
      &char_max_y);
  const auto character_rect_min = Vec2i { char_min_x, char_min_y };
  const auto character_rect_max = Vec2i { char_max_x, char_max_y };
  return { character_rect_min, character_rect_max };
}

AARectul Font::GetCharacterAtlasRect(const char inCharacter) const
{
  EXPECTS(static_cast<uint64_t>(inCharacter) < mCharactersAtlasRects.size());
  return mCharactersAtlasRects[inCharacter];
}

AARectf Font::GetCharacterAtlasTextureCoordinatesRect(const char inCharacter) const
{
  const auto character_atlas_rect = GetCharacterAtlasRect(inCharacter);

  const auto character_texture_coordinates_rect_min = Vec2f(character_atlas_rect.GetMin()) / mAtlasTexture->GetWidth();
  const auto character_texture_coordinates_rect_max = Vec2f(character_atlas_rect.GetMax()) / mAtlasTexture->GetHeight();

  EXPECTS(IsBetween(character_texture_coordinates_rect_min, Zero<Vec2f>(), One<Vec2f>()));
  EXPECTS(IsBetween(character_texture_coordinates_rect_max, Zero<Vec2f>(), One<Vec2f>()));

  const auto atlas_character_texture_coordinate
      = AARectf { character_texture_coordinates_rect_min, character_texture_coordinates_rect_max };
  return atlas_character_texture_coordinate;
}

float Font::GetAscent() const
{
  int ascent_i, _;
  stbtt_GetFontVMetrics(&mFontInfo, &ascent_i, &_, &_);
  return (ascent_i * mFontScale);
}

float Font::GetDescent() const
{
  int descent_i, _;
  stbtt_GetFontVMetrics(&mFontInfo, &_, &descent_i, &_);
  return (descent_i * mFontScale);
}

float Font::GetLineGap() const
{
  int linegap_i, _;
  stbtt_GetFontVMetrics(&mFontInfo, &_, &_, &linegap_i);
  return (linegap_i * mFontScale);
}

Mesh Font::GetTextMesh(const std::string_view inText,
    const ETextHAlignment& inHAlignment,
    const ETextVAlignment& inVAlignment) const
{
  // Get character rects
  auto text_rect = AARectf();
  std::vector<AARectf> character_rects;
  character_rects.reserve(inText.size());
  {
    auto current_baseline = Zero<Vec2f>();
    for (std::size_t i = 0; i < inText.size(); ++i)
    {
      const auto character = inText[i];
      if (character == '\n')
      {
        current_baseline[0] = 0;
        current_baseline[1] -= (GetLineHeight() + GetLineGap());
        character_rects.push_back(AARectf { Zero<Vec2f>(), Zero<Vec2f>() });
        continue;
      }

      auto character_rect = AARectf(GetCharacterRect(character));
      const auto character_size = character_rect.GetSize();
      const auto character_rect_min = Vec2f { character_rect.GetMin()[0], -character_rect.GetMax()[1] };
      const auto character_rect_max = Vec2f { character_rect.GetMax()[0], -character_rect.GetMin()[1] };
      character_rect.SetMinMax(character_rect_min, character_rect_max);
      character_rect += current_baseline;
      character_rects.push_back(character_rect);

      int advance_width_i, left_side_bearing_i;
      stbtt_GetCodepointHMetrics(&mFontInfo, character, &advance_width_i, &left_side_bearing_i);
      const auto advance_width = static_cast<float>(advance_width_i * mFontScale);
      UNUSED(left_side_bearing_i);
      current_baseline[0] += advance_width;

      if (i < inText.size() - 1)
      {
        const auto kerning = stbtt_GetCodepointKernAdvance(&mFontInfo, character, inText[i + 1]);
        current_baseline[0] += kerning * mFontScale;
      }

      text_rect.Wrap(character_rect);
    }
  }

  // Build mesh
  Mesh text_mesh;
  const auto alignment_offset = GetAlignmentOffset(text_rect, inHAlignment, inVAlignment);
  for (std::size_t i = 0; i < inText.size(); ++i)
  {
    const auto character = inText.at(i);
    const auto& character_rect = Translated(character_rects[i], alignment_offset);

    for (const auto& character_rect_point : MakePointsRange(character_rect))
    { text_mesh.AddVertex(XY0(character_rect_point)); }

    const auto i4 = i * 4;
    const auto face_id_0 = text_mesh.AddFace(i4, i4 + 2, i4 + 1);
    const auto face_id_1 = text_mesh.AddFace(i4 + 1, i4 + 2, i4 + 3);

    const auto character_texture_coordinates_rect = GetCharacterAtlasTextureCoordinatesRect(character);
    const auto& cr = character_texture_coordinates_rect;

    text_mesh.SetCornerTextureCoordinates(text_mesh.GetCornerIdFromFaceIdAndInternalCornerId(face_id_0, 0),
        Vec2f(cr.GetMin()[0], cr.GetMax()[1]));
    text_mesh.SetCornerTextureCoordinates(text_mesh.GetCornerIdFromFaceIdAndInternalCornerId(face_id_0, 1),
        Vec2f(cr.GetMax()[0], cr.GetMax()[1]));
    text_mesh.SetCornerTextureCoordinates(text_mesh.GetCornerIdFromFaceIdAndInternalCornerId(face_id_0, 2),
        Vec2f(cr.GetMin()[0], cr.GetMin()[1]));

    text_mesh.SetCornerTextureCoordinates(text_mesh.GetCornerIdFromFaceIdAndInternalCornerId(face_id_1, 0),
        Vec2f(cr.GetMin()[0], cr.GetMin()[1]));
    text_mesh.SetCornerTextureCoordinates(text_mesh.GetCornerIdFromFaceIdAndInternalCornerId(face_id_1, 1),
        Vec2f(cr.GetMax()[0], cr.GetMax()[1]));
    text_mesh.SetCornerTextureCoordinates(text_mesh.GetCornerIdFromFaceIdAndInternalCornerId(face_id_1, 2),
        Vec2f(cr.GetMax()[0], cr.GetMin()[1]));
  }

  return text_mesh;
}

Vec2f Font::GetAlignmentOffset(const AARectf& inTextRect,
    const ETextHAlignment& inHAlignment,
    const ETextVAlignment& inVAlignment) const
{
  auto alignment_offset = Zero<Vec2f>();
  switch (inHAlignment)
  {
  case ETextHAlignment::LEFT:
    alignment_offset[0] = 0.0f;
    break;

  case ETextHAlignment::CENTER:
    alignment_offset[0] = -inTextRect.GetSize()[0] * 0.5f;
    break;

  case ETextHAlignment::RIGHT:
    alignment_offset[0] = -inTextRect.GetSize()[0];
    break;
  }

  const auto ascent = GetAscent();
  const auto descent = GetDescent();
  UNUSED(ascent);
  UNUSED(descent);
  switch (inVAlignment)
  {
  case ETextVAlignment::TOP:
    alignment_offset[1] = -ascent;
    break;

  case ETextVAlignment::CENTER:
    alignment_offset[1] = (inTextRect.GetSize()[1] - ascent) * 0.5f;
    break;

  case ETextVAlignment::BASELINE:
    alignment_offset[1] = 0.0f;
    break;

  case ETextVAlignment::BOTTOM:
    alignment_offset[1] = inTextRect.GetSize()[1] - ascent;
    break;
  }

  return alignment_offset;
}

uint64_t Font::ComputeAtlasCharactersPlacement(const std::string_view inCharacters)
{
  const auto line_height = mFontSize;
  const auto padding = All<Vec2ul>(Ceil(mFontSize / 2.0f));

  // Get the size of all characters in a single row
  auto atlas_characters_all_in_a_row_width = 0.0f;
  for (const auto& atlas_character : inCharacters)
  {
    const auto character_width = GetCharacterSize(atlas_character)[0];
    atlas_characters_all_in_a_row_width += (padding[0] + character_width);
  }

  // Compute the area this row of all character occupies
  const auto atlas_characters_all_in_a_row_area = (line_height + padding[1]) * atlas_characters_all_in_a_row_width;

  // And then distribute it in a square
  constexpr auto JustInCaseScale = 1.5f;
  const auto atlas_image_size = static_cast<uint64_t>(Ceil(Sqrt(atlas_characters_all_in_a_row_area * JustInCaseScale)));
  if (atlas_image_size >= 4096)
    THROW_EXCEPTION("Font size is too big. Can't create font atlas because texture size would be very big");

  // Now simply write characters one after the other, and when we arrive to the end of the line, skip to the next line
  auto current_character_top_left = Zero<Vec2ul>();
  mCharactersAtlasRects = std::vector<AARectul>(256, AARectul { Zero<Vec2ul>(), Zero<Vec2ul>() });
  for (const auto& atlas_character : inCharacters)
  {
    EXPECTS(static_cast<uint64_t>(atlas_character) < mCharactersAtlasRects.size());

    const auto character_size = GetCharacterSize(atlas_character);
    const auto character_size_with_padding = (character_size + padding);

    auto atlas_character_placement = MakeAAHyperBoxFromMinSize(current_character_top_left + padding, character_size);
    if (atlas_character_placement.GetMax()[0] + padding[0] >= atlas_image_size)
    {
      current_character_top_left[0] = 0u;
      current_character_top_left[1] += (line_height + padding[1]);
      atlas_character_placement = MakeAAHyperBoxFromMinSize(current_character_top_left + padding, character_size);
    }

    mCharactersAtlasRects[atlas_character] = atlas_character_placement;
    assert(Contains(AARectul { Zero<Vec2ul>(), All<Vec2ul>(atlas_image_size) }, atlas_character_placement));

    current_character_top_left[0] += character_size_with_padding[0];
  }

  return atlas_image_size;
}
}