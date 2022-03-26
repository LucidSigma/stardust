use std::process::Command;

fn main() {
    println!("Running premake5...");

    if cfg!(target_os = "windows") {
        Command::new("premake5")
            .arg("vs2022")
            .status()
            .expect("Failed to execute premake5.");
    }
}
