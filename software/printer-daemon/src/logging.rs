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

use chrono::Local;
use log::{debug, LevelFilter};
use simplelog::{ColorChoice, Config, ConfigBuilder, LevelPadding, TargetPadding, TerminalMode, TermLogger, ThreadLogMode};
use time::UtcOffset;

use crate::config;

pub fn initialise() {
    let logging_filter = get_logging_level();
    let logging_configuration = get_logging_config();

    _ = TermLogger::init(
        logging_filter,
        logging_configuration,
        TerminalMode::Stderr,
        ColorChoice::Never
    );

    debug!("Logging initialised");
    debug!("Will log to stderr. Your OS daemon management service should store this into a log file");
}

fn get_logging_level() -> LevelFilter {
    let log_level_envvar = std::env::var(config::LOGGING_LEVEL_ENVVAR);

    match log_level_envvar {
        Ok(level) => match level.to_uppercase().as_str() {
            "ERROR" => LevelFilter::Error,
            "WARN" => LevelFilter::Warn,
            "DEBUG" => LevelFilter::Debug,

            _ => LevelFilter::Warn,
        }
        _ => LevelFilter::Warn,
    }
}

fn get_logging_config() -> Config {
    ConfigBuilder::new()
        .set_max_level(LevelFilter::Error)
        .set_time_level(LevelFilter::Error)
        .set_thread_level(LevelFilter::Debug)
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
