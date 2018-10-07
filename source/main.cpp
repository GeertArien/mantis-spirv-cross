#include <iostream>
#include <iomanip>
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

void PrintHelp(const std::vector<CMDArgument*> arguments) {
	std::cout << "Arguments:\n";

	for (const auto& argument : arguments) {
		std::cout << std::left << std::setw(13) << argument->GetIdentifier() << std::setw(45) << argument->GetDescription();
		if (argument->IsRequired()) {
			std::cout << "required";
		}
		std::cout << '\n';
	}

	std::cout << '\n' << "Example:" << '\n'
			  << "mantis-spirv-cross --input flat.spv --output flat.frag --lang glsl-100-es" << std::endl;
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

	CMDArgument show_help(CMDArgument::Type::Bool, "--help", "show help", false);
	CMDArgument input(CMDArgument::Type::String, "--input", "SPIR-V input file", std::string(), true);
	CMDArgument output(CMDArgument::Type::String, "--output", "output file", std::string(), true);
	CMDArgument lang(CMDArgument::Type::String, "--lang", "output language (glsl-100-es, glsl-330)", std::string(), true);

	CMDParser cmd_parser;

	if (!cmd_parser.Parse(argc, argv, { &show_help, &input, &output, &lang }) && !show_help.GetBoolValue()) {
		std::vector<std::string> errors = cmd_parser.GetErrors();
		std::cerr << "Errors:" << std::endl;

		for (const auto& error : errors) {
			std::cerr << "- " << error << std::endl;
		}

		return 1;
	}

	if (show_help.GetBoolValue()) {
		PrintHelp({ &show_help, &input, &output, &lang });
		return 0;
	}
	else {
		if (CrossCompile(input.GetStringValue(), output.GetStringValue(), lang.GetStringValue())) {
			return 0;
		}
		else {
			return 1;
		}
	}
}