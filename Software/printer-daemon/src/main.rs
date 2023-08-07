use std::os::unix::net::{UnixListener, UnixStream};
use std::thread;

mod config;
mod driver;
mod protocol;

fn main() {
    let socket_path = format!(
        "{}/{}",
        config::RUNTIME_DIRECTORY_MACOS,
        config::SOCKET_FILE
    );

    let socket_listener = UnixListener::bind(socket_path).
        expect("Failed to bind to socket");

    for stream in socket_listener.incoming() {
        match stream {
            Ok(stream) => {
                thread::spawn(move || handler_main(stream));
            }
            Err(_) => {
                eprintln!("Failed to accept client");
            }
        }
    }
}

fn handler_main(stream: UnixStream) {
    println!("New client connected");
}
