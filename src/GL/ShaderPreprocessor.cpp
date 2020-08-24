#include <ez/ShaderPreprocessor.h>
#include <ez/FileUtils.h>
#include <ez/Macros.h>
#include <regex>

namespace ez
{
std::map<std::string, uint64_t> ShaderPreprocessor::sFileNameToFileIdentifier;

std::string ShaderPreprocessor::PreprocessShaderCode(const std::string_view inShaderCode,
    const Span<std::filesystem::path>& inIncludeDirs,
    const std::filesystem::path& inFilePath)
{
  std::set<std::filesystem::path> already_processed_files;
  return ShaderPreprocessor::PreprocessShaderCode(inShaderCode, inIncludeDirs, already_processed_files, inFilePath);
}

std::string ShaderPreprocessor::PreprocessShaderErrorMessage(const std::string_view inErrorMessage)
{
  std::string preprocessed_error_message { inErrorMessage };

  std::map<uint64_t, std::string> file_id_to_file_name_map;
  for (const auto& file_name_to_file_id : sFileNameToFileIdentifier)
    file_id_to_file_name_map[file_name_to_file_id.second] = file_name_to_file_id.first;

  std::size_t next_find_pos = 0;
  std::size_t file_identifier_token_pos = 0;
  while (true)
  {
    file_identifier_token_pos = preprocessed_error_message.find(FileIdentifierToken, next_find_pos);
    if (file_identifier_token_pos == std::string::npos)
      break;

    const auto first_char_after_token_pos = (file_identifier_token_pos + FileIdentifierToken.size());
    const auto end_of_file_identifier_pos
        = preprocessed_error_message.find_first_not_of("0123456789", first_char_after_token_pos);
    const auto file_identifier_length = (end_of_file_identifier_pos - first_char_after_token_pos);
    const auto file_identifier_str
        = preprocessed_error_message.substr(first_char_after_token_pos, file_identifier_length);
    const auto file_identifier_with_token_length = (file_identifier_length + FileIdentifierToken.size());
    const auto file_identifier = std::atoi(file_identifier_str.c_str());

    std::string file_name = "UnknowShaderFile";
    if (file_id_to_file_name_map.count(file_identifier) != 0)
    {
      file_name = file_id_to_file_name_map.at(file_identifier);
    }

    preprocessed_error_message.replace(file_identifier_token_pos, file_identifier_with_token_length, file_name);
    next_find_pos = end_of_file_identifier_pos;
  }

  return preprocessed_error_message;
}

std::string ShaderPreprocessor::PreprocessShaderCode(const std::string_view inShaderCode,
    const Span<std::filesystem::path>& inIncludeDirs,
    std::set<std::filesystem::path>& ioAlreadyProcessedFiles,
    const std::filesystem::path& inFilePath)
{
  sFileNameToFileIdentifier["UnknowShaderFile"] = 0;

  std::string preprocessed_code = "";
  const auto original_code = std::string { inShaderCode.data() };

  // Regexs
  const auto include_regex = std::regex { "#include[\\s]+[<\"]([a-zA-Z_\\-\\.0-9\\s/]+)[>\"]" };
  const auto verbatim_marker_open_regex = std::regex { "R\"\"\\(\\s*" };
  const auto verbatim_marker_close_regex = std::regex { "\\)\"\"\\s*" };

  // Retrieve code lines
  std::vector<std::string> original_code_lines;
  {
    std::string original_code_line;
    std::stringstream original_code_stream { original_code };
    while (std::getline(original_code_stream, original_code_line, '\n'))
    { original_code_lines.push_back(std::move(original_code_line)); }
  }

  const auto IsVersionLine = [](const std::string_view inLine) { return (inLine.find("#version") == 0); };
  const auto has_version_line = std::any_of(original_code_lines.cbegin(), original_code_lines.cend(), IsVersionLine);

  // Process line by line
  auto version_line_found = false;
  auto added_first_line_directive_already = false;
  auto file_identifier = 0;
  for (std::size_t original_line_number = 0; original_line_number < original_code_lines.size(); ++original_line_number)
  {
    const auto& line = original_code_lines.at(original_line_number);
    if (IsVersionLine(line)) // If it is the version line, copy and continue
    {
      preprocessed_code += line + "\n";
      version_line_found = true;
      continue;
    }

    // Ignore verbatim markers
    if (std::regex_match(line, verbatim_marker_open_regex) || std::regex_match(line, verbatim_marker_close_regex))
      continue;

    // After the version line and ignoring verbatim markers,
    // add the #line directive to indicate file identifier and first line (in case included from other file)
    const auto version_line_passed_if_any = (!has_version_line || version_line_found);
    if (version_line_passed_if_any && !added_first_line_directive_already && !inFilePath.empty())
    {
      const auto filename = inFilePath.filename();
      if (sFileNameToFileIdentifier.count(filename) == 0)
      {
        const auto new_file_id = sFileNameToFileIdentifier.size();
        sFileNameToFileIdentifier[filename] = new_file_id;
      }
      file_identifier = sFileNameToFileIdentifier.at(filename);
      const auto line_number = (has_version_line ? "2" : "1");
      preprocessed_code += std::string { "#line " } + line_number + " " + FileIdentifierToken
          + std::to_string(file_identifier) + "\n";
      added_first_line_directive_already = true;
    }

    // See if line is an include
    std::smatch includes_matches;
    if (!std::regex_search(line.cbegin(), line.cend(), includes_matches, include_regex))
    {
      preprocessed_code += line + "\n";
      continue;
    }

    // Replace include
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
        = PreprocessShaderCode(include_path_content, inIncludeDirs, ioAlreadyProcessedFiles, full_include_path);

    // Add include contents in preprocessed code
    preprocessed_code += preprocessed_include_path_content;
    preprocessed_code += "#line " + std::to_string(original_line_number + 2) + " " + FileIdentifierToken
        + std::to_string(file_identifier) + "\n";
  }
  return preprocessed_code;
}
}