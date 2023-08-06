/*
 * Copyright 2023 Vadim Chichikalyuk
 *
 * This file is part of Thermal Printer
 *
 * Thermal Printer is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * Thermal Printer is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Thermal Printer. If not, see <https://www.gnu.org/licenses/>.
 */

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
