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
  std::string preprocessed_code = "";
  const auto original_code = std::string { inShaderCode.data() };

  const auto include_regex = std::regex { "#include[\\s]+[<\"]([a-zA-Z_\\-\\.0-9\\s/]+)[>\"]" };
  const auto verbatim_marker_open_regex = std::regex { "R\"\"\\(\\s*" };
  const auto verbatim_marker_close_regex = std::regex { "\\)\"\"\\s*" };

  std::vector<std::string> original_code_lines = { "" };
  {
    std::stringstream original_code_stream { original_code };
    while (std::getline(original_code_stream, original_code_lines.back(), '\n')) { original_code_lines.emplace_back(); }
  }

  // Process line by line
  for (std::size_t original_line_number = 0; original_line_number < original_code_lines.size(); ++original_line_number)
  {
    const auto& line = original_code_lines.at(original_line_number);

    // Remove verbatim markers
    if (std::regex_match(line, verbatim_marker_open_regex) || std::regex_match(line, verbatim_marker_close_regex))
      continue;

    std::smatch includes_matches;
    if (!std::regex_search(line.cbegin(), line.cend(), includes_matches, include_regex))
    {
      preprocessed_code += line + "\n";
      continue;
    }
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

    // Add include contents in preprocessed code
    preprocessed_code += std::string { "#line 2" } + "\n";
    preprocessed_code += preprocessed_include_path_content;
    preprocessed_code += "#line " + std::to_string(original_line_number + 2) + "\n";
  }
  return preprocessed_code;
}
}