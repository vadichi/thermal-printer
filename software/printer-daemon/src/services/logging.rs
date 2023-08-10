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

use std::time::Duration;
use chrono::Local;
use log::{info, LevelFilter, warn};
use simplelog::{ColorChoice, Config, ConfigBuilder, LevelPadding, TargetPadding, TerminalMode, TermLogger, ThreadLogMode};
use time::UtcOffset;

use crate::config;

pub fn initialise() {
    let logging_filter = get_logging_level();
    let logging_configuration = get_logging_config();

     let result =  TermLogger::init(
        logging_filter,
        logging_configuration,
        TerminalMode::Stderr,
        ColorChoice::Never
     );

    if result.is_err() {
        eprintln!("Failed to initialise logging service");
        eprintln!("Printer daemon will continue executing but will be unable to make further logs");
    }

    info!("Logging initialised");
    info!("Logging level set to {}", logging_filter);
    info!("Logging to stderr");
    info!("The daemon manager of your OS should redirect the log to a file");
}

fn get_logging_level() -> LevelFilter {
    let log_level_envvar = std::env::var(config::LOGGING_LEVEL_ENVVAR);

    match log_level_envvar {
        Ok(level) => match level.to_uppercase().as_str() {
            "ERROR" => LevelFilter::Error,
            "WARN" => LevelFilter::Warn,
            "INFO" => LevelFilter::Info,
            "DEBUG" => LevelFilter::Debug,

            _ => {
                // Any calls to logging functions are ignored until the logging services is fully initialised
                async fn wait_and_log() {
                    tokio::time::sleep(Duration::from_millis(50)).await;
                    warn!("Unknown value of the logging level environmental variable (${})", config::LOGGING_LEVEL_ENVVAR);
                    warn!("Defaulting to INFO");
                }

                tokio::spawn(wait_and_log());
                LevelFilter::Info
            },
        }
        Err(_) => {
            // Any calls to logging functions are ignored until the logging services is fully initialised
            async fn wait_and_log() {
                tokio::time::sleep(Duration::from_millis(50)).await;
                info!("Logging level environmental variable (${}) not set", config::LOGGING_LEVEL_ENVVAR);
                info!("Defaulting to INFO");
            }

            tokio::spawn(wait_and_log());
            LevelFilter::Info
        },
    }
}

fn get_logging_config() -> Config {
    ConfigBuilder::new()
        .set_max_level(LevelFilter::Error)
        .set_time_level(LevelFilter::Error)
        .set_thread_level(LevelFilter::Error)
        .set_target_level(LevelFilter::Trace)
        .set_target_padding(TargetPadding::Off)
        .set_location_level(LevelFilter::Trace)
        .set_level_padding(LevelPadding::Off)
        .set_thread_mode(ThreadLogMode::Both)
        .set_time_format_custom(config::LOGGING_TIMESTAMP_FORMAT)
        .set_time_offset(get_utc_offset())
        .build()
}

fn get_utc_offset() -> UtcOffset {
    let offset_seconds = Local::now().offset().local_minus_utc();
    let offset_result = UtcOffset::from_whole_seconds(offset_seconds);

    match offset_result {
        Ok(offset) => offset,
        Err(_) => UtcOffset::UTC,
    }
}
