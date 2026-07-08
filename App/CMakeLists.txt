add_library(app_logic
    Src/state_machine.c
    Src/guards.c
)

target_include_directories(app_logic PUBLIC Inc)
