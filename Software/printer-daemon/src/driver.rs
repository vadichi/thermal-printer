use futures::stream::{SplitSink, SplitStream};
use futures::StreamExt;
use tokio::net::TcpStream;
use tokio_tungstenite::{MaybeTlsStream, WebSocketStream};
use tungstenite::{Error, Message};

use crate::config;

struct Driver {
    rx_stream: SplitStream<WebSocketStream<MaybeTlsStream<TcpStream>>>,
    tx_sink: SplitSink<WebSocketStream<MaybeTlsStream<TcpStream>>, Message>,
}

impl Driver {
    async fn new() -> Driver {
        let (stream, _response) =
            tokio_tungstenite::connect_async(config::PRINTER_WEBSOCKET)
            .await
            .expect("Failed to connect to printer");

        let (tx_sink,
            rx_stream) = stream.split();

        Driver {
            rx_stream,
            tx_sink,
        }
    }

    async fn await_response(&mut self) {
        let response = self.rx_stream.next().await;
    }
}
