#pragma once

#include <string>


namespace MantisSpirvCross {

	class CMDArgument final {

	public:
		enum class Type {
			Bool,
			String
		};

		explicit CMDArgument(Type type, const std::string& identifier, const std::string& description, bool value);
		explicit CMDArgument(Type type, const std::string& identifier, const std::string& description,
				const std::string& value, bool required);

		void SetBoolValue(bool value);
		void SetStringValue(const std::string& value);

		Type GetType() const
		{ return type_; }

		const std::string& GetIdentifier() const
		{ return identifier_; }

		const std::string& GetDescription() const
		{ return description_; }

		const std::string& GetStringValue() const
		{ return string_value_; }

		bool GetBoolValue() const
		{ return bool_value_; }

		bool IsRequired() const
		{ return required_; }


	private:
		Type type_;
		std::string identifier_;
		std::string description_;
		bool bool_value_ = false;
		std::string string_value_;
		bool required_ = false;

	};

}