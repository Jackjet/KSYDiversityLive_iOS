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
#include "NvGuidDef.h"
#include <QString>


// Test whether the string represent 'true'
bool NvIsTrueString(const QString &str);
bool NvIsTrueString(const QStringRef &str);

// Construct a string from an arbitrary binary data
QString NvBinaryToString(const void *binaryData, unsigned int dataSizeInBytes);

// Return size in bytes of the binary data represented by the string
unsigned int NvBinarySizeInBytesForString(const QString &binaryString);

// Construct binary data from string, binaryData should have been allocated with enough sapce!
void NvStringToBinary(const QString &binaryString, void *binaryData);

// Construct a string from GUID object
QString NvGuidToString(const GUID &guid);

// Construct a GUID object from string
void NvStringToGuid(const QString &guidString, GUID &guid);

// Construct a SNvColor object from string
SNvColor NvStringToColor(const QString &str);
SNvColor NvStringToColor(const QStringRef &str);

// Construct a string object from SNvColor
QString NvColorToString(const SNvColor &color);

// Construct a SNvPosition2D object from string
SNvPosition2D NvStringToPosition2D(const QString &str);
SNvPosition2D NvStringToPosition2D(const QStringRef &str);

// Construct a SNvPosition3D object from string
SNvPosition3D NvStringToPosition3D(const QString &str);
SNvPosition3D NvStringToPosition3D(const QStringRef &str);

