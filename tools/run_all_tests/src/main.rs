use std::ffi::OsStr;
use std::fs;
use std::path::Path;
use std::process::Command;

use clap::{App, Arg};

fn main() {
    let arguments = App::new("Run All Tests")
        .arg(
            Arg::with_name("type")
                .long("type")
                .takes_value(true)
                .required(false)
                .help("Build type of tests (debug or release)"),
        )
        .get_matches();

    let build_type = {
        let tentative_build_type =
            String::from(arguments.value_of("type").unwrap_or("debug")).to_lowercase();

        if tentative_build_type != "debug" && tentative_build_type != "release" {
            println!(
                "Invalid build type {}. Defaulting to debug.",
                tentative_build_type
            );

            String::from("debug")
        } else {
            tentative_build_type
        }
    };

    let test_directory = if build_type == "debug" {
        "build/bin/tests/Debug/unit"
    } else {
        "build/bin/tests/Release/unit"
    };

    println!("Executing {} unit tests...", build_type);

    if !Path::new(test_directory).exists() {
        panic!("Unit test directory does not exist for build type {}.", build_type);
    }

    if cfg!(target_os = "windows") {
        fs::read_dir(test_directory)
        .unwrap()
        .filter(|path| {
            Path::new(path.as_ref().unwrap().file_name().to_str().unwrap())
                .extension()
                .unwrap_or(OsStr::new(""))
                == "exe"
        })
        .map(|path| path.as_ref().unwrap().file_name())
        .for_each(|test| {
            let file_string = format!("{}/{}", test_directory, test.to_str().unwrap());

            println!("Running test \"{}\"...", file_string);

            Command::new("powershell")
                .arg(format!("./{}", file_string))
                .status()
                .expect("Failed to execute tests.");
        });
    }

    println!("Tests completed.");
}
