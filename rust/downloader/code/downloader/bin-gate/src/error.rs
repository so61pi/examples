#[derive(Debug)]
pub enum Error {
    Postgres(postgres::error::Error),
    SerdeJson(serde_json::Error),
    Other(failure::Error),
}

impl From<postgres::error::Error> for Error {
    fn from(e: postgres::error::Error) -> Self {
        Error::Postgres(e)
    }
}

impl From<serde_json::Error> for Error {
    fn from(e: serde_json::Error) -> Self {
        Error::SerdeJson(e)
    }
}

impl From<failure::Error> for Error {
    fn from(e: failure::Error) -> Self {
        Error::Other(e)
    }
}

impl std::fmt::Display for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            Error::Postgres(e) => e.fmt(f),
            Error::SerdeJson(e) => e.fmt(f),
            Error::Other(e) => e.fmt(f),
        }
    }
}

impl std::error::Error for Error {}
