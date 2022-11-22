/**
 * @date:   2022-10-17 15:12:05
 * @lastModifiedTime: 2022-10-17 16:48:01
 * @License: Apache License 2.0, https://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef __DEBUGGER_H__
#define __DEBUGGER_H__

extern char glob_var;                                                                                              \
// call if you want to debug a value or a line of code.
#define debug(...)                                                                                                     \
    if (glob_var == '1' || glob_var == 'y' || glob_var == 'Y') {                                                       \
        debugLogger(0, __LINE__, __FILE__, __VA_ARGS__);                                                               \
    }
// call if you encounter a error, or a state that was not expected.
#define error(...) debugLogger(1, __LINE__, __FILE__, __VA_ARGS__)

void debugLogger(unsigned long ulErrorType, int iLine, const char *pszFile, const char *pszFormat, ...);
#endif //__DEBUGGER_H__
