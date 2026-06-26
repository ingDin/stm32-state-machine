typedef struct {
    const char* description;
    int num_presses;
    state_t expected_state;
} state_transition_case_t;

typedef struct {
    state_t target_state;
    uint32_t tick;
    int expected_toggles;
    const char* description;
} blink_case_t;

typedef struct {
    const char* description;
    int num_presses_before_transition;
    state_t expected_state_after_transition;
} timer_reset_case_t;