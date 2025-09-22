#include <print>
#include <string>

#include <volt/lx/lexer.hpp>
#include <volt/lx/token.hpp>

auto toSv(std::u8string_view view) {
	return std::string_view{reinterpret_cast<const char*> (view.data()), view.size()};
}


auto main(int, char**) -> int {
	std::u8string text {
		u8"  \n Hello World\n"
		u8"   some other_identifier with µs"
	};

	std::println("{}", toSv(text));

	for (const auto token : volt::lx::lex(text)) {
		if (token.type == volt::lx::TokenType::eEOL)
			std::println("EOL");
		else if (token.type == volt::lx::TokenType::eEOF)
			std::println("EOF");
		else if (token.type == volt::lx::TokenType::eIdentifier)
			std::println("Identifier: '{}'", toSv(std::get<std::u8string_view> (token.metadata)));
		else
			std::println("unknown token");
	}

	return 0;
}
