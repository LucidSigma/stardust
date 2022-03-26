use std::path::Path;
use std::process::Command;

fn main() {
    println!("Copying test resources...");

    if cfg!(target_os = "windows") {
        const TEST_RESOURCE_DIRECTORY: &str = "tests/test_resources";

        for build_type in ["Debug", "Release"] {
            let build_directory = format!("build/bin/tests/{}", build_type);

            if !Path::new(&build_directory).exists() {
                println!(
                    "{} tests do not exist at {}. They might not be built yet.",
                    build_type, build_directory
                );

                continue;
            }

            Command::new("powershell")
                .arg("Copy-Item")
                .args(["-Path", TEST_RESOURCE_DIRECTORY])
                .args([
                    "-Destination",
                    &format!("{}", build_directory),
                ])
                .arg("-Recurse")
                .arg("-Force")
                .status()
                .unwrap_or_else(|_| {
                    panic!(
                        "Failed to copy test resources into {} tests directory",
                        String::from(build_type).to_lowercase()
                    )
                });

            println!(
                "Copied test resources to {} directory ({}).",
                String::from(build_type).to_lowercase(),
                build_directory
            );
        }
    }

    println!("Finished copying test resources.");
}
