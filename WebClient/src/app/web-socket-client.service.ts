import {Injectable} from '@angular/core';

export enum ConnectionStatus {
    NOT_CONNECTED,
    CONNECTING,
    OPEN,
    CLOSING,
    CLOSED,
}

@Injectable({
    providedIn: 'root'
})
export class WebSocketClientService {
    socket: WebSocket | null = null;

    decoder: TextDecoder = new TextDecoder();

    get connection_status(): ConnectionStatus {
        if (this.socket == null) {
            return ConnectionStatus.NOT_CONNECTED;
        }

        switch (this.socket.readyState) {
            case WebSocket.CONNECTING:
                return ConnectionStatus.CONNECTING;
            case WebSocket.OPEN:
                return ConnectionStatus.OPEN;
            case WebSocket.CLOSING:
                return ConnectionStatus.CLOSING;
            case WebSocket.CLOSED:
                return ConnectionStatus.CLOSED;
            default:
                return ConnectionStatus.NOT_CONNECTED;
        }
    }

    async connect(server_ip: string): Promise<boolean> {
        this.socket = new WebSocket("ws://" + server_ip);
        this.socket.binaryType = 'arraybuffer';

        let await_connection: Promise<void> = new Promise<void>((resolve, reject): void => {
            let timeout = setTimeout(() => {
                reject()
            }, 5000);

            this.socket?.addEventListener("open", () => {
                clearTimeout(timeout);
                resolve();
            });
        });

        return await await_connection.then((): boolean => {
            return true;
        }).catch((): boolean => {
            return false;
        });
    }

    async disconnect(): Promise<boolean> {
        if (this.socket == null) return false;

        this.socket?.close();
        this.socket = null;

        return true;
    }

    async send_scroll_command(lines: number): Promise<boolean> {
        if ((this.socket == null) || (this.socket.readyState != WebSocket.OPEN)) return false;

        let command: number[] = [];

        command.push("L".charCodeAt(0));
        command.push(lines);

        this.socket?.send(new Uint8Array(command));

        let await_response: Promise<void> = new Promise<void>((resolve, reject): void => {
            let timeout = setTimeout(() => {
                reject()
            }, 5000);

            this.socket?.addEventListener("message", (event: MessageEvent) => {
                clearTimeout(timeout);
                let response: string = this.decoder.decode(event.data);

                if (response == "S") {
                    resolve();
                } else {
                    reject();
                }
            });
        });

        return await await_response.then((): boolean => true).catch((): boolean => false);
    }

    async send_print_command(dots: boolean[], enable_bold_font: boolean): Promise<boolean> {
        if ((this.socket == null) || (this.socket.readyState != WebSocket.OPEN)) return false;

        let command: number[] = [];

        command.push("P".charCodeAt(0));
        command.push((enable_bold_font ? "B" : "N").charCodeAt(0));
        command.push(...this.compress_bit_array(dots));

        this.socket?.send(new Uint8Array(command));

        let await_response: Promise<void> = new Promise<void>((resolve, reject): void => {
            let timeout = setTimeout(() => {
                reject()
            }, 5000);

            this.socket?.addEventListener("message", (event: MessageEvent) => {
                clearTimeout(timeout);
                let response: string = this.decoder.decode(event.data);

                if (response == "P") {
                    resolve();
                } else {
                    reject();
                }
            });
        });

        return await await_response.then((): boolean => true).catch((): boolean => false);
    }

    private compress_bit_array(dots: boolean[]): number[] {
        let bytes: number[] = Array(Math.ceil(dots.length / 8)).fill(0);

        for (let dot: number = 0; dot < dots.length; dot++) {
            let byte_index: number = Math.floor(dot / 8);
            let bit_index: number = dot % 8;

            if (dots[dot]) {
                let mask: number = 0b00000001 << bit_index;
                bytes[byte_index] |= mask;
            }
        }

        return bytes;
    }

}
