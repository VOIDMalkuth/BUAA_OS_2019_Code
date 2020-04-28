#undef panic
#undef env_run

#define panic(...)                               \
    do                                           \
    {                                            \
        _panic(__FILE__, __LINE__, __VA_ARGS__); \
        return;                                  \
    } while(0)

#define env_run(x)     \
	do {                  \
		_env_run((x)); \
		return;		   \
	} while (0)
