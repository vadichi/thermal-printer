import {Component} from '@angular/core';
import {ConnectionStatus, WebSocketClientService} from "../web-socket-client.service";
import {MatDialog} from "@angular/material/dialog";
import {CustomDialogueComponent} from "../custom-dialogue/custom-dialogue.component";

@Component({
    selector: 'app-control-panel',
    templateUrl: './control-panel.component.html',
    styleUrls: ['./control-panel.component.scss']
})
export class ControlPanelComponent {

    private connection: WebSocketClientService = new WebSocketClientService();

    protected controls_enabled: boolean = true;

    constructor(private dialogue: MatDialog) {}

    protected get connection_status(): string {
        let status = this.connection?.connection_status;

        switch (status) {
            case ConnectionStatus.NOT_CONNECTED:
                return "not connected";

            case ConnectionStatus.CONNECTING:
                return "connecting...";

            case ConnectionStatus.OPEN:
                return "connected";

            case ConnectionStatus.CLOSING:
                return "disconnecting...";

            case ConnectionStatus.CLOSED:
                return "disconnected";
        }
    }

    protected async connect(): Promise<void> {
        this.controls_enabled = false;

        let success: boolean = false;
        try {
            success = await this.connection.connect("green-frost-printer.local:8080");
        } catch (exception) {
            if (exception instanceof SyntaxError) {
                this.show_dialogue("Oops!", "Please enter a valid hostname.");
            }
        }

        if (!success) {
            this.show_dialogue("Oops!", "Failed to connect to printer.")
        }

        this.controls_enabled = true;
    }

    protected async disconnect(): Promise<void> {
        this.controls_enabled = false;

        let success: boolean = await this.connection.disconnect();

        if (!success) {
            this.show_dialogue("Oops!", "Already disconnected.");
        }

        this.controls_enabled = true;
    }

    protected async scroll(lines_input: string): Promise<void> {
        let lines: number = parseInt(lines_input);

        if (Number.isNaN(lines)) {
            this.show_dialogue("Oops!", "Please enter a valid number of lines to scroll.");
            return;
        }

        let result: boolean = await this.connection.send_scroll_command(lines);

        if (result) {
            this.show_dialogue("Success!", "Command sent successfully.");
        }
        else {
            this.show_dialogue("Oops!", "Failed to send command to printer.");
        }
    }

    private show_dialogue(title: string, text: string): void {
        this.dialogue.open(CustomDialogueComponent, {data: {title: title, text: text}});
    }

}
