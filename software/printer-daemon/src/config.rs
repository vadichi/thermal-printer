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

use phf::{Map, phf_map, phf_set, Set};
use time::format_description::FormatItem;
use time::macros::format_description;

pub const LOGGING_LEVEL_ENVVAR: &str = "THERMAL_PRINTER_LOG_LEVEL";
pub const LOGGING_TIMESTAMP_FORMAT: &[FormatItem] =
    format_description!("[year]-[month]-[day]T[hour]:[minute]:[second][offset_hour sign:mandatory]:[offset_minute]");

pub const OS_DETECTION_SUPPORTED_SYSTEMS: Set<&'static str> = phf_set!("macos", "linux");

pub const RUNTIME_DATA_DIRECTORY_NAME: &str = "com.vadimtch.thermal_printer.printer_daemon";
pub const RUNTIME_DATA_DIRECTORY_LOCATION: Map<&'static str, &'static str> = phf_map!(
    "macos" => "$HOME/Library/Caches",
    "linux" => "/run/user/$UID",
);
