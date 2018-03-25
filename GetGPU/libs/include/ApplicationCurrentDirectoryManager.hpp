#ifndef __APPLICATIONCURRENTDIRECTORYMANAGER_HPP__
#define __APPLICATIONCURRENTDIRECTORYMANAGER_HPP__
#include "ApplicationCurrentDirectoryManagerA.hpp"
#include "ApplicationCurrentDirectoryManagerW.hpp"

#ifdef UNICODE
typedef ApplicationCurrentDirectoryManagerW ApplicationCurrentDirectoryManager;
#else
typedef ApplicationCurrentDirectoryManagerA ApplicationCurrentDirectoryManager;
#endif
#endif
