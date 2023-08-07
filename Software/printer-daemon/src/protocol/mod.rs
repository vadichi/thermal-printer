use serde::{Deserialize, Serialize};

#[derive(Deserialize, Serialize)]
struct RequestHeader {
    command: DaemonCommands,
}

#[derive(Deserialize, Serialize)]
enum DaemonCommands {}
