#pragma once

#include <generator>

#include "volt/lx/token.hpp"
#include "volt/parser/ast.hpp"


namespace volt::parser {
	auto parse(std::generator<lx::Token>&& tokens) noexcept -> std::unique_ptr<parser::ASTNode>;
}
