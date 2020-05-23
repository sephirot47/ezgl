#pragma once

#include "ez/ETextHAlignment.h"
#include "ez/ETextVAlignment.h"
#include "ez/Texture2D.h"
#include "ez/stb_truetype.h"
#include <filesystem>
#include <memory>

struct stbtt_fontinfo;

namespace ez
{
class Mesh;

class Font final
{
public:
  Font(const std::filesystem::path& inFontPath, const uint64_t inFontSize);
  Font(const Font&) = delete;
  Font& operator=(const Font&) = delete;
  Font(Font&&) = default;
  Font& operator=(Font&&) = default;
  ~Font() = default;

  Vec2ul GetCharacterSize(const char inCharacter) const;
  AARecti GetCharacterRect(const char inCharacter) const;
  AARectul GetCharacterAtlasRect(const char inCharacter) const;
  AARectf GetCharacterAtlasTextureCoordinatesRect(const char inCharacter) const;
  Mesh GetTextMesh(const std::string_view inText,
      const ETextHAlignment& inHAlignment,
      const ETextVAlignment& inVAlignment) const;

  std::shared_ptr<Texture2D> GetAtlasTexture() const { return mAtlasTexture; }
  float GetFontScale() const { return mFontScale; }
  uint64_t GetFontSize() const { return mFontSize; }
  float GetLineHeight() const { return GetFontSize(); }
  float GetAscent() const;
  float GetDescent() const;
  float GetLineGap() const;

private:
  std::shared_ptr<Texture2D> mAtlasTexture;
  uint64_t mFontSize { 0ul };
  float mFontScale { 0.0f };

  stbtt_fontinfo mFontInfo;
  std::vector<uint8_t> mFontBuffer;
  std::vector<AARectul> mCharactersAtlasRects;

  Vec2f GetAlignmentOffset(const AARectf& inTextRect,
      const ETextHAlignment& inHAlignment,
      const ETextVAlignment& inVAlignment) const;
  uint64_t ComputeAtlasCharactersPlacement(const std::string_view inCharacters);
};
}