// Expr = Expr "+" Factor
//      | Expr "-" Factor
//      | Factor
//
// Factor = Factor "*" Term
//        | Factor "/" Term
//        | Term
//
// Term = Num
//      | "(" <Expr> ")"
//
// Num = [0-9]+

#[derive(Debug)]
pub enum Expr {
    Number(i32),
    Op(Box<Expr>, Operator, Box<Expr>),
    Error,
}

#[derive(Copy, Clone, Debug)]
pub enum Operator {
    Mul,
    Div,
    Add,
    Sub,
}
