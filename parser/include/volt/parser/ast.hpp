#pragma once

#include <memory>
#include <string>
#include <string_view>


namespace volt::parser {
	class ASTUnaryOperatorNode;
	class ASTBinaryOperatorNode;
	class ASTIntegerLiteral;
	class ASTTypeNode;

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
			virtual auto accept(parser::ASTIntegerLiteral& node) noexcept -> void = 0;
			virtual auto accept(parser::ASTTypeNode& node) noexcept -> void = 0;
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
	class ASTComptimeExpressionNode : public parser::ASTExpressionNode {
		public:
			constexpr ASTComptimeExpressionNode() noexcept = default;
			virtual constexpr ~ASTComptimeExpressionNode() override = default;
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
			inline ASTUnaryOperatorNode(
				parser::UnaryOperator operator_,
				std::unique_ptr<parser::ASTExpressionNode>&& child
			) noexcept :
				parser::ASTExpressionNode{},
				m_operator {operator_},
				m_child {std::move(child)}
			{}
			inline ~ASTUnaryOperatorNode() override = default;

			inline auto visit(parser::ASTVisitor& visitor) noexcept -> void override {
				visitor.accept(*this);
			}
			inline auto getOperator() const noexcept -> parser::UnaryOperator {
				return m_operator;
			}
			inline auto getChild() const noexcept -> parser::ASTExpressionNode& {
				return *m_child;
			}

		private:
			parser::UnaryOperator m_operator;
			std::unique_ptr<parser::ASTExpressionNode> m_child;
	};
	class ASTBinaryOperatorNode final : public parser::ASTExpressionNode {
		public:
			inline ASTBinaryOperatorNode(
				parser::BinaryOperator operator_,
				std::unique_ptr<parser::ASTExpressionNode>&& leftChild,
				std::unique_ptr<parser::ASTExpressionNode>&& rightChild
			) noexcept :
				parser::ASTExpressionNode{},
				m_operator {operator_},
				m_leftChild {std::move(leftChild)},
				m_rightChild {std::move(rightChild)}
			{}
			inline ~ASTBinaryOperatorNode() override = default;

			inline auto visit(parser::ASTVisitor& visitor) noexcept -> void override {
				visitor.accept(*this);
			}
			inline auto getOperator() const noexcept -> parser::BinaryOperator {
				return m_operator;
			}
			inline auto getLeftChild() const noexcept -> parser::ASTExpressionNode& {
				return *m_leftChild;
			}
			inline auto getRightChild() const noexcept -> parser::ASTExpressionNode& {
				return *m_rightChild;
			}

		private:
			parser::BinaryOperator m_operator;
			std::unique_ptr<parser::ASTExpressionNode> m_leftChild;
			std::unique_ptr<parser::ASTExpressionNode> m_rightChild;
	};

	class ASTIntegerLiteral final : public parser::ASTExpressionNode {
		public:
			inline ASTIntegerLiteral(__int128 value, std::u8string&& inCodeText) noexcept :
				m_value {value},
				m_inCodeText {std::move(inCodeText)}
			{}
			inline ~ASTIntegerLiteral() override = default;

			inline auto visit(parser::ASTVisitor& visitor) noexcept -> void override {
				visitor.accept(*this);
			}
			inline auto getValue() const noexcept -> __int128 {
				return m_value;
			}
			inline auto getInCodeText() const noexcept -> std::u8string_view {
				return m_inCodeText;
			}

		private:
			__int128 m_value;
			std::u8string m_inCodeText;
	};

	class ASTTypeNode final : public parser::ASTComptimeExpressionNode {
		public:
			inline ASTTypeNode(std::size_t UUID, std::u8string&& inCodeText) noexcept :
				m_UUID {UUID},
				m_identifier {},
				m_inCodeText {std::move(inCodeText)}
			{}
			inline ~ASTTypeNode() override = default;

			inline auto visit(parser::ASTVisitor& visitor) noexcept -> void override {
				visitor.accept(*this);
			}
			inline auto getUUID() const noexcept -> std::size_t {
				return m_UUID;
			}
			inline auto getInCodeText() const noexcept -> std::u8string_view {
				return m_inCodeText;
			}
			inline auto getIdentifier() const noexcept -> std::u8string_view {
				return m_identifier;
			}

		private:
			std::size_t m_UUID;
			std::u8string_view m_identifier;
			std::u8string m_inCodeText;
	};
}
