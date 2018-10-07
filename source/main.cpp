#include <iostream>
#include <spirv_glsl.hpp>
#include "cmd_line/CMDParser.h"


using namespace MantisSpirvCross;

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

int main(int argc, const char** argv) {

	CMDArgument show_help(CMDArgument::Type::Bool, "--help", "show help", false);
	CMDArgument input(CMDArgument::Type::String, "--input", "SPIR-V input file", std::string(), true);
	CMDArgument output(CMDArgument::Type::String, "--output", "output file", std::string(), true);
	CMDArgument lang(CMDArgument::Type::String, "--lang", "output language (glsl-100-es, glsl-330)", std::string(), true);

	CMDParser cmd_parser;

	if (!cmd_parser.Parse(argc, argv, {&show_help, &input, &output, &lang})) {
		std::vector<std::string> errors = cmd_parser.GetErrors();
		std::cerr << "Errors:" << std::endl;

		for (const auto& error : errors) {
			std::cerr << "- " << error << std::endl;
		}

		return 1;
	}

	// Set some options.
	spirv_cross::CompilerGLSL::Options options;

	if (lang.GetStringValue() == "glsl-100-es") {
		options.version = 100;
		options.es = true;
	}
	else if (lang.GetStringValue() == "glsl-330") {
		options.version = 330;
		options.es = false;
	}
	else {
		std::cerr << "Invalid value for '" << lang.GetIdentifier() << "' argument: " << lang.GetStringValue() << std::endl;
		return 1;
	}

	std::vector<uint32_t> spirv_binary = ReadSpirvFile(input.GetStringValue().data());

	std::string source;

	try {
		spirv_cross::CompilerGLSL glsl(std::move(spirv_binary));
		glsl.set_common_options(options);
		source = glsl.compile();
	}
	catch (const std::exception& e) {
		std::cerr << "Invalid SPIR-V file: " << input.GetStringValue() << std::endl;
		return 1;
	}

	WriteStringToFile(output.GetStringValue().data(), source.data());

	return 0;
}