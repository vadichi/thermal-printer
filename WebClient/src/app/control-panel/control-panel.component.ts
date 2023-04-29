import {Component} from '@angular/core';
import {ConnectionStatus, WebSocketClientService} from "../web-socket-client.service";

@Component({
    selector: 'app-control-panel',
    templateUrl: './control-panel.component.html',
    styleUrls: ['./control-panel.component.scss']
})
export class ControlPanelComponent {

    connection: WebSocketClientService = new WebSocketClientService();

    get connection_status(): string {
        let status = this.connection?.connection_status;

        switch (status) {
            case ConnectionStatus.NOT_CONNECTED:
                return "Not Connected";

            case ConnectionStatus.CONNECTING:
                return "Connecting";

            case ConnectionStatus.OPEN:
                return "Connected";

            case ConnectionStatus.CLOSING:
                return "Closing";

            case ConnectionStatus.CLOSED:
                return "Closed";
        }
    }

    async connect(): Promise<void> {
        let success: boolean = false;
        try {
            success = await this.connection.connect("green-frost-printer.local:8080");
        } catch (exception) {
            if (exception instanceof SyntaxError) {
                alert("Invalid IP address");
            }
        }

        if (!success) {
            alert("Failed to connect");
        }
    }

    async disconnect(): Promise<void> {
        let success: boolean = await this.connection.disconnect();

        if (!success) {
            alert("Already disconnected");
        }
    }

    async scroll(lines_input: string): Promise<void> {
        let lines: number = parseInt(lines_input);

        if (Number.isNaN(lines)) {
            alert("Invalid number of lines");
            return;
        }

        let result: boolean = await this.connection.send_scroll_command(lines);

        if (result) {
            alert("Success");
        }
        else {
            alert("Failed to send command");
        }
    }

    async print(): Promise<void> {

    }

}
