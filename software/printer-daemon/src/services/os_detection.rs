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

use log::{debug, error, info};

use crate::config;

pub struct OSDetector {
    os_string: &'static str,
}

impl OSDetector {
    pub fn new() -> Self {
        let os_string = OSDetector::detect_os();
        debug!("Detected host operating system: {}", os_string);

        if config::OS_DETECTION_SUPPORTED_SYSTEMS.contains(os_string) {
            info!("Host operating system supported");
        } else {
            error!("Host operating system is not yet supported");

            error!("Fatal error – shutting down");
            panic!("Printer daemon had to shut down due to an unrecoverable error. See logs for more details.");
        }

        debug!("Initialised OS detection");
        Self { os_string }
    }

    pub fn get_config_property<T>(&self, property_map: phf::Map<&'static str, T>) -> T
    where T: Clone {
        let result = property_map.get(self.os_string);

        match result {
            Some(property) => {
                // This is generally a reference to a reference (e.g. &(&'static str)), so this is an inexpensive operation
                property.clone()
            },
            None => {
                error!("Missing OS-specific configuration entry for {}", self.os_string);

                error!("Fatal error – shutting down");
                panic!("Printer daemon had to shut down due to an unrecoverable error. See logs for more details.");
            }
        }
    }

    fn detect_os() -> &'static str {
        std::env::consts::OS
    }
}
