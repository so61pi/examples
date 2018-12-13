#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP


// #define BOOST_SPIRIT_DEBUG


#include <string>
#include <vector>
#include <boost/spirit/include/qi.hpp>


enum class Operator {
    multiplies, divides, modulus,
    plus, minus,
    shift_left, shift_right,
    greater_equal, less_equal, greater, less,
    equal_to, not_equal_to,
    bitwise_and,
    bitwise_xor,
    bitwise_or,
    logical_and,
    logical_or,
    assign,
    multiplies_assign,
    divides_assign,
    modulus_assign,
    plus_assign,
    minus_assign,
    shift_left_assign,
    shift_right_assign,
    bitwise_and_assign,
    bitwise_xor_assign,
    bitwise_or_assign,
    pre_inc, pre_dec, post_inc, post_dec,
    address_of, dereference, unary_plus, negate, complement, logical_not,
    access_mem, mem_ptr,
    if_else_,
    function_call,
    size_of
};


template<typename Iterator, typename Signature, typename Skipper>
class c_grammar : public boost::spirit::qi::grammar<Iterator, Signature, Skipper>
{
    template<typename... Ts>
    using rule_type = boost::spirit::qi::rule<Iterator, Ts...>;

    template<typename T = boost::spirit::qi::unused_type>
    using symbols_type = boost::spirit::qi::symbols<char, T>;

public:
    c_grammar();
    ~c_grammar();

private:
    symbols_type<> keywords;
    symbols_type<Operator> multiplicative_operator;
    symbols_type<Operator> additive_operator;
    symbols_type<Operator> shift_operator;
    symbols_type<Operator> relational_operator;
    symbols_type<Operator> equality_operator;
    symbols_type<Operator> AND_operator;
    symbols_type<Operator> exclusive_OR_operator;
    symbols_type<Operator> inclusive_OR_operator;
    symbols_type<Operator> logical_AND_operator;
    symbols_type<Operator> logical_OR_operator;

    symbols_type<> typedef_table;

