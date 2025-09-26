#pragma once

#include <memory>


namespace volt::parser {
	class ASTUnaryOperatorNode;
	class ASTBinaryOperatorNode;

	class ASTVisitor {
		public:
			ASTVisitor(const ASTVisitor&) = delete;
			auto operator=(const ASTVisitor&) -> ASTVisitor& = delete;
			ASTVisitor(ASTVisitor&&) = delete;
			auto operator=(ASTVisitor&&) -> ASTVisitor& = delete;

			constexpr ASTVisitor() noexcept = default;
			virtual constexpr ~ASTVisitor() noexcept = default;

			virtual auto accept(parser::ASTUnaryOperatorNode& node) noexcept -> void = 0;
			virtual auto accept(parser::ASTBinaryOperatorNode& node) noexcept -> void = 0;
	};

	class ASTNode {
		public:
			ASTNode(const ASTNode&) = delete;
			auto operator=(const ASTNode&) -> ASTNode& = delete;
			ASTNode(ASTNode&&) = delete;
			auto operator=(ASTNode&&) -> ASTNode& = delete;

			constexpr ASTNode() noexcept = default;
			virtual constexpr ~ASTNode() = default;

			virtual auto visit(parser::ASTVisitor& visitor) noexcept -> void = 0;
	};

	class ASTExpressionNode : public parser::ASTNode {
		public:
			constexpr ASTExpressionNode() noexcept = default;
			virtual constexpr ~ASTExpressionNode() override = default;
	};
	class ASTStatementNode : public parser::ASTNode {
		public:
			constexpr ASTStatementNode() noexcept = default;
			virtual constexpr ~ASTStatementNode() override = default;
	};

	enum class UnaryOperator {
		ePlus,
		eMinus,
		eLogicalNot,
		eBitwiseNot,
	};
	enum class BinaryOperator {
		ePlus,
		eMinus,
		eTimes,
		eDivide,
		eModulus,
		ePower,

		eLogicalOr,
		eLogicalAnd,
		eLogicalXor,
		eLogicalEqual,
		eLogicalNotEqual,
		eLogicalGreater,
		eLogicalLess,
		eLogicalGreaterOrEqual,
		eLogicalLessOrEqual,

		eBitwiseOr,
		eBitwiseAnd,
		eBitwiseXor,
		eBitwiseLeftShift,
		eBitwiseRightShift,
	};

	class ASTUnaryOperatorNode final : public parser::ASTExpressionNode {
		public:
			constexpr ASTUnaryOperatorNode(std::unique_ptr<parser::ASTExpressionNode>&& child) noexcept :
				parser::ASTExpressionNode{},
				m_child {std::move(child)}
			{}
			constexpr ~ASTUnaryOperatorNode() override = default;

			inline auto visit(parser::ASTVisitor& visitor) noexcept -> void override {
				visitor.accept(*this);
			}
			inline auto getChild() const noexcept -> parser::ASTExpressionNode& {
				return *m_child;
			}

		private:
			std::unique_ptr<parser::ASTExpressionNode> m_child;
	};
	class ASTBinaryOperatorNode final : public parser::ASTExpressionNode {
		public:
			constexpr ASTBinaryOperatorNode(
				std::unique_ptr<parser::ASTExpressionNode>&& leftChild,
				std::unique_ptr<parser::ASTExpressionNode>&& rightChild
			) noexcept :
				parser::ASTExpressionNode{},
				m_leftChild {std::move(leftChild)},
				m_rightChild {std::move(rightChild)}
			{}
			constexpr ~ASTBinaryOperatorNode() override = default;

			inline auto visit(parser::ASTVisitor& visitor) noexcept -> void override {
				visitor.accept(*this);
			}
			inline auto getLeftChild() const noexcept -> parser::ASTExpressionNode& {
				return *m_leftChild;
			}
			inline auto getRightChild() const noexcept -> parser::ASTExpressionNode& {
				return *m_rightChild;
			}

		private:
			std::unique_ptr<parser::ASTExpressionNode> m_leftChild;
			std::unique_ptr<parser::ASTExpressionNode> m_rightChild;
	};
}
