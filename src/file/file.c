/*
Copyright (c) 2017 ricynlee

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#include "file.h"

#if defined(_WIN32)
# include <windows.h>
# include <errno.h>
#endif

// 判断文件是否过大
// 返回值:0文件大小OK,1文件过大,(-1)文件无法访问
// 若无LFS,2GB以上文件即过大
int access_test(const char* path){
    int     fd, ret;
    off_t   offset;
    fd=open(path,O_RDONLY);
    if(fd<0)
        return (-1);

    offset=lseek(fd,0,SEEK_END);
    if(offset<0){
        ret=1;
    }else{
        ret=0;
    }

    close(fd);

    return ret;
}

// Win32 API实现的truncate,功能近似POSIX truncate
// 仅支持操作本地存储器上的文件
#if defined(_WIN32)
int truncate_WIN32_API(const char* path, off_t length){

    HANDLE hF = CreateFile(path, FILE_GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
    if(INVALID_HANDLE_VALUE == hF)
        return EACCES;

# if defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS==64 // LFS on

#   if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    LONG length_HI_32BITS = *((LONG*)(&length) + 1);
    LONG length_LO_32BITS = *((LONG*)(&length));
#   elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    LONG length_HI_32BITS = *((LONG*)(&length));
    LONG length_LO_32BITS = *((LONG*)(&length) + 1);
#   endif

    if(INVALID_SET_FILE_POINTER == SetFilePointer(hF, length_LO_32BITS, &length_HI_32BITS, FILE_BEGIN)){
        CloseHandle(hF);
        return EINVAL;
    }

# else // LFS off

    if(INVALID_SET_FILE_POINTER == SetFilePointer(hF, length, NULL, FILE_BEGIN)){
        CloseHandle(hF);
        return EINVAL;
    }

# endif

    if(! SetEndOfFile(hF)){
        CloseHandle(hF);
        return EIO;
    }

    CloseHandle(hF);
    return 0;
}
#endif
