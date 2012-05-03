/*************************************************************************
 * 
 * TIGHTDB CONFIDENTIAL
 * __________________
 * 
 *  [2011] - [2012] TightDB Inc
 *  All Rights Reserved.
 * 
 * NOTICE:  All information contained herein is, and remains
 * the property of TightDB Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to TightDB Incorporated
 * and its suppliers and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from TightDB Incorporated.
 *
 **************************************************************************/
#ifndef __TDB_GROUP_WRITER__
#define __TDB_GROUP_WRITER__

#ifdef _MSC_VER
#include "win32/stdint.h"
#else
#include <stdint.h> // unint8_t etc
#endif
#include <cstdlib> // size_t

namespace tightdb {

// Pre-declarations
class Group;
class SlabAlloc;

class GroupWriter {
public:
    GroupWriter(Group& group);
    
	bool IsValid() const;
    
	void Commit();
    
    size_t write(const char* p, size_t n);
    void WriteAt(size_t pos, const char* p, size_t n);
    
private:
	void DoCommit(uint64_t topPos);
    
	// Member variables
	Group&     m_group;
	SlabAlloc& m_alloc;
	size_t     m_len;
	int        m_fd;
};
    
} //namespace tightdb

#endif //__TDB_GROUP_WRITER__