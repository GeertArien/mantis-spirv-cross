#pragma once

#include <vector>
#include <string>
#include "CMDArgument.h"


namespace MantisSpirvCross {

	class CMDParser final {

	public:
		bool Parse(int argc, const char** argv, std::vector<CMDArgument*> arguments);

		std::vector<std::string> GetErrors() const
		{ return errors_; }

	private:
		std::vector<std::string> errors_;

		void ValidateArguments(std::vector<CMDArgument*>& arguments);

	};

}


