#include <print>
#include <string>

#include <volt/lx/lexer.hpp>
#include <volt/lx/token.hpp>

auto toSv(std::u8string_view view) {
	return std::string_view{reinterpret_cast<const char*> (view.data()), view.size()};
}


auto main(int, char**) -> int {
	std::u8string text {
		u8"hello= -1_0e+20;\n"
		u8"1+2;\n"
		u8"other = \"hi\\\"hi\";\n"
		u8"char = 'A';\n"
		u8"if (hello ==world) /* comment */ {\n"
		u8"\thello += world;\n"
		u8"} // other comment"
	};

	std::println("{}", toSv(text));

	for (const auto token : volt::lx::lex(text)) {
		if (token.type == volt::lx::TokenType::eEOL)
			std::println("EOL");
		else if (token.type == volt::lx::TokenType::eEOF)
			std::println("EOF");
		else if (token.type == volt::lx::TokenType::eEOS)
			std::println("EOS");
		else if (token.type == volt::lx::TokenType::eIdentifier)
			std::println("Identifier: '{}'", toSv(std::get<std::u8string_view> (token.metadata)));
		else if (token.type == volt::lx::TokenType::eOperator)
			std::println("Operator: '{}'", toSv(std::get<std::u8string_view> (token.metadata)));
		else if (token.type == volt::lx::TokenType::eKeywordIf)
			std::println("Keyword if");
		else if (token.type == volt::lx::TokenType::eKeywordElse)
			std::println("Keyword else");
		else if (token.type == volt::lx::TokenType::eOpenComment)
			std::println("Open comment");
		else if (token.type == volt::lx::TokenType::eCloseComment)
			std::println("Close comment");
		else if (token.type == volt::lx::TokenType::eSingleLineComment)
			std::println("Single line comment");
		else if (token.type == volt::lx::TokenType::eCommentContent)
			std::println("Comment: '{}'", toSv(std::get<std::u8string_view> (token.metadata)));
		else if (token.type == volt::lx::TokenType::eLiteralNumber)
			std::println("Number literal: {}", toSv(std::get<std::u8string_view> (token.metadata)));
		else if (token.type == volt::lx::TokenType::eLiteralString)
			std::println("String literal: '{}'", toSv(std::get<std::u8string_view> (token.metadata)));
		else if (token.type == volt::lx::TokenType::eLiteralCharacter)
			std::println("Character literal: '{}'", toSv(std::get<std::u8string_view> (token.metadata)));
		else
			std::println("unknown token");
	}

	return 0;
}
