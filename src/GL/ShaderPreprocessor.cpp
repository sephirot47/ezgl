#include "ez/ShaderPreprocessor.h"
#include "ez/FileUtils.h"
#include "ez/Macros.h"
#include <regex>

namespace ez
{
std::string ShaderPreprocessor::PreprocessShaderCode(const std::string_view inShaderCode,
    const Span<std::filesystem::path>& inIncludeDirs)
{
  std::set<std::filesystem::path> already_processed_files;
  return ShaderPreprocessor::PreprocessShaderCode(inShaderCode, inIncludeDirs, already_processed_files);
}

std::string ShaderPreprocessor::PreprocessShaderCode(const std::string_view inShaderCode,
    const Span<std::filesystem::path>& inIncludeDirs,
    std::set<std::filesystem::path>& ioAlreadyProcessedFiles)
{
  std::string preprocessed_code = inShaderCode.data();

  // Remove verbatim markers
  const auto verbatim_marker_open_regex = std::regex { "R\"\"\\(" };
  const auto verbatim_marker_close_regex = std::regex { "\\)\"\"" };
  preprocessed_code = std::regex_replace(preprocessed_code, verbatim_marker_open_regex, "");
  preprocessed_code = std::regex_replace(preprocessed_code, verbatim_marker_close_regex, "");

  // Substitute includes
  const auto include_regex = std::regex { "#include[\\s]+[<\"]([a-zA-Z_\\-\\.0-9\\s/]+)[>\"]" };
  std::smatch includes_matches;
  std::string::const_iterator search_begin { preprocessed_code.cbegin() };
  while (std::regex_search(search_begin, preprocessed_code.cend(), includes_matches, include_regex))
  {
    std::filesystem::path partial_include_path = includes_matches[1].str();

    // Search include path using include dirs
    std::filesystem::path full_include_path;
    for (const auto& include_dir : inIncludeDirs)
    {
      auto full_include_path_to_try = include_dir / partial_include_path;
      if (std::filesystem::exists(full_include_path_to_try))
      {
        full_include_path = std::move(full_include_path_to_try);
        break;
      }
    }

    // Handle errors
    {
      if (full_include_path.empty())
        THROW_EXCEPTION("Included path " << partial_include_path
                                         << " does not exist or was not found using the provided include paths");
    }

    const auto first_time_included = (ioAlreadyProcessedFiles.count(full_include_path) == 0);
    ioAlreadyProcessedFiles.insert(full_include_path);

    // Add the preprocessed contents of the included file
    const auto include_path_content = (first_time_included ? GetFileContents(full_include_path) : "");
    const auto preprocessed_include_path_content
        = PreprocessShaderCode(include_path_content, inIncludeDirs, ioAlreadyProcessedFiles);

    // Replace include contents in shader code
    const std::size_t include_statement_pos = includes_matches[0].first - preprocessed_code.cbegin();
    const std::size_t include_statement_length = includes_matches[0].length();
    preprocessed_code.replace(include_statement_pos, include_statement_length, preprocessed_include_path_content);

    // Prepare next begin iterator
    search_begin = (preprocessed_code.cbegin() + include_statement_pos + include_path_content.length());
  }

  return preprocessed_code;
}
}