typedef struct {
    state_t target_state;
    uint32_t tick;
    int expected_toggles;
    const char* description;
} blink_case_t;