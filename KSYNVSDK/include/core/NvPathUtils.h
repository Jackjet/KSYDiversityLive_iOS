//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jul 03. 2014
//   Author:        NewAuto video team
//================================================================================
#include "NvDef.h"
#include <QString>


// Ensure the directory of file path exist(create it if necessary)
// NOTE: filePath must be a path contain a file name, if it is a
// pure directory path, this function will not work at all!
bool NvEnsureFileDir(const QString &filePath);

// Ensure the directory path exist(create it if necessary)
// NOTE: make sure dirPath is a directory path
bool NvEnsureDir(const QString &dirPath);

// Check whether the path is a URL
bool NvIsPathUrl(const QString &filePath);

// Check whether the path come from iOS ALAssetLibrary
bool NvIsPathFromALAssetLibrary(const QString &filePath);

// Check whether the path come from iOS iPod library
bool NvIsPathFromIPodLibrary(const QString &filePath);

// Copy a directory to another directory recursively
bool NvCopyDirectoryRecursively(const QString &srcDirPath, const QString &dstDirPath);

