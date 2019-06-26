pub trait TA {
    fn cool(&self) {
        println!("A::cool");
    }
}

pub trait TB: TA {
    fn cool(&self) {
        println!("B::cool");
    }
}

pub struct S;

impl TA for S {}
impl TB for S {}

#[cfg(test)]
mod tests {
	use super::*;

    #[test]
	fn test_call() {
        let x = S{};

        TA::cool(&x);
        TB::cool(&x);

        (&x as &TA).cool();
        // (&x as &TB).cool(); // doesn't work, multiple `cool` found.
    }
}
