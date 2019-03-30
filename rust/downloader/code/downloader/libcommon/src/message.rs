use chrono::{DateTime, Local};
use postgres::to_sql_checked;
use postgres_derive::{FromSql, ToSql};
use serde::{Deserialize, Serialize};

// #[derive(Debug, ToSql, FromSql, Deserialize, Serialize, Clone)]
// #[postgres(name = "inserttype")]
// pub enum InsertType {
//     #[postgres(name = "new")]
//     New,

//     #[postgres(name = "retry")]
//     Retry,
// }

#[derive(Debug, ToSql, FromSql, Clone)]
#[postgres(name = "endtype")]
pub enum EndType {
    #[postgres(name = "success")]
    Success,

    #[postgres(name = "timeout")]
    Timeout,
}

#[derive(Debug, ToSql, FromSql, Deserialize, Serialize, Clone)]
#[postgres(name = "urltype")]
pub enum UrlType {
    #[postgres(name = "html")]
    Html,

    #[postgres(name = "media")]
    Media,
}

impl std::fmt::Display for UrlType {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            UrlType::Html => write!(f, "html"),
            UrlType::Media => write!(f, "media"),
        }
    }
}

#[derive(Debug, ToSql, FromSql, Deserialize, Serialize, Clone)]
#[postgres(name = "hashtype")]
pub enum HashType {
    #[postgres(name = "sha2-256")]
    Sha2256,

    #[postgres(name = "sha2-512")]
    Sha2512,

    #[postgres(name = "sha3-256")]
    Sha3256,

    #[postgres(name = "sha3-512")]
    Sha3512,
}

#[derive(Serialize, Deserialize, Debug)]
pub struct WorkspaceCreationRequest {
    pub name: String,
}

#[derive(Serialize, Deserialize, Debug)]
pub struct WorkspaceCreationResponse {
    pub id: i64,
}

#[derive(Serialize, Deserialize, Debug)]
pub struct WorkspaceDeletionRequest {
    pub name: String,
}

#[derive(Serialize, Deserialize, Debug)]
pub struct DownloadStart {
    pub urljobid: i64,
    pub dstarttime: DateTime<Local>,
}

#[derive(Serialize, Deserialize, Debug)]
pub struct HtmlDownloadResult {
    pub htmlurls: Vec<String>,
    pub mediaurls: Vec<String>,
}

#[derive(Serialize, Deserialize, Debug)]
pub struct MediaDownloadResult {
    pub filename: String,
    pub filesize: i64, /* >0 */
    pub filehash: String,
    pub hashtype: HashType,
    pub mediawidth: i32,  /* >0 */
    pub mediaheight: i32, /* >0 */
}

#[derive(Serialize, Deserialize, Debug)]
pub enum DownloadResult {
    Error,
    Html(HtmlDownloadResult),
    Media(MediaDownloadResult),
}

#[derive(Serialize, Deserialize, Debug)]
pub struct DownloadEnd {
    pub urljobid: i64,
    pub dstoptime: DateTime<Local>,
    pub comment: String,
    pub result: DownloadResult,
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct UrlDownloadRequest {
    pub urljobid: i64,
    pub url: String,
}
