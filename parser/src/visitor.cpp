#include "volt/parser/visitor.hpp"


namespace volt::parser {
	auto ASTTraversalVisitor::accept(parser::ASTUnaryOperatorNode& node) noexcept -> void {
		node.visit(m_visitor);
		node.getChild().visit(*this);
	}

	auto ASTTraversalVisitor::accept(parser::ASTBinaryOperatorNode& node) noexcept -> void {
		node.getLeftChild().visit(*this);
		node.visit(m_visitor);
		node.getRightChild().visit(*this);
	}

	auto ASTTraversalVisitor::accept(parser::ASTIntegerLiteral& node) noexcept -> void {
		node.visit(m_visitor);
	}

	auto ASTTraversalVisitor::accept(parser::ASTTypeNode& node) noexcept -> void {
		node.visit(m_visitor);
	}
}
