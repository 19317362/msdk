/*
**
**  lzma wrapper
**  Ŀǰֻʵ���˽�ѹ����
**  jinpeiqi 2013.07.11
*/

#ifndef _BFLZMA_H_
#define _BFLZMA_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Types.h"


#ifdef __cplusplus
extern "C" {
#endif

    typedef void (*callback) (char *filename, uint64_t size, void *p); /* ��ѹcallback */

    typedef struct
    {
        callback cb;              /* callback */
    } lzmalite_param;

    /**
     * @param dest  // Ŀ��Ŀ¼��ע��һ��Ҫȷ����Ŀ¼�Ѿ�����
     * @param src   // ��Ҫ��ѹ��7z�ļ�
     * @param param // ��������ѹ�ص���δ��ɣ�
     * @return      // 1 ʧ�� 0 �ɹ�
     * @remarks ��ѹ�ļ�.
     */
    int MY_CDECL lzmalite_uncompress(const char *dest, const char *src, lzmalite_param *param);


#ifdef __cplusplus
}
#endif

#endif  /* _BFLZMA_H_ */