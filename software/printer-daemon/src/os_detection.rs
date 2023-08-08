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

use log::{debug, error};

pub struct OSDetector {
    os_string: &'static str,
}

impl OSDetector {
    pub fn initialise() -> Self {
        let os_string = detect_os();

        if os_string != "macos" {
            error!("The host operating system is not yet supported");
            panic!("The host operating system is not yet supported");
        }

        Self { os_string }
    }

    pub fn get_config_property<T>(&self, property_map: phf::Map<&'static str, T>) -> T
    where T: Clone {
        let result = property_map.get(self.os_string);

        match result {
            Some(property) => property.clone(),
            None => {
                error!("Missing OS-specific configuration entry for {}", self.os_string);
                panic!("The host operating system is not yet fully supported");
            }
        }
    }
}

fn detect_os() -> &'static str {
    let os_string = std::env::consts::OS;

    debug!("Detected host operating system: {}", os_string);
    os_string
}
