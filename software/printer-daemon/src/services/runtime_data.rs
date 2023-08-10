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

use std::fs::{create_dir};
use std::path::{Path, PathBuf};
use log::{debug, error, info};

use crate::config;
use crate::services::os_detection::OSDetector;

pub struct RuntimeDataManager {
    data_dir: Box<Path>,
}

impl RuntimeDataManager {
    pub fn new(os_detector: OSDetector) -> Self {
        let data_dir_parent_str = os_detector.get_config_property(config::RUNTIME_DATA_DIRECTORY_LOCATION);

        debug!("Attempting to expand runtime data directory location: {}", data_dir_parent_str);
        let data_dir_parent_str: String = match shellexpand::env(data_dir_parent_str) {
            Ok(cow) => {
                debug!("Expanded into {}", cow.as_ref());
                cow.into_owned()
            }
            Err(_) => {
                error!("Failed to expand path: {}", data_dir_parent_str);
                error!("Required environmental variable not set");

                error!("Fatal error – shutting down");
                panic!("Printer daemon had to shut down due to an unrecoverable error. See logs for more details.");
            }
        };

        let data_dir: PathBuf = [&data_dir_parent_str, config::RUNTIME_DATA_DIRECTORY_NAME]
            .iter()
            .collect();

        info!("Setting runtime data directory path: {}", data_dir.display());

        debug!("Initialised runtime data manager");
        Self { data_dir: data_dir.into_boxed_path() }
    }

    fn diagnose_io_errors(&self) {
        debug!("Attempting to diagnose runtime data directory access I/O errors");
        debug!("Runtime data directory is at {}", self.data_dir.display());

        // Returns true if and only if the directory exists and is accessible
        if self.data_dir.exists() {
            debug!("No problems discovered");
            error!("Issues with access to runtime data directory: {}", self.data_dir.display());
            error!("Unable to diagnose I/O issues");

            error!("Fatal error – shutting down");
            panic!("Printer daemon had to shut down due to an unrecoverable error. See logs for more details.");
        }

        info!("Expecting runtime data directory at: {}", self.data_dir.display());
        info!("Runtime data directory does not exist or is inaccessible");
        info!("Attempting to create");

        match self.data_dir.parent() {
            Some(parent) => {
                debug!("Runtime data directory is in a system directory");

                if !parent.exists() {
                    error!("Attempted access to a system directory at {}", parent.display());
                    error!("Required system directory does not exist or is inaccessible");
                    error!("Will not attempt to create");

                    error!("Fatal error – shutting down");
                    panic!("Printer daemon had to shut down due to an unrecoverable error. See logs for more details.");
                }
            },
            None => {
                debug!("Runtime data directory is in a filesystem or prefix root");
            },
        };

        match create_dir(&self.data_dir) {
            Ok(_) => {
                debug!("Runtime data directory created successfully");
            },
            Err(_) => {
                error!("Unable to create runtime data directory: {}", self.data_dir.display());
                error!("This is likely caused by a misconfiguration of access permissions");

                error!("Fatal error – shutting down");
                panic!("Printer daemon had to shut down due to an unrecoverable error. See logs for more details.");
            },
        }

        if !self.data_dir.exists() {
            error!("Newly created runtime data directory at {} is inaccessible", self.data_dir.display());
            error!("This is likely caused by a misconfiguration of access permissions");

            error!("Fatal error – shutting down");
            panic!("Printer daemon had to shut down due to an unrecoverable error. See logs for more details.");
        }

        debug!("Runtime data directory access issues diagnostics complete");
    }
}
