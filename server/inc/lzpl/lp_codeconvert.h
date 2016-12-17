/*
 * Copyright (c) 2007 Alexey Vatchenko <av@bsdua.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * utf8: implementation of UTF-8 charset encoding (RFC3629).
 */
#ifndef _UTF8_H_
#define _UTF8_H_

#include "lp_base.h"


 //begin声明所处的名字空间
NS_LZPL_BEGIN



#define UTF8_IGNORE_ERROR		0x01
#define UTF8_SKIP_BOM			0x02

#define UTF8_BOM                            (0XBFBBEF)
#define UTF16_BOM                           (0XFEFF)
#define UTF32_BOM                           (0X0000FEFF)


DECLARE size_t LPAPI lpUTF8ToWchar(const char *in, size_t insize, wchar_t *out, size_t outsize, int flags);
DECLARE size_t LPAPI lpWcharToUTF8(const wchar_t *in, size_t insize, char *out, size_t outsize, int flags);

inline DECLARE size_t LPAPI lpUTF8ToUTF16(const char* pcszSrc, size_t tSrcLen, wchar_t* pszDst, size_t tDstLen)
{
	return lpUTF8ToWchar(pcszSrc, tSrcLen, pszDst, tDstLen, UTF8_IGNORE_ERROR | UTF8_SKIP_BOM);
}

inline DECLARE size_t LPAPI lpUTF16ToUTF8(const wchar_t* pcszSrc, size_t tSrcLen, char* pszDst, size_t tDstLen)
{
	return lpWcharToUTF8(pcszSrc, tSrcLen, pszDst, tDstLen, UTF8_IGNORE_ERROR | UTF8_SKIP_BOM);
}

DECLARE BOOL LPAPI lpUTF8ToGB2312(const char *pText, LPUINT32 dwSize, string &pOut);
DECLARE BOOL LPAPI lpGB2312ToUTF8(const char *pText, LPUINT32 dwSize, string& pOut);







//end声明所处的名字空间
NS_LZPL_END


#endif /* !_UTF8_H_ */

 
