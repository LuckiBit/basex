#ifndef PLATFORM_H
#define PLATFORM_H

/**
 * @brief Initialize platform settings.
 * 
 * On Windows, sets console to UTF-8 encoding.
 * Prints platform info.
 * 
 * Call once before any console output.
 * 
 * @return int Returns 0 on success.
 */
int platform_init(void);

#endif // PLATFORM_H
