use lalrpop_util::lalrpop_mod;

lalrpop_mod!(pub parser);

pub mod ast;
pub mod eval;
pub mod lexer;

pub fn compile(input: &str) {
    let mut errors = Vec::new();
    match parser::ExprParser::new().parse(&mut errors, lexer::Lexer::new(input)) {
        Ok(v) => {
            if errors.is_empty() {
                println!("{:?}", eval::evaluate(&v));
            }
            println!("{:#?}", v);
        }
        Err(e) => {
            println!("{:#?}", e);
        }
    }
    println!("{:#?}", errors);
}
