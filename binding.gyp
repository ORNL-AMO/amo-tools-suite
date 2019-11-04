
{
    "targets": [
        # {
        #     "target_name": "standalone",
        #     'include_dirs': [
        #         "include",
        #         "include/ssmt",
        #         "include/calculator/util",
        #         "<!(node -e \"require('nan')\")",
        #     ],
        #     'sources' : [
        #         'bindings/standalone.cpp',
        #         'src/ssmt/SteamSystemModelerTool.cpp',
        #         'src/ssmt/SaturatedProperties.cpp',
        #         'src/calculator/util/CHP.cpp',
        #         "<!@(node -e \"console.log(require('fs').readdirSync('src/calculator/util/').map(f=>'src/calculator/util/'+f).join(' '))\")"
        #     ],
        #     "conditions": [
        #         [ 'OS=="mac"', {
        #             "xcode_settings": {
        #                 'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
        #                 'OTHER_LDFLAGS': ['-stdlib=libc++'],
        #                 'MACOSX_DEPLOYMENT_TARGET': '10.9',
        #                 'CLANG_CXX_LIBRARY': 'libc++',
        #                 'GCC_ENABLE_CPP_RTTI': 'YES',
        #                 'GCC_ENABLE_CPP_EXCEPTIONS': "YES"
        #             },
        #         }],
        #         [ 'OS=="linux"', {
        #             'cflags_cc': ['-fexceptions']
        #         }]
        #     ]
        # },
        # {
        #     "target_name": "fan",
        #     'include_dirs': [
        #         "include",
        #         "include/fans",
        #         "include/results/",
        #         "include/calculator/motor/",
        #         "include/calculator/pump/",
        #         'include/ssmt',
        #         "include/calculator/util/",
        #         "<!(node -e \"require('nan')\")",
        #     ],
        #     'sources' : [
        #         'bindings/fan.cpp',
        #         'src/ssmt/SteamSystemModelerTool.cpp',
        #         'src/ssmt/SaturatedProperties.cpp',
        #         "<!@(node -e \"console.log(require('fs').readdirSync('src/fans/').map(f=>'src/fans/'+f).join(' '))\")",
        #         "<!@(node -e \"console.log(require('fs').readdirSync('src/calculator/pump/').map(f=>'src/calculator/pump/'+f).join(' '))\")",
        #         "<!@(node -e \"console.log(require('fs').readdirSync('src/calculator/motor/').map(f=>'src/calculator/motor/'+f).join(' '))\")",
        #         "<!@(node -e \"console.log(require('fs').readdirSync('src/calculator/util/').map(f=>'src/calculator/util/'+f).join(' '))\")",
        #         "<!@(node -e \"console.log(require('fs').readdirSync('src/results/').map(f=>'src/results/'+f).join(' '))\")"
        #     ],
        #     "conditions": [
        #         [ 'OS=="mac"', {
        #             "xcode_settings": {
        #                 'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
        #                 'OTHER_LDFLAGS': ['-stdlib=libc++'],
        #                 'MACOSX_DEPLOYMENT_TARGET': '10.9',
        #                 'CLANG_CXX_LIBRARY': 'libc++',
        #                 'GCC_ENABLE_CPP_RTTI': 'YES',
        #                 'GCC_ENABLE_CPP_EXCEPTIONS': "YES"
        #             },
        #         }],
        #         [ 'OS=="linux"', {
        #             'cflags_cc': ['-fexceptions']
        #         }]
        #     ]
        # },
        # {
        #     "target_name": "phast",
        #     'include_dirs': ['include', 'include/calculator/losses', 'include/phast', 'include/calculator/furnace' ,
        #         "<!(node -e \"require('nan')\")"
        #      ],
        #     'sources' : [
        #         'bindings/phast.cpp',
        #         "<!@(node -e \"console.log(require('fs').readdirSync('src/calculator/losses/').map(f=>'src/calculator/losses/'+f).join(' '))\")",
        #         "<!@(node -e \"console.log(require('fs').readdirSync('src/calculator/furnace/').map(f=>'src/calculator/furnace/'+f).join(' '))\")"
        #     ],
        #     "conditions": [
        #         [ 'OS=="mac"', {
        #             "xcode_settings": {
        #                 'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
        #                 'OTHER_LDFLAGS': ['-stdlib=libc++'],
        #                 'MACOSX_DEPLOYMENT_TARGET': '10.9',
        #                 'CLANG_CXX_LIBRARY': 'libc++',
        #                 'GCC_ENABLE_CPP_RTTI': 'YES',
        #                 'GCC_ENABLE_CPP_EXCEPTIONS': "YES"
        #             },
        #         }],
        #         [ 'OS=="linux"', {
        #             'cflags_cc': ['-fexceptions']
        #         }]
        #     ]
        # },
        # {
        #     "target_name": "psat",
        #     'include_dirs': ['include', 'include/results', 'include/ssmt', 'include/calculator/pump', 'include/calculator/motor', 'include/calculator/util',
        #         "<!(node -e \"require('nan')\")"
        #      ],
        #     'sources' : [
        #         'bindings/psat.cpp',
        #         'src/ssmt/SteamSystemModelerTool.cpp',
        #         'src/ssmt/SaturatedProperties.cpp',
        #         "<!@(node -e \"console.log(require('fs').readdirSync('src/calculator/pump/').map(f=>'src/calculator/pump/'+f).join(' '))\")",
        #         "<!@(node -e \"console.log(require('fs').readdirSync('src/calculator/motor/').map(f=>'src/calculator/motor/'+f).join(' '))\")",
        #         "<!@(node -e \"console.log(require('fs').readdirSync('src/calculator/util/').map(f=>'src/calculator/util/'+f).join(' '))\")",
        #         "<!@(node -e \"console.log(require('fs').readdirSync('src/results/').map(f=>'src/results/'+f).join(' '))\")",
        #     ],
        #     "conditions": [
        #         [ 'OS=="mac"', {
        #             "xcode_settings": {
        #                 'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
        #                 'OTHER_LDFLAGS': ['-stdlib=libc++'],
        #                 'MACOSX_DEPLOYMENT_TARGET': '10.9',
        #                 'CLANG_CXX_LIBRARY': 'libc++',
        #                 'GCC_ENABLE_CPP_RTTI': 'YES',
        #                 'GCC_ENABLE_CPP_EXCEPTIONS': "YES"
        #             },
        #         }],
        #         [ 'OS=="linux"', {
        #             'cflags_cc': ['-fexceptions']
        #         }]
        #     ]
        # },
        {
            "target_name": "ssmt",
            'include_dirs': ['include', 'include/ssmt', 'include/ssmt/api', 'include/ssmt/domain',
                'include/ssmt/service', 'include/ssmt/service/high_pressure_header',
                'include/ssmt/service/low_pressure_header', 'include/ssmt/service/medium_pressure_header',
                'include/ssmt/service/water_and_condensate', 'include/ssmt/service/power_balance',
                'include/ssmt/service/process_steam_usage', 'include/ssmt/service/energy_and_cost',
                "<!(node -e \"require('nan')\")"
            ],
            'sources' : [
                'bindings/ssmt.cpp',
                "<!@(node -e \"console.log(require('fs').readdirSync('src/ssmt/').map(f=>'src/ssmt/'+f).join(' '))\")",
                "<!@(node -e \"console.log(require('fs').readdirSync('src/ssmt/api/').map(f=>'src/ssmt/api/'+f).join(' '))\")",
                "<!@(node -e \"console.log(require('fs').readdirSync('src/ssmt/domain/').map(f=>'src/ssmt/domain/'+f).join(' '))\")",
                "<!@(node -e \"console.log(require('fs').readdirSync('src/ssmt/service/').map(f=>'src/ssmt/service/'+f).join(' '))\")",
                "<!@(node -e \"console.log(require('fs').readdirSync('src/ssmt/service/high_pressure_header/').map(f=>'src/ssmt/service/high_pressure_header/'+f).join(' '))\")",
                "<!@(node -e \"console.log(require('fs').readdirSync('src/ssmt/service/low_pressure_header/').map(f=>'src/ssmt/service/low_pressure_header/'+f).join(' '))\")",
                "<!@(node -e \"console.log(require('fs').readdirSync('src/ssmt/service/medium_pressure_header/').map(f=>'src/ssmt/service/medium_pressure_header/'+f).join(' '))\")",
                "<!@(node -e \"console.log(require('fs').readdirSync('src/ssmt/service/water_and_condensate/').map(f=>'src/ssmt/service/water_and_condensate/'+f).join(' '))\")",
                "<!@(node -e \"console.log(require('fs').readdirSync('src/ssmt/service/power_balance/').map(f=>'src/ssmt/service/power_balance/'+f).join(' '))\")",
                "<!@(node -e \"console.log(require('fs').readdirSync('src/ssmt/service/process_steam_usage/').map(f=>'src/ssmt/service/process_steam_usage/'+f).join(' '))\")",
                "<!@(node -e \"console.log(require('fs').readdirSync('src/ssmt/service/energy_and_cost/').map(f=>'src/ssmt/service/energy_and_cost/'+f).join(' '))\")",
            ],
            "conditions": [
                [ 'OS=="mac"', {
                    "xcode_settings": {
                        'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
                        'OTHER_LDFLAGS': ['-stdlib=libc++'],
                        'MACOSX_DEPLOYMENT_TARGET': '10.9',
                        'CLANG_CXX_LIBRARY': 'libc++',
                        'GCC_ENABLE_CPP_RTTI': 'YES',
                        'GCC_ENABLE_CPP_EXCEPTIONS': "YES"
                    },
                }],
                [ 'OS=="linux"', {
                    'cflags_cc': ['-fexceptions']
                }]
            ]
        },
		# {
        #     "target_name": "db",
        #     'include_dirs': ['include', 'include/sqlite', 'third_party/sqlite', 'include/calculator/losses', 'include/calculator/motor',
        #         "<!(node -e \"require('nan')\")"
        #      ],
        #     'sources' : [
        #         'bindings/db.cpp',
        #         'third_party/sqlite/sqlite3.c',
        #         "<!@(node -e \"console.log(require('fs').readdirSync('src/calculator/losses/').map(f=>'src/calculator/losses/'+f).join(' '))\")",
        #         "<!@(node -e \"console.log(require('fs').readdirSync('src/sqlite/').map(f=>'src/sqlite/'+f).join(' '))\")",
        #     ],
        #     "conditions": [
        #         [ 'OS=="mac"', {
        #             "xcode_settings": {
        #                 'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
        #                 'OTHER_LDFLAGS': ['-stdlib=libc++'],
        #                 'MACOSX_DEPLOYMENT_TARGET': '10.9',
        #                 'CLANG_CXX_LIBRARY': 'libc++',
        #                 'GCC_ENABLE_CPP_RTTI': 'YES',
        #                 'GCC_ENABLE_CPP_EXCEPTIONS': "YES"
        #             },
        #         }],
        #         [ 'OS=="linux"', {
        #             'cflags_cc': ['-fexceptions']
        #         }]
        #     ]
        # },
        # {
        #     "target_name": "calculator",
        #     'include_dirs': [
        #         'include',
        #         'include/ssmt',
        #         'include/calculator/util',
        #         "<!(node -e \"require('nan')\")"
        #     ],
        #     'sources': [
        #         'bindings/calculator.cpp',
        #         'src/calculator/util/ElectricityReduction.cpp',
        #         'src/calculator/util/NaturalGasReduction.cpp',
        #         'src/calculator/util/CompressedAirReduction.cpp',
        #         'src/calculator/util/CompressedAirPressureReduction.cpp',
        #         'src/calculator/util/WaterReduction.cpp',
        #         'src/ssmt/SteamSystemModelerTool.cpp',
        #         'src/ssmt/SaturatedProperties.cpp',
        #         'src/calculator/util/SteamReduction.cpp',
        #         "<!@(node -e \"console.log(require('fs').readdirSync('src/calculator/util/').map(f=>'src/calculator/util/'+f).join(' '))\")",
        #     ],
        #     "conditions": [
        #         [ 'OS=="mac"', {
        #             "xcode_settings": {
        #                 'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
        #                 'OTHER_LDFLAGS': ['-stdlib=libc++'],
        #                 'MACOSX_DEPLOYMENT_TARGET': '10.9',
        #                 'CLANG_CXX_LIBRARY': 'libc++',
        #                 'GCC_ENABLE_CPP_RTTI': 'YES',
        #                 'GCC_ENABLE_CPP_EXCEPTIONS': "YES"
        #             },
        #         }],
        #         [ 'OS=="linux"', {
        #             'cflags_cc': ['-fexceptions']
        #         }]
        #     ]
        # }
    ]
}

