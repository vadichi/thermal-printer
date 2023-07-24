import {Component, Inject} from '@angular/core';
import {MAT_DIALOG_DATA} from "@angular/material/dialog";

export interface CustomDialogueOptions {
    title: string;
    text: string;
}

@Component({
  selector: 'app-custom-dialogue',
  templateUrl: './custom-dialogue.component.html',
  styleUrls: ['./custom-dialogue.component.scss']
})
export class CustomDialogueComponent {
    constructor(@Inject(MAT_DIALOG_DATA) protected options: CustomDialogueOptions,) {}
}
