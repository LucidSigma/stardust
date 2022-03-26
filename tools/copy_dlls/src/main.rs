use std::fs;
use std::path::Path;

use clap::{App, Arg};

struct BuildDirectories<'a> {
    debug: &'a str,
    release: &'a str,
}

impl BuildDirectories<'_> {
    #[inline]
    fn does_debug_directory_exist(&self) -> bool {
        Path::new(self.debug).exists()
    }

    #[inline]
    fn does_release_directory_exist(&self) -> bool {
        Path::new(self.release).exists()
    }
}

struct DLLData {
    name: String,
    location: String,
    has_separate_build_directories: bool,
}

impl DLLData {
    #[inline]
    fn does_exist(&self) -> bool {
        if self.has_separate_build_directories {
            Path::new(&format!("{}/Debug", self.location)).exists()
                && Path::new(&format!("{}/Release", self.location)).exists()
        } else {
            Path::new(&self.location).exists()
        }
    }
}

fn main() {
    println!("Copying DLLS...");

    if !cfg!(target_os = "windows") {
        println!("Not a Windows system.");

        return;
    }

    let arguments = App::new("Copy DLLS")
        .arg(
            Arg::with_name("sdl2_version")
                .long("sdl2")
                .takes_value(true)
                .required(true)
                .help("SDL2 version"),
        )
        .arg(
            Arg::with_name("angle_version")
                .long("angle")
                .takes_value(true)
                .required(true)
                .help("ANGLE version"),
        )
        .get_matches();

    let sdl2_dll = DLLData {
        name: String::from("SDL2"),
        location: format!(
            "dep/SDL2-{}/lib",
            arguments
                .value_of("sdl2_version")
                .expect("SDL2 version not found")
        ),
        has_separate_build_directories: false,
    };

    let lib_egl_dll = DLLData {
        name: String::from("libEGL"),
        location: format!(
            "dep/ANGLE-{}/lib",
            arguments
                .value_of("angle_version")
                .expect("ANGLE version not found")
        ),
        has_separate_build_directories: true,
    };

    let lib_gles_v2_dll = DLLData {
        name: String::from("libGLESv2"),
        location: format!(
            "dep/ANGLE-{}/lib",
            arguments
                .value_of("angle_version")
                .expect("ANGLE version not found")
        ),
        has_separate_build_directories: true,
    };

    let dlls = vec![sdl2_dll, lib_egl_dll, lib_gles_v2_dll];

    for dll in &dlls {
        if !dll.does_exist() {
            panic!(
                "{} DLL does not exist at location {}.",
                dll.name, dll.location
            );
        }
    }

    let project_build_directories = BuildDirectories {
        debug: "build/bin/Debug",
        release: "build/bin/Release",
    };

    let manual_test_build_directories = BuildDirectories {
        debug: "build/bin/tests/Debug/manual",
        release: "build/bin/tests/Release/manual",
    };

    let unit_test_build_directories = BuildDirectories {
        debug: "build/bin/tests/Debug/unit",
        release: "build/bin/tests/Release/unit",
    };

    copy_dlls_to_directories(&project_build_directories, &dlls, "project");
    copy_dlls_to_directories(&manual_test_build_directories, &dlls, "manual test");
    copy_dlls_to_directories(&unit_test_build_directories, &dlls, "unit test");

    println!("Finished copying DLLs.");
}

fn copy_dlls_to_directories(
    build_directories: &BuildDirectories,
    dlls: &[DLLData],
    copy_run_name: &str,
) {
    for dll in dlls {
        if build_directories.does_debug_directory_exist() {
            let dll_location = format!(
                "{}{}/{}.dll",
                dll.location,
                if dll.has_separate_build_directories {
                    "/Debug"
                } else {
                    ""
                },
                dll.name
            );

            fs::copy(
                &dll_location,
                format!("{}/{}.dll", build_directories.debug, dll.name),
            )
            .unwrap_or_else(|e| {
                panic!(
                    "Failed to copy {} DLL at {} to {} debug build location: {:?}",
                    dll.name, dll_location, copy_run_name, e
                )
            });

            println!(
                "Copied {} DLL to debug {} build ({}).",
                dll.name, copy_run_name, build_directories.debug
            );
        } else {
            println!("Build directory for debug {} does not exist", copy_run_name);
        }

        if build_directories.does_release_directory_exist() {
            let dll_location = format!(
                "{}{}/{}.dll",
                dll.location,
                if dll.has_separate_build_directories {
                    "/Release"
                } else {
                    ""
                },
                dll.name
            );

            fs::copy(
                &dll_location,
                format!("{}/{}.dll", build_directories.release, dll.name),
            )
            .unwrap_or_else(|_| {
                panic!(
                    "Failed to copy {} DLL to {} release build location",
                    dll.name, copy_run_name
                )
            });

            println!(
                "Copied {} DLL to release {} build ({}).",
                dll.name, copy_run_name, build_directories.release
            );
        } else {
            println!(
                "Build directory for release {} does not exist",
                copy_run_name
            );
        }
    }
}
