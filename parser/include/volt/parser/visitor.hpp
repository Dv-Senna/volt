#pragma once

#include "volt/parser/ast.hpp"
#include "volt/parser/export.hpp"


namespace volt::parser {
	class ASTTraversalVisitor final : public parser::ASTVisitor {
		public:
			inline ASTTraversalVisitor(parser::ASTVisitor& visitor) noexcept :
				m_visitor {visitor}
			{}
			constexpr ~ASTTraversalVisitor() override = default;

			VOLT_PARSER_EXPORT auto accept(parser::ASTUnaryOperatorNode& node) noexcept -> void override;
			VOLT_PARSER_EXPORT auto accept(parser::ASTBinaryOperatorNode& node) noexcept -> void override;
			VOLT_PARSER_EXPORT auto accept(parser::ASTIntegerLiteral& node) noexcept -> void override;
			VOLT_PARSER_EXPORT auto accept(parser::ASTTypeNode& node) noexcept -> void override;

		private:
			ASTVisitor& m_visitor;
	};
}
