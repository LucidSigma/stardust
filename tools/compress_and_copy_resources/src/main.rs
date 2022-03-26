use std::fs;
use std::path::Path;
use std::process::Command;

fn main() {
    println!("Copying resources...");

    const SOURCE_DIRECTORY: &str = "resources";

    if !Path::new(SOURCE_DIRECTORY).exists() {
        panic!("Resources folder does not exist");
    }

    let (debug_build_directory, release_build_directory) = get_build_directories();

    for resource_type in ["assets", "locales", "scripts"] {
        create_asset_archive(
            SOURCE_DIRECTORY,
            debug_build_directory,
            resource_type,
            "Debug",
        );

        create_asset_archive(
            SOURCE_DIRECTORY,
            release_build_directory,
            resource_type,
            "Release",
        );
    }

    println!("Resources copied successfully.");
}

fn get_build_directories<'a>() -> (&'a str, &'a str) {
    if cfg!(target_os = "windows") {
        ("build/bin/Debug", "build/bin/Release")
    } else {
        panic!("Unsupported operating system")
    }
}

fn create_asset_archive(
    source_directory: &str,
    destination_directory: &str,
    resource_type: &str,
    build_type: &str,
) {
    if Path::new(destination_directory).exists() {
        let destination_directory = format!("{}/resources", destination_directory);

        if !Path::new(&destination_directory).exists() {
            fs::create_dir(&destination_directory)
                .unwrap_or_else(|_| panic!("Failed to create directory {}", destination_directory));

            println!("Created directory {}", destination_directory);
        }

        Command::new("powershell")
            .arg("python")
            .arg("./tools/create_pak_archive.py")
            .arg(format!("{}/{}", source_directory, resource_type))
            .arg(format!("{}/{}.pak", destination_directory, resource_type))
            .arg(resource_type)
            .status()
            .unwrap_or_else(|_| {
                panic!(
                    "Failed to execute PAK Python script for {} build",
                    String::from(build_type).to_lowercase()
                )
            });
    } else {
        println!(
            "{} build directory does not exist. It might not be built.",
            build_type
        );
    }

    println!(
        "Created {} archive successfully in {}.",
        resource_type, destination_directory
    );
}
