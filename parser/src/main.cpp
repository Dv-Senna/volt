#include <cstdlib>
#include <print>
#include <utility>

#include "volt/lx/lexer.hpp"
#include "volt/parser/ast.hpp"
#include "volt/parser/parser.hpp"


class PrettyPrintVisitor final : public volt::parser::ASTVisitor {
	public:
		constexpr PrettyPrintVisitor() = default;
		constexpr ~PrettyPrintVisitor() override = default;

		inline auto accept(volt::parser::ASTUnaryOperatorNode& node) noexcept -> void override {
			std::println("Unary operator {}", std::to_underlying(node.getOperator()));
		}
		inline auto accept(volt::parser::ASTBinaryOperatorNode& node) noexcept -> void override {
			std::println("Binary operator {}", std::to_underlying(node.getOperator()));
		}
		inline auto accept(volt::parser::ASTIntegerLiteral& node) noexcept -> void override {

		}
};


auto main(int, char**) -> int {
	return EXIT_SUCCESS;
}
