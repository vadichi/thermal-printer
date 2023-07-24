import {Component, OnInit} from '@angular/core';
import {ConnectionStatus, WebSocketClientService} from "../web-socket-client.service";
import {MatDialog} from "@angular/material/dialog";
import {CustomDialogueComponent} from "../custom-dialogue/custom-dialogue.component";

@Component({
    selector: 'app-control-panel',
    templateUrl: './control-panel.component.html',
    styleUrls: ['./control-panel.component.scss']
})
export class ControlPanelComponent implements OnInit {

    private connection: WebSocketClientService = new WebSocketClientService();

    protected controls_enabled: boolean = true;
    protected connection_action_in_progress: boolean = false;
    protected grid_list_columns: number = 3;

    constructor(private dialogue: MatDialog) {}

    ngOnInit(): void {
        this.update_grid_list();

        window.addEventListener('resize', () => {
            this.update_grid_list();
        });
    }

    protected get connection_status(): string {
        let status = this.connection.connection_status;

        switch (status) {
            case ConnectionStatus.NOT_CONNECTED:
                return "not connected";

            case ConnectionStatus.CONNECTING:
                return "connecting...";

            case ConnectionStatus.CONNECTED:
                return "connected";

            case ConnectionStatus.DISCONNECTING:
                return "disconnecting...";

            case ConnectionStatus.DISCONNECTED:
                return "disconnected";
        }
    }

    protected get connect_button_enabled(): boolean {
        let status = this.connection.connection_status;

        return status === ConnectionStatus.NOT_CONNECTED ||
            status === ConnectionStatus.DISCONNECTED;
    }

    protected get disconnect_button_enabled(): boolean {
        let status = this.connection.connection_status;

        return status === ConnectionStatus.CONNECTING ||
            status === ConnectionStatus.CONNECTED ||
            status === ConnectionStatus.DISCONNECTING;
    }

    protected async connect(): Promise<void> {
        this.controls_enabled = false;
        this.connection_action_in_progress = true;

        let success: boolean = false;
        try {
            success = await this.connection.connect("green-frost-printer.local:8080");
        } catch (exception) {
            if (exception instanceof SyntaxError) {
                this.show_dialogue("Oops!", "Please enter a valid hostname.");
            }
        }

        if (!success) {
            this.show_dialogue("Oops!", "Failed to connect to printer.");
        }

        this.controls_enabled = true;
        this.connection_action_in_progress = false;
    }

    protected async disconnect(): Promise<void> {
        this.connection_action_in_progress = true;
        this.controls_enabled = false;

        let success: boolean = await this.connection.disconnect();

        if (!success) {
            this.show_dialogue("Oops!", "Already disconnected.");
        }

        this.controls_enabled = true;
        this.connection_action_in_progress = false;
    }

    protected async scroll(lines_input: string): Promise<void> {
        this.controls_enabled = false;

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

        this.controls_enabled = true;
    }

    protected info_menu_open_github(): void {
        window.open('https://github.com/vadichi/Arduino-Printer', '_blank');
    }

    private show_dialogue(title: string, text: string): void {
        this.dialogue.open(CustomDialogueComponent, {data: {title: title, text: text}});
    }

    private update_grid_list(): void {
        let width: number = window.innerWidth;

        if (width <= 640) {
            this.grid_list_columns = 1;
        } else if (width <= 640 * 2) {
            this.grid_list_columns = 2;
        } else {
            this.grid_list_columns = 3;
        }
    }

}
