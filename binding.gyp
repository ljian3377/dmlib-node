{
  "targets": [
    {
      "target_name": "dmlib-addon",
      "defines": ["V8_DEPRECATION_WARNINGS=1"],
      "sources": ["src/addon.cc"],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "dmlib/dmlib_core/inc"
      ],
      "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "xcode_settings": {
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "10.7"
      },
      "msvs_settings": {
        "VCCLCompilerTool": {
          "ExceptionHandling": 1
        }
      },
      "conditions": [
        ["OS==\"mac\"", {}],
        ["OS==\"linux\"", {}],
        [
          "OS==\"win\"",
          {
            "libraries": ["../deps/win/dmlib.lib"],
            "copies": [
              {
                "destination": "<(module_root_dir)/build/Release/",
                "files": ["<(module_root_dir)/deps/win/dmlib.dll"]
              }
            ]
          }
        ]
      ]
    }
  ]
}
