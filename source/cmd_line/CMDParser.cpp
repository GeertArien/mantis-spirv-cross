#include "CMDParser.h"


namespace MantisSpirvCross {

bool CMDParser::Parse(int argc, const char** argv, std::vector<CMDArgument*> arguments) {
	errors_.clear();

	for (size_t i = 1; i < argc; ++i) {
		const std::string current_identifier = argv[i];
		bool found_argument = false;

		for (auto& argument : arguments) {
			if (argument->GetIdentifier() == current_identifier) {
				found_argument = true;

				if (argument->GetType() == CMDArgument::Type::String) {
					if (++i < argc) {
						argument->SetStringValue(argv[i]);
						break;
					}
					else {
						errors_.push_back("Expected value for argument '" + argument->GetIdentifier() + "'");
					}
				}
				else {
					argument->SetBoolValue(true);
					break;
				}
			}
		}

		if (!found_argument) {
			errors_.push_back("Invalid argument '" + current_identifier + "'");
		}
	}

	ValidateArguments(arguments);
	return errors_.empty();
}

void CMDParser::ValidateArguments(std::vector<CMDArgument*>& arguments) {
	for (const auto& argument : arguments) {
		if (argument->IsRequired() && argument->GetStringValue().empty()) {
			errors_.push_back("'" + argument->GetIdentifier() + "' is a required argument");
		}
	}
}

}