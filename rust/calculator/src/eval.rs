use crate::ast::{Expr, Operator};

pub fn evaluate(expr: &Expr) -> i32 {
    match expr {
        Expr::Number(num) => *num,
        Expr::Op(l, op, r) => match op {
            Operator::Mul => evaluate(l) * evaluate(r),
            Operator::Div => evaluate(l) / evaluate(r),
            Operator::Add => evaluate(l) + evaluate(r),
            Operator::Sub => evaluate(l) - evaluate(r),
        },
        Expr::Error => panic!("please check erroneous expression"),
    }
}
