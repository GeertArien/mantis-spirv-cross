#include "CMDArgument.h"


namespace MantisSpirvCross {

CMDArgument::CMDArgument(Type type, const std::string& identifier, const std::string& description, bool value)
		: type_(type), identifier_(identifier), description_(description), bool_value_(value)
{ }

CMDArgument::CMDArgument(Type type, const std::string& identifier, const std::string& description,
		const std::string& value, bool required)
		: type_(type), identifier_(identifier), description_(description), string_value_(value), required_(required)
{ }

void CMDArgument::SetBoolValue(bool value) {
	bool_value_ = value;
}

void CMDArgument::SetStringValue(const std::string& value) {
	string_value_ = value;
}

}
