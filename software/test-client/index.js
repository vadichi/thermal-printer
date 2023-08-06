const socket = new WebSocket('ws://green-frost-printer.local:8080/');
socket.binaryType = 'arraybuffer';

const encoder = new TextEncoder();
const decoder = new TextDecoder();

socket.addEventListener('open', (event) => {
    console.log('Connection established');
})

socket.addEventListener('message', (event) => {
    console.log('Received message: ', decoder.decode(event.data));
})

function sendMessage(data) {
    socket.send(new Uint8Array(data));
}
