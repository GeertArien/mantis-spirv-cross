#include <iostream>
#include <iomanip>
#include <Parser.h>
#include <Argument.h>
#include <spirv_glsl.hpp>


static std::vector<uint32_t> ReadSpirvFile(const char *path) {
	FILE *file = fopen(path, "rb");
	if (!file) {
		fprintf(stderr, "Failed to open SPIRV file: %s\n", path);
		return {};
	}

	fseek(file, 0, SEEK_END);
	size_t len = ftell(file) / sizeof(uint32_t);
	rewind(file);

	std::vector<uint32_t> spirv(len);
	if (fread(spirv.data(), sizeof(uint32_t), len, file) != len)
		spirv.clear();

	fclose(file);
	return spirv;
}

static bool WriteStringToFile(const char *path, const char *string) {
	FILE *file = fopen(path, "w");
	if (!file) {
		fprintf(stderr, "Failed to write file: %s\n", path);
		return false;
	}

	fprintf(file, "%s", string);
	fclose(file);
	return true;
}

bool CrossCompile(const std::string& input_file, const std::string& output_file, const std::string& language) {
	// Set some options.
	spirv_cross::CompilerGLSL::Options options;

	if (language == "glsl-100-es") {
		options.version = 100;
		options.es = true;
	}
	else if (language == "glsl-330") {
		options.version = 330;
		options.es = false;
	}
	else {
		std::cerr << "Invalid target language '" << language << "'" << std::endl;
		return false;
	}

	std::vector<uint32_t> spirv_binary = ReadSpirvFile(input_file.data());

	std::string source;

	try {
		spirv_cross::CompilerGLSL glsl(std::move(spirv_binary));
		glsl.set_common_options(options);
		source = glsl.compile();
	}
	catch (const std::exception& e) {
		std::cerr << "Invalid SPIR-V file: " << input_file << std::endl;
		return false;
	}

	return WriteStringToFile(output_file.data(), source.data());
}

int main(int argc, const char** argv) {

	CMD::Argument show_help(CMD::Argument::Type::Bool, "--help", "show help", false);
	CMD::Argument input(CMD::Argument::Type::String, "--input", "SPIR-V input file", std::string(), true);
	CMD::Argument output(CMD::Argument::Type::String, "--output", "output file", std::string(), true);
	CMD::Argument lang(CMD::Argument::Type::String, "--lang", "output language (glsl-100-es, glsl-330)", std::string(), true);

	CMD::Parser cmd_parser("--input flat.spv --output flat.frag --lang glsl-100-es");
	cmd_parser.SetArguments({ &show_help, &input, &output, &lang });

	if (!cmd_parser.Parse(argc, argv)) {
		if (!show_help.GetBoolValue()) {
			cmd_parser.PrintErrors(std::cerr);
			return 1;
		}
	}

	if (show_help.GetBoolValue()) {
		cmd_parser.PrintHelp(std::cout);
		return 0;
	}

	if (CrossCompile(input.GetStringValue(), output.GetStringValue(), lang.GetStringValue())) {
		return 0;
	}
	else {
		return 1;
	}
}