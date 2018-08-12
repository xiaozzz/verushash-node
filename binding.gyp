{
    "targets": [
        {
            "target_name": "verushash",
            "dependencies": [
            ],
            "sources": [
                "crypto/common.h",
                "crypto/haraka.h",
                "crypto/haraka_portable.h",
                "crypto/verus_hash.h",
                "crypto/haraka.c",
                "crypto/haraka_portable.c",
                "crypto/verus_hash.cpp",
                "verushash.cc",
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
            ],
            "defines": [
            ],
            "cflags_cc": [
                "-std=c++11",
                "-Wl,--whole-archive",
                "-fPIC",
                "-fexceptions",
                "-Ofast",
                "-march=native",
                "-msse4",
                "-msse4.1",
                "-msse4.2",
                "-mssse3",
                "-mavx",
                "-maes",
            ],
            "cflags": [
                "-Wl,--whole-archive",
                "-fPIC",
                "-fexceptions",
                "-Ofast",
                "-march=native",
                "-msse4",
                "-msse4.1",
                "-msse4.2",
                "-mssse3",
                "-mavx",
                "-maes",
            ],
            "link_settings": {
                "libraries": [
                    "-Wl,-rpath,./build/Release/",
                ]
            },
            "conditions": [
                ['OS=="mac"', {
                    'xcode_settings': {
                        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                    }
                }]
            ]
        }
    ]
}
