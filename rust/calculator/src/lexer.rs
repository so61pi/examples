use std::iter::Peekable;
use std::str::CharIndices;
use std::str::FromStr;

#[derive(Copy, Clone, Debug)]
pub enum LexicalError {
    InvalidChar(usize, char),
}

#[derive(Copy, Clone, Debug)]
pub enum Tok {
    OParen,
    CParen,
    Mul,
    Div,
    Add,
    Sub,
    Num(i32),
}

pub struct Lexer<'input> {
    input: &'input str,
    chars: Peekable<CharIndices<'input>>,
}

impl<'input> Lexer<'input> {
    pub fn new(input: &'input str) -> Self {
        Lexer {
            input: input,
            chars: input.char_indices().peekable(),
        }
    }
}

impl<'input> Iterator for Lexer<'input> {
    // Result<(tok-begin-loc, tok, tok-end-loc), lex-error>
    type Item = Result<(usize, Tok, usize), LexicalError>;

    fn next(&mut self) -> Option<Self::Item> {
        // Skip white space and comment.
        if let Some((i, _c)) = self.chars.peek() {
            let re = regex::Regex::new(r"^([[:space:]]|(/\*.*?\*/))+").unwrap();
            if let Some(m) = re.find(&self.input[*i..]) {
                assert_eq!(0, m.start());
                for _ in 0..m.end() {
                    self.chars.next();
                }
            }
        }

        if let Some((i, c)) = self.chars.next() {
            match c {
                '(' => return Some(Ok((i, Tok::OParen, i + 1))),
                ')' => return Some(Ok((i, Tok::CParen, i + 1))),
                '*' => return Some(Ok((i, Tok::Mul, i + 1))),
                '/' => return Some(Ok((i, Tok::Div, i + 1))),
                '+' => return Some(Ok((i, Tok::Add, i + 1))),
                '-' => return Some(Ok((i, Tok::Sub, i + 1))),
                _ if c.is_digit(10) => {
                    let skipped = skip_peek(&mut self.chars, |(_i, c)| c.is_digit(10));
                    let e = i + skipped + 1;
                    let num = i32::from_str(&self.input[i..e]).unwrap();
                    return Some(Ok((i, Tok::Num(num), e)));
                }
                _ => {
                    return Some(Err(LexicalError::InvalidChar(i, c)));
                }
            }
        }
        return None;
    }
}

fn skip_peek<I, F>(it: &mut Peekable<I>, f: F) -> usize
where
    I: std::iter::Iterator,
    F: Fn(&I::Item) -> bool,
{
    let mut count = 0;
    while let Some(v) = it.peek() {
        if f(v) {
            count += 1;
            it.next();
        } else {
            break;
        }
    }
    return count;
}
