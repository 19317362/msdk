/***************************************************************************************
* AUTHOR : sudami [sudami@163.com]
* TIME   : 2009/03/03 [3:3:2009 - 15:10]
* MODULE : F:\Tmp\���̲���\code\ntfs-3g-1.0\libntfs-3g\compat.c
* 
* Description:
*   
*                        
*
***
* Copyright (c) 2008 - 2010 sudami.
* Freely distributable in source or binary for noncommercial purposes.
* TAKE IT EASY,JUST FOR FUN.
*
****************************************************************************************/




#include "compat.h"


//#ifndef HAVE_FFS
/**
 * ffs - Find the first set bit in an int
 * @x:
 *
 * Description...
 *
 * Returns:
 */
int ffs(int x)
{
	int r = 1;

	if (!x)
		return 0;
	if (!(x & 0xffff)) {
		x >>= 16;
		r += 16;
	}
	if (!(x & 0xff)) {
		x >>= 8;
		r += 8;
	}
	if (!(x & 0xf)) {
		x >>= 4;
		r += 4;
	}
	if (!(x & 3)) {
		x >>= 2;
		r += 2;
	}
	if (!(x & 1)) {
		x >>= 1;
		r += 1;
	}
	return r;
}
//#endif /* HAVE_FFS */

/* WINDOWS */