    rule_type<char()> character_set;
    rule_type<std::string()> identifier;
    rule_type<char()> identifier_nondigit;
    rule_type<char()> digit;
    rule_type<char()> nondigit;
    rule_type<char()> hexadecimal_digit;
    rule_type<std::string()> constant;
    rule_type<std::string()> integer_constant;
    rule_type<std::string()> floating_constant;
    rule_type<std::string()> enumeration_constant;
    rule_type<std::string()> character_constant;
    rule_type<std::string()> decimal_constant;
    rule_type<std::string()> integer_suffix;
    rule_type<std::string()> octal_constant;
    rule_type<std::string()> hexadecimal_constant;
    rule_type<char()> nonzero_digit;
    rule_type<char()> octal_digit;
    rule_type<> hexadecimal_prefix;
    rule_type<std::string()> unsigned_suffix;
    rule_type<std::string()> long_suffix;
    rule_type<std::string()> long_long_suffix;
    rule_type<std::string()> decimal_floating_constant;
    rule_type<std::string()> hexadecimal_floating_constant;
    rule_type<std::string()> fractional_constant;
    rule_type<std::string()> exponent_part;
    rule_type<std::string()> floating_suffix;
    rule_type<std::string()> digit_sequence;
    rule_type<std::string()> hexadecimal_fractional_constant;
    rule_type<std::string()> binary_exponent_part;
    rule_type<std::string()> hexadecimal_digit_sequence;
    rule_type<char()> sign;
    rule_type<std::string()> c_char_sequence;
    rule_type<char()> c_char;
    rule_type<std::string()> escape_sequence;
    rule_type<std::string()> simple_escape_sequence;
    rule_type<std::string()> octal_escape_sequence;
    rule_type<std::string()> hexadecimal_escape_sequence;
    rule_type<std::string()> string_literal;
    rule_type<> encoding_prefix;
    rule_type<std::string()> s_char_sequence;
    rule_type<char()> s_char;
    rule_type<Skipper> primary_expression;
    rule_type<Skipper> expression;
    rule_type<Skipper> generic_selection;
    rule_type<Skipper> assignment_expression;
    rule_type<Skipper> generic_assoc_list;
    rule_type<Skipper> generic_association;
    rule_type<Skipper> type_name;
    rule_type<Skipper> postfix_expression;
    rule_type<Skipper> postfix_expression_helper;
    rule_type<Skipper> argument_expression_list;
    rule_type<Skipper> initializer_list;
    rule_type<Skipper> unary_expression;
    symbols_type<Operator> unary_operator;
    rule_type<Skipper> cast_expression;
    rule_type<Skipper> multiplicative_expression;
    rule_type<Skipper> additive_expression;
    rule_type<Skipper> shift_expression;
    rule_type<Skipper> relational_expression;
    rule_type<Skipper> equality_expression;
    rule_type<Skipper> AND_expression;
    rule_type<Skipper> exclusive_OR_expression;
    rule_type<Skipper> inclusive_OR_expression;
    rule_type<Skipper> logical_AND_expression;
    rule_type<Skipper> logical_OR_expression;
    rule_type<Skipper> conditional_expression;
    symbols_type<Operator> assignment_operator;
    rule_type<Skipper> constant_expression;
    rule_type<Skipper> declaration;
    rule_type<Skipper, std::vector<std::string>()> declaration_specifiers;
    rule_type<Skipper> init_declarator_list;
    rule_type<Skipper> static_assert_declaration;
    rule_type<std::string()> storage_class_specifier;
    rule_type<Skipper, std::string()> type_specifier;
    rule_type<std::string()> type_qualifier;
    rule_type<std::string()> function_specifier;
    rule_type<Skipper> alignment_specifier;
    rule_type<Skipper> init_declarator;
    rule_type<Skipper> declarator;
    rule_type<Skipper> initializer;
    rule_type<Skipper> atomic_type_specifier;
    rule_type<Skipper, std::string()> struct_or_union_specifier;
    rule_type<Skipper, std::string()> enum_specifier;
    rule_type<std::string()> typedef_name;
    rule_type<std::string()> struct_or_union;
    rule_type<Skipper> struct_declaration_list;
    rule_type<Skipper> struct_declaration;
    rule_type<Skipper> specifier_qualifier_list;
    rule_type<Skipper> struct_declarator_list;
    rule_type<Skipper> struct_declarator;
    rule_type<Skipper> enumerator_list;
    rule_type<Skipper> enumerator;
    rule_type<Skipper> pointer;
    rule_type<Skipper> direct_declarator;
    rule_type<Skipper> direct_declarator_helper;
    rule_type<Skipper, std::vector<std::string>()> type_qualifier_list;
    rule_type<Skipper> parameter_type_list;
    rule_type<Skipper> identifier_list;
    rule_type<Skipper> parameter_list;
    rule_type<Skipper> parameter_declaration;
    rule_type<Skipper> abstract_declarator;
    rule_type<Skipper> direct_abstract_declarator;
    rule_type<Skipper> direct_abstract_declarator_helper;
    rule_type<Skipper> designation;
    rule_type<Skipper> designator_list;
    rule_type<Skipper> designator;
    rule_type<Skipper> statement;
    rule_type<Skipper> labeled_statement;
    rule_type<Skipper> compound_statement;
    rule_type<Skipper> expression_statement;
    rule_type<Skipper> selection_statement;
    rule_type<Skipper> iteration_statement;
    rule_type<Skipper> jump_statement;
    rule_type<Skipper> block_item_list;
    rule_type<Skipper> block_item;
    rule_type<Signature, Skipper> translation_unit;
    rule_type<Skipper> external_declaration;
    rule_type<Skipper> function_definition;
    rule_type<Skipper> declaration_list;
    rule_type<> new_line;
};


#endif // GRAMMAR_HPP
