#pragma once
#pragma warning(disable:4996)
# define Debug
# ifdef Debug
# define FILE_PATH "result.log"
# include <iostream>
# include <fstream>
# include <atlstr.h>

void printLog(CString text);

# endif