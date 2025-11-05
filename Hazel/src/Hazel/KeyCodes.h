// 键盘键值：参考GLFW定义，构建引擎自定义键值体系
#define HZ_KEY_UNKNOWN            -1

// 打印键区
#define HZ_KEY_SPACE              32
#define HZ_KEY_APOSTROPHE         39  /* ' */
#define HZ_KEY_COMMA              44  /* , */
#define HZ_KEY_MINUS              45  /* - */
#define HZ_KEY_PERIOD             46  /* . */
#define HZ_KEY_SLASH              47  /* / */
#define HZ_KEY_0                  48
#define HZ_KEY_1                  49
// ...（省略数字键、字母键定义，逻辑与GLFW一致）
#define HZ_KEY_A                  65
#define HZ_KEY_B                  66
#define HZ_KEY_C                  67
// ...（省略其余字母键）

// 功能键
#define HZ_KEY_ESCAPE             256
#define HZ_KEY_ENTER              257
#define HZ_KEY_TAB                258
#define HZ_KEY_BACKSPACE          259
#define HZ_KEY_INSERT             260
#define HZ_KEY_DELETE             261
// ...（省略方向键、功能键F1-F12等定义）