/// DBTrait for common interface.
pub trait DBTrait {
    fn get_username(&mut self, id: u64) -> String;
}

/// DBReal is the real implementation of DBTrait.
pub struct DBReal {}

impl DBReal {
    pub fn new() -> DBReal {
        DBReal{}
    }
}

impl DBTrait for DBReal {
    fn get_username(&mut self, id: u64) -> String {
        id.to_string()
    }
}

/// Logic code, which uses DBReal as default implementation for DBTrait.
pub struct Logic<DB: DBTrait = DBReal> {
    db: DB,
}

impl<DB: DBTrait> Logic<DB> {
    pub fn new(db: DB) -> Self {
        Logic{
            db: db,
        }
    }

    pub fn is_root(&mut self, id: u64) -> bool {
        self.db.get_username(id) == "root"
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    // Create a fake implementation for DBTrait.
    mod mock {
        use super::DBTrait;

        // This implementation is supposed to be used only in tests module.
        pub(super) struct DBMock {}

        impl DBMock {
            pub fn new() -> DBMock {
                DBMock{}
            }
        }

        impl DBTrait for DBMock {
            fn get_username(&mut self, _id: u64) -> String {
                String::from("root")
            }
        }
    }

    #[test]
    fn test() {
        let db = mock::DBMock::new();
        let mut logic = Logic::new(db);

        assert!(logic.is_root(0));
        assert!(logic.is_root(1));
    }
}
