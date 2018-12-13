#ifndef GRAMMAR_CPP_HPP
#define GRAMMAR_CPP_HPP


#include "grammar.hpp"

#include <iostream>


template<typename Iterator, typename Signature, typename Skipper>
c_grammar<Iterator, Signature, Skipper>::c_grammar()
    : c_grammar::base_type(translation_unit)
{
    namespace qi = boost::spirit::qi;


    keywords
        =   "enum", "restrict", "unsigned", "break", "extern", "return", "void", "case",
            "float", "short", "volatile", "char", "for", "signed", "while", "const", "goto",
            "sizeof", "continue", "if", "static", "default", "inline", "struct", "do", "int",
            "switch", "double", "long", "typedef", "else", "union"
        ;


    multiplicative_operator.add
        ("*", Operator::multiplies)
        ("/", Operator::divides)
        ("%", Operator::modulus)
        ;

    additive_operator.add
        ("+", Operator::plus)
        ("-", Operator::minus)
        ;

    shift_operator.add
        ("<<", Operator::shift_left)
        (">>", Operator::shift_right)
        ;

    relational_operator.add
        (">=", Operator::greater_equal)
        ("<=", Operator::less_equal)
        (">", Operator::greater)
        ("<", Operator::less)
        ;

    equality_operator.add
        ("==", Operator::equal_to)
        ("!=", Operator::not_equal_to)
        ;

    AND_operator.add
        ("&", Operator::bitwise_and)
        ;

    exclusive_OR_operator.add
        ("^", Operator::bitwise_xor)
        ;

    inclusive_OR_operator.add
        ("|", Operator::bitwise_or)
        ;

    logical_AND_operator.add
        ("&&", Operator::logical_and)
        ;

    logical_OR_operator.add
        ("||", Operator::logical_or)
        ;


    character_set
        =   qi::alnum
        |   qi::char_('!') | qi::char_('"') | qi::char_('#') | qi::char_('%')
        |   qi::char_('&') | qi::char_('\'') | qi::char_('(') | qi::char_(')')
        |   qi::char_('*') | qi::char_('+') | qi::char_(',') | qi::char_('-')
        |   qi::char_('.') | qi::char_('/') | qi::char_(':')
        |   qi::char_(';') | qi::char_('<') | qi::char_('=') | qi::char_('>')
        |   qi::char_('?') | qi::char_('[') | qi::char_('\\') | qi::char_(']')
        |   qi::char_('^') | qi::char_('_') | qi::char_('{') | qi::char_('|')
        |   qi::char_('}') | qi::char_('~')
        |   new_line
        ;

    //
    // A.1.3
    //
    
    // identifier
    //     =   identifier_nondigit
    //     |   identifier >> identifier_nondigit
    //     |   identifier >> digit
    //     ;
    //
    identifier
        =   qi::lexeme[identifier_nondigit >> *(identifier_nondigit | digit)] - keywords
        ;

    identifier_nondigit
        =   nondigit.alias()
        ;

    nondigit
        =   qi::alpha | qi::char_('_')
        ;

    digit
        =   qi::digit
        ;

    //
    // A.1.5
    //
    constant
        =   integer_constant
        |   floating_constant
        |   enumeration_constant
        |   character_constant
        ;

    integer_constant
        =   qi::lexeme[hexadecimal_constant >> -integer_suffix]
        |   qi::lexeme[decimal_constant >> -integer_suffix]
        |   qi::lexeme[octal_constant >> -integer_suffix]
        ;

    // decimal_constant
    //     =   nonzero_digit
    //     |   decimal_constant >> digit
    //     ;
    //
    decimal_constant
        =   qi::lexeme[nonzero_digit >> *digit]
        ;

    // octal_constant
    //     =   '0'
    //     |   octal_constant >> octal_digit
    //     ;
    //
    octal_constant
        =   qi::lexeme['0' >> *octal_digit]
        ;

    // hexadecimal_constant
    //     =   hexadecimal_prefix >> hexadecimal_digit
    //     |   hexadecimal_constant >> hexadecimal_digit
    //     ;
    //
    hexadecimal_constant
        =   qi::lexeme[hexadecimal_prefix >> +hexadecimal_digit]
        ;

    hexadecimal_prefix
        =   qi::no_case[qi::string("0x")]
        ;

    nonzero_digit
        =   digit - '0'
        ;

    octal_digit
        =   digit - '8' - '9'
        ;

    hexadecimal_digit
        =   qi::xdigit
        ;

    integer_suffix
        =   qi::lexeme[unsigned_suffix >> -long_suffix]
        |   qi::lexeme[unsigned_suffix >> long_long_suffix]
        |   qi::lexeme[long_suffix >> -unsigned_suffix]
        |   qi::lexeme[long_long_suffix >> -unsigned_suffix]
        ;

    unsigned_suffix
        =   qi::no_case[qi::string("u")]
        ;

    long_suffix
        =   qi::no_case[qi::string("l")]
        ;

    long_long_suffix
        =   qi::no_case[qi::string("ll")]
        ;

    floating_constant
        =   decimal_floating_constant
        |   hexadecimal_floating_constant
        ;

    decimal_floating_constant
        =   qi::lexeme[fractional_constant >> -exponent_part >> -floating_suffix]
        |   qi::lexeme[digit_sequence >> exponent_part >> -floating_suffix]
        ;

    hexadecimal_floating_constant
        =   qi::lexeme[hexadecimal_prefix >> hexadecimal_fractional_constant >> binary_exponent_part >> -floating_suffix]
        |   qi::lexeme[hexadecimal_prefix >> hexadecimal_digit_sequence >> binary_exponent_part >> -floating_suffix]
        ;

    fractional_constant
        =   qi::lexeme[-digit_sequence >> qi::char_('.') >> digit_sequence]
        |   qi::lexeme[digit_sequence >> qi::char_('.')]
        ;

    exponent_part
        =   qi::lexeme[qi::no_case[qi::char_('e')] >> -sign >> digit_sequence]
        ;

    sign
        =   qi::char_('-') | qi::char_('+')
        ;

    // digit_sequence
    //     =   digit
    //     |   digit_sequence >> digit
    //     ;
    //
    digit_sequence
        =   qi::lexeme[+digit]
        ;

    hexadecimal_fractional_constant
        =   qi::lexeme[-hexadecimal_digit_sequence >> qi::char_('.') >> hexadecimal_digit_sequence]
        |   qi::lexeme[hexadecimal_digit_sequence >> qi::char_('.')]
        ;

    binary_exponent_part
        =   qi::lexeme[qi::no_case[qi::char_('p')]  >> -sign >> digit_sequence]
        ;

    // hexadecimal_digit_sequence
    //     =   hexadecimal_digit
    //     |   hexadecimal_digit_sequence >> hexadecimal_digit
    //     ;
    //
    hexadecimal_digit_sequence
        =   qi::lexeme[+hexadecimal_digit]
        ;

    floating_suffix
        =   qi::no_case[qi::string("f")] | qi::no_case[qi::string("l")]
        ;

    enumeration_constant
        =   identifier.alias()
        ;

    character_constant
        =   qi::lexeme[(qi::lit('\'') | "L'") >> c_char_sequence >> '\'']
        ;

    // c_char_sequence
    //     = c_char
    //     | c_char_sequence >> c_char
    //     ;
    //
    c_char_sequence
        =   qi::lexeme[+c_char]
        ;

    c_char
        =   character_set - '\'' - '\\' - new_line
        ;

    escape_sequence
        =   simple_escape_sequence
        |   octal_escape_sequence
        |   hexadecimal_escape_sequence
        ;

    simple_escape_sequence
        =   qi::string("\\'")
        |   qi::string("\\\"")
        |   qi::string("\\?")
        |   qi::string("\\\\")
        |   qi::string("\\a")
        |   qi::string("\\b")
        |   qi::string("\\f")
        |   qi::string("\\n")
        |   qi::string("\\r")
        |   qi::string("\\t")
        |   qi::string("\\v")
        ;

    octal_escape_sequence
        =   qi::lexeme[qi::string("\\") >> qi::repeat(1, 3)[octal_digit]]
        ;

    // hexadecimal_escape_sequence
    //     =   lit("\\x") >> hexadecimal_digit
    //     |   hexadecimal_escape_sequence >> hexadecimal_digit
    //     ;
    //
    hexadecimal_escape_sequence
        =   qi::lexeme[qi::string("\\x") >> +hexadecimal_digit]
        ;

    //
    // A.1.6
    //
    string_literal
        =   qi::lexeme[qi::string("\"") >> -s_char_sequence >> qi::string("\"")]
        |   qi::lexeme[qi::string("L\"") >> -s_char_sequence >> qi::string("\"")]
        ;

    // s_char_sequence
    //     =   s_char
    //     |   s_char_sequence >> s_char
    //     ;
    //
    s_char_sequence
        =   qi::lexeme[+s_char]
        ;

    s_char
        =   character_set - '"' - '\\' - new_line
        ;

    //
    // A.2.1
    //
    primary_expression
        =   identifier
        |   constant
        |   string_literal
        |   '(' >> expression >> ')'
        ;

    // eliminate left recursion
    // remove Compound literals
    //
    // postfix_expression
    //     =   primary_expression
    //     |   postfix_expression >> '[' >> expression >> ']'
    //     |   postfix_expression >> '(' >> -argument_expression_list >> ')'
    //     |   postfix_expression >> '.' >> identifier
    //     |   postfix_expression >> "->" >> identifier
    //     |   postfix_expression >> "++"
    //     |   postfix_expression >> "--"
    //     |   '(' >> type_name >> ')' >> '{' >> initializer_list >> '}'
    //     |   '(' >> type_name >> ')' >> '{' >> initializer_list >> ',' >> '}'
    //     ;
    //
    postfix_expression
        =   primary_expression >>
           *(
                '[' >> expression >> ']'
            |   '(' >> -argument_expression_list >> ')'
            |   '.' >> identifier
            |   "->" >> identifier
            |   "++"
            |   "--"
            )
        ;

    // argument_expression_list
    //     =   assignment_expression
    //     |   argument_expression_list >> ',' >> assignment_expression
    //     ;
    //
    argument_expression_list
        =   assignment_expression % ','
        ;

    unary_expression
        =   postfix_expression
        |   (("++" >> qi::attr(Operator::pre_inc)) | ("--" >> qi::attr(Operator::pre_dec))) >> unary_expression
        |   unary_operator >> cast_expression
        |   "sizeof" >> qi::omit[qi::no_skip[+qi::space]] >> qi::attr(Operator::size_of) >> unary_expression
        |   qi::lit("sizeof") >> '(' >> type_name >> ')'
        ;

    unary_operator.add
        ("&", Operator::address_of)
        ("*", Operator::dereference)
        ("+", Operator::unary_plus)
        ("-", Operator::negate)
        ("~", Operator::complement)
        ("!", Operator::logical_not)
        ;

    cast_expression
        =   *('(' >> type_name >> ')') >> unary_expression
        ;

    // multiplicative_expression
    //     =   cast_expression
    //     |   multiplicative_expression >> '*' >> cast_expression
    //     |   multiplicative_expression >> '/' >> cast_expression
    //     |   multiplicative_expression >> '%' >> cast_expression
    //     ;
    //
    multiplicative_expression
        =   cast_expression >> *(multiplicative_operator >> cast_expression)
        ;

    // additive_expression
    //     =   multiplicative_expression
    //     |   additive_expression >> '+' >> multiplicative_expression
    //     |   additive_expression >> '-' >> multiplicative_expression
    //     ;
    //
    additive_expression
        =   multiplicative_expression >> *(additive_operator >> multiplicative_expression)
        ;

    // shift_expression
    //     =   additive_expression
    //     |   shift_expression >> "<<" >> additive_expression
    //     |   shift_expression >> ">>" >> additive_expression
    //     ;
    //
    shift_expression
        =   additive_expression >> *(shift_operator >> additive_expression)
        ;

    // relational_expression
    //     =   shift_expression
    //     |   relational_expression >> "<=" >> shift_expression
    //     |   relational_expression >> ">=" >> shift_expression
    //     |   relational_expression >> "<" >> shift_expression
    //     |   relational_expression >> ">" >> shift_expression
    //     ;
    //
    relational_expression
        =   shift_expression >> *(relational_operator >> shift_expression)
        ;

    // eliminate left recursion
    //
    // equality_expression
    //     =   relational_expression
    //     |   equality_expression >> "==" >> relational_expression
    //     |   equality_expression >> "!=" >> relational_expression
    //     ;
    //
    equality_expression
        =   relational_expression >> *(equality_operator >> relational_expression)
        ;

    // AND_expression
    //     =   equality_expression
    //     |   AND_expression >> '&' >> equality_expression
    //     ;
    //
    AND_expression
        =   equality_expression >> *(AND_operator >> equality_expression)
        ;

    // exclusive_OR_expression
    //     =   AND_expression
    //     |   exclusive_OR_expression >> '^' >> AND_expression
    //     ;
    //
    exclusive_OR_expression
        =   AND_expression >> *(exclusive_OR_operator >> AND_expression)
        ;

    // inclusive_OR_expression
    //     =   exclusive_OR_expression
    //     |   inclusive_OR_expression >> '|' >> exclusive_OR_expression
    //     ;
    //
    inclusive_OR_expression
        =   exclusive_OR_expression >> *(inclusive_OR_operator >> exclusive_OR_expression)
        ;

    // logical_AND_expression
    //     =   inclusive_OR_expression
    //     |   logical_AND_expression >> "&&" >> inclusive_OR_expression
    //     ;
    //
    logical_AND_expression
        =   inclusive_OR_expression >> *(logical_AND_operator >> inclusive_OR_expression)
        ;

    // logical_OR_expression
    //     =   logical_AND_expression
    //     |   logical_OR_expression >> "||" >> logical_AND_expression
    //     ;
    //
    logical_OR_expression
        =   logical_AND_expression >> *(logical_OR_operator >> logical_AND_expression)
        ;

    conditional_expression
        =   logical_OR_expression >> -('?' >> expression >> ':' >> conditional_expression)
        ;

    assignment_expression
        =   *(unary_expression >> assignment_operator) >> conditional_expression
        ;

    assignment_operator.add
        ("=", Operator::assign)
        ("*=", Operator::multiplies_assign)
        ("/=", Operator::divides_assign)
        ("%=", Operator::modulus_assign)
        ("+=", Operator::plus_assign)
        ("-=", Operator::minus_assign)
        ("<<=", Operator::shift_left_assign)
        (">>=", Operator::shift_right_assign)
        ("&=", Operator::bitwise_and_assign)
        ("^=", Operator::bitwise_xor_assign)
        ("|=", Operator::bitwise_or_assign)
        ;

    // expression
    //     =   assignment_expression
    //     |   expression >> ',' >> assignment_expression
    //     ;
    //
    expression
        =   assignment_expression % ','
        ;

    constant_expression
        =   conditional_expression.alias()
        ;

    //
    // A.2.2
    //
    declaration
        =   declaration_specifiers >> -init_declarator_list >> ';'
        ;

    // declaration_specifiers
    //     =   qi::lexeme[qi::string("typedef") >> -declaration_specifiers
    //     |   storage_class_specifier >> -declaration_specifiers
    //     |   type_specifier >> -declaration_specifiers
    //     |   type_qualifier >> -declaration_specifiers
    //     |   function_specifier >> -declaration_specifiers
    //     ;
    //
    declaration_specifiers
        =  +(   qi::string("typedef") >> qi::omit[qi::no_skip[+qi::space]]
            |   storage_class_specifier
            |   type_specifier
            |   type_qualifier
            |   function_specifier
            )
        ;

    // init_declarator_list
    //     =   init_declarator
    //     |   init_declarator_list >> ',' >> init_declarator
    //     ;
    //
    init_declarator_list
        =   init_declarator % ','
        ;

    init_declarator
        =   declarator >> -('=' >> initializer)
        ;

    storage_class_specifier
        =   qi::string("extern") >> qi::omit[qi::no_skip[+qi::space]]
        |   qi::string("static") >> qi::omit[qi::no_skip[+qi::space]]
        |   qi::string("auto") >> qi::omit[qi::no_skip[+qi::space]]
        |   qi::string("register") >> qi::omit[qi::no_skip[+qi::space]]
        ;


    auto validate_typedef_name =
        [this](std::string& tdn, decltype(typedef_name)::context_type& context, bool& pass) {
            pass = (typedef_table.find(tdn) != nullptr);
        };
    type_specifier
        =   qi::string("void")
        |   qi::string("char")
        |   qi::string("short")
        |   qi::string("int")
        |   qi::string("long")
        |   qi::string("float")
        |   qi::string("long")
        |   qi::string("double")
        |   qi::string("signed")
        |   qi::string("unsigned")
        |   struct_or_union_specifier
        |   enum_specifier
        |   typedef_name[validate_typedef_name]
        ;

    struct_or_union_specifier
        =   struct_or_union >> -identifier >> '{' >> struct_declaration_list >> '}'
        |   struct_or_union >> identifier
        ;

    struct_or_union
        =   qi::string("struct") >> qi::omit[qi::no_skip[+qi::space]]
        |   qi::string("union") >> qi::omit[qi::no_skip[+qi::space]]
        ;

    // struct_declaration_list
    //     =   struct_declaration
    //     |   struct_declaration_list >> struct_declaration
    //     ;
    //
    struct_declaration_list
        =   +struct_declaration
        ;

    struct_declaration
        =   specifier_qualifier_list >> struct_declarator_list >> ';'
        ;

    specifier_qualifier_list
        =   type_specifier >> -specifier_qualifier_list
        |   type_qualifier >> -specifier_qualifier_list
        ;

    // struct_declarator_list
    //     =   struct_declarator
    //     |   struct_declarator_list >> ',' >> struct_declarator
    //     ;
    //
    struct_declarator_list
        =   struct_declarator % ','
        ;

    struct_declarator
        =   -declarator >> ':' >> constant_expression   
        |   declarator
        ;

    enum_specifier
        =   qi::string("enum") >> -identifier >> '{' >> enumerator_list >> '}'
        |   qi::string("enum") >> -identifier >> '{' >> enumerator_list >> ',' >> '}'
        |   qi::string("enum") >> identifier
        ;

    // enumerator_list
    //     =   enumerator
    //     |   enumerator_list >> ',' >> enumerator
    //     ;
    //
    enumerator_list
        =   enumerator % ','
        ;

    enumerator
        =   enumeration_constant >> '=' >> constant_expression
        |   enumeration_constant
        ;

    type_qualifier
        =   qi::string("const") >> qi::omit[qi::no_skip[+qi::space]]
        |   qi::string("restrict") >> qi::omit[qi::no_skip[+qi::space]]
        |   qi::string("volatile") >> qi::omit[qi::no_skip[+qi::space]]
        ;

    function_specifier
        =   qi::string("inline") >> qi::omit[qi::no_skip[+qi::space]]
        ;

    declarator
        =   -pointer >> direct_declarator
        ;

    // eliminate left recursion
    //
    // direct_declarator
    //     =   identifier
    //     |   '(' >> declarator >> ')'
    //     |   direct_declarator >> '[' >> -type_qualifier_list >> -assignment_expression >> ']'
    //     |   direct_declarator >> '[' >> "static" >> -type_qualifier_list >> assignment_expression >> ']'
    //     |   direct_declarator >> '[' >> type_qualifier_list >> "static" >> assignment_expression >> ']'
    //     |   direct_declarator >> '[' >> -type_qualifier_list >> '*' >> ']'
    //     |   direct_declarator >> '(' >> parameter_type_list >> ')'
    //     |   direct_declarator >> '(' >> -identifier_list >> ')'
    //     ;
    //
    direct_declarator
        =   identifier >> direct_declarator_helper
        |   '(' >> declarator >> ')' >> direct_declarator_helper
        ;
    direct_declarator_helper
        =   '[' >> -type_qualifier_list >> -assignment_expression >> ']' >> direct_declarator_helper
        |   '[' >> qi::lit("static") >> qi::omit[qi::no_skip[+qi::space]] >> -type_qualifier_list >> assignment_expression >> ']' >> direct_declarator_helper
        |   '[' >> type_qualifier_list >> "static" >> qi::omit[qi::no_skip[+qi::space]] >> assignment_expression >> ']' >> direct_declarator_helper
        |   '[' >> -type_qualifier_list >> '*' >> ']' >> direct_declarator_helper
        |   '(' >> parameter_type_list >> ')' >> direct_declarator_helper
        |   '(' >> -identifier_list >> ')' >> direct_declarator_helper
        |   qi::eps
        ;

    // pointer
    //     =   '*' >> -type_qualifier_list >> pointer
    //     |   '*' >> -type_qualifier_list
    //     ;
    //
    pointer
        =   +('*' >> -type_qualifier_list)
        ;

    // type_qualifier_list
    //     =   type_qualifier
    //     |   type_qualifier_list >> type_qualifier
    //     ;
    //
    type_qualifier_list
        =   +type_qualifier
        ;

    parameter_type_list
        =   parameter_list >> -(qi::lit(',') >> "...")
        ;

    // parameter_list
    //     =   parameter_declaration
    //     |   parameter_list >> ',' >> parameter_declaration
    //     ;
    //
    parameter_list
        =   parameter_declaration % ','
        ;

    parameter_declaration
        =   declaration_specifiers >> (declarator | -abstract_declarator)
        ;

    // identifier_list
    //     =   identifier
    //     |   identifier_list >> ',' >> identifier
    //     ;
    //
    identifier_list
        =   identifier % ','
        ;

    type_name
        =   specifier_qualifier_list >> -abstract_declarator
        ;

    abstract_declarator
        =   -pointer >> direct_abstract_declarator
        |   pointer
        ;

    // eliminate left recursion
    //
    // direct_abstract_declarator
    //     =   '(' >> abstract_declarator >> ')'
    //     |   -direct_abstract_declarator >> '[' >> -type_qualifier_list >> -assignment_expression >> ']'
    //     |   -direct_abstract_declarator >> '[' >> "static" >> -type_qualifier_list >> assignment_expression >> ']'
    //     |   -direct_abstract_declarator >> '[' >> type_qualifier_list >> "static" >> assignment_expression >> ']'
    //     |   -direct_abstract_declarator >> '[' >> '*' >> ']'
    //     |   -direct_abstract_declarator >> '(' >> -parameter_type_list >> ')'
    //     ;
    //
    direct_abstract_declarator
        =  -(   '(' >> abstract_declarator >> ')'
            )
        |  *(   '[' >> -type_qualifier_list >> -assignment_expression >> ']'
            |   '[' >> qi::lit("static") >> qi::omit[qi::no_skip[+qi::space]] >> -type_qualifier_list >> assignment_expression >> ']'
            |   '[' >> type_qualifier_list >> "static" >> qi::omit[qi::no_skip[+qi::space]] >> assignment_expression >> ']'
            |   '[' >> qi::lit('*') >> ']'
            |   '(' >> -parameter_type_list >> ')'
            )
        ;

    typedef_name
        =   identifier.alias()
        ;

    initializer
        =   assignment_expression
        |   '{' >> initializer_list >> '}'
        |   '{' >> initializer_list >> ',' >> '}'
        ;

    // initializer_list
    //     =   -designation >> initializer
    //     |   initializer_list >> ',' >> -designation >> initializer
    //     ;
    //
    initializer_list
        =   (-designation >> initializer) % ','
        ;

    designation
        =   designator_list >> '='
        ;

    // designator_list
    //     =   designator
    //     |   designator_list >> designator
    //     ;
    //
    designator_list
        =   +designator
        ;

    designator
        =   '[' >> constant_expression >> ']'
        |   '.' >> identifier
        ;

    //
    // A.2.3
    //
    statement
        =   labeled_statement
        |   compound_statement
        |   expression_statement
        |   selection_statement
        |   iteration_statement
        |   jump_statement
        ;

    labeled_statement
        =   identifier >> ':' >> statement
        |   "case" >> qi::omit[qi::no_skip[+qi::space]] >> constant_expression >> ':' >> statement
        |   "default" >> qi::omit[qi::no_skip[+qi::space]] >> ':' >> statement
        ;

    compound_statement
        =   '{' >> -block_item_list >> '}'
        ;

    // block_item_list
    //     =   block_item
    //     |   block_item_list >> block_item
    //     ;
    //
    block_item_list
        =   +block_item
        ;

    block_item
        =   declaration
        |   statement
        ;

    expression_statement
        =   -expression >> ';'
        ;

    selection_statement
        =   qi::lit("if") >> '(' >> expression >> ')' >> statement
        |   qi::lit("if") >> '(' >> expression >> ')' >> statement >> "else" >> statement
        |   qi::lit("switch") >> '(' >> expression >> ')' >> statement
        ;

    iteration_statement
        =   qi::lit("while") >> '(' >> expression >> ')' >> statement
        |   "do" >> qi::omit[qi::no_skip[+qi::space]] >> statement >> qi::omit[qi::no_skip[+qi::space]] >> "while" >> '(' >> expression >> ')' >> ';'
        |   qi::lit("for") >> '(' >> -expression >> ';' >> -expression >> ';' >> -expression >> ')' >> statement
        |   qi::lit("for") >> '(' >> declaration >> -expression >> ';' >> -expression >> ')' >> statement
        ;

    jump_statement
        =   "goto" >> qi::omit[qi::no_skip[+qi::space]] >> identifier >> ';'
        |   "continue" >> qi::lit(';')
        |   "break" >> qi::lit(';')
        |   "return" >> qi::omit[qi::no_skip[+qi::space]] >> -expression >> ';'
        ;

    //
    // A.2.4
    //

    // translation_unit
    //     =   external_declaration
    //     |   translation_unit >> external_declaration
    //     ;
    //
    translation_unit
        =   +external_declaration
        ;

    external_declaration
        =   function_definition
        |   declaration
        ;

    function_definition
        =   declaration_specifiers >> declarator >> -declaration_list >> compound_statement
        ;

    // declaration_list
    //     =   declaration
    //     |   declaration_list >> declaration
    //     ;
    //
    declaration_list
        =   +declaration
        ;

    new_line
        =   qi::eol
        ;

    //
    // enable debug
    //
    BOOST_SPIRIT_DEBUG_NODES(
        (character_set)
        (identifier)
        (identifier_nondigit)
        (digit)
        (nondigit)
        (hexadecimal_digit)
        (constant)
        (integer_constant)
        (floating_constant)
        (enumeration_constant)
        (character_constant)
        (decimal_constant)
        (integer_suffix)
        (octal_constant)
        (hexadecimal_constant)
        (nonzero_digit)
        (octal_digit)
        (hexadecimal_prefix)
        (unsigned_suffix)
        (long_suffix)
        (long_long_suffix)
        (decimal_floating_constant)
        (hexadecimal_floating_constant)
        (fractional_constant)
        (exponent_part)
        (floating_suffix)
        (digit_sequence)
        (hexadecimal_fractional_constant)
        (binary_exponent_part)
        (hexadecimal_digit_sequence)
        (sign)
        (c_char_sequence)
        (c_char)
        (escape_sequence)
        (simple_escape_sequence)
        (octal_escape_sequence)
        (hexadecimal_escape_sequence)
        (string_literal)
        (encoding_prefix)
        (s_char_sequence)
        (s_char)
        (primary_expression)
        (expression)
        (generic_selection)
        (assignment_expression)
        (generic_assoc_list)
        (generic_association)
        (type_name)
        (postfix_expression)
        (postfix_expression_helper)
        (argument_expression_list)
        (initializer_list)
        (unary_expression)
        (cast_expression)
        (multiplicative_expression)
        (additive_expression)
        (shift_expression)
        (relational_expression)
        (equality_expression)
        (AND_expression)
        (exclusive_OR_expression)
        (inclusive_OR_expression)
        (logical_AND_expression)
        (logical_OR_expression)
        (conditional_expression)
        (constant_expression)
        (declaration)
        (declaration_specifiers)
        (init_declarator_list)
        (static_assert_declaration)
        (storage_class_specifier)
        (type_specifier)
        (type_qualifier)
        (function_specifier)
        (alignment_specifier)
        (init_declarator)
        (declarator)
        (initializer)
        (atomic_type_specifier)
        (struct_or_union_specifier)
        (enum_specifier)
        (typedef_name)
        (struct_or_union)
        (struct_declaration_list)
        (struct_declaration)
        (specifier_qualifier_list)
        (struct_declarator_list)
        (struct_declarator)
        (enumerator_list)
        (enumerator)
        (pointer)
        (direct_declarator)
        (direct_declarator_helper)
        (type_qualifier_list)
        (parameter_type_list)
        (identifier_list)
        (parameter_list)
        (parameter_declaration)
        (abstract_declarator)
        (direct_abstract_declarator)
        (direct_abstract_declarator_helper)
        (designation)
        (designator_list)
        (designator)
        (statement)
        (labeled_statement)
        (compound_statement)
        (expression_statement)
        (selection_statement)
        (iteration_statement)
        (jump_statement)
        (block_item_list)
        (block_item)
        (translation_unit)
        (external_declaration)
        (function_definition)
        (declaration_list)
        (new_line)
    );
}


template<typename Iterator, typename Signature, typename Skipper>
c_grammar<Iterator, Signature, Skipper>::~c_grammar() = default;


#endif // GRAMMAR_CPP_HPP
