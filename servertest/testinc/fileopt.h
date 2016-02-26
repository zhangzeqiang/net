#ifndef __FILEOPT_H__
#define __FILEOPT_H__

#include <iostream>
#include <string.h>
#include <list>
using namespace std;


namespace Utils {

        class CFileOpt {

        FILE* fd;
        char* buff;

    public:
        CFileOpt () {}
        virtual ~CFileOpt () {}

        /** 
         * 用完记得releaseBuf()
         */
        char* readFileBuf (const char* filename) {
            long lSize;
            size_t result;

            fd = fopen(filename, "rb");
            if (fd == NULL) {
                fputs ("File Error", stderr);
                exit (1);
            }   

            /** 获取文件大小 */
            fseek (fd, 0, SEEK_END);
            lSize = ftell (fd);
            rewind (fd);

            /** 分配内存存储整个文件 */
            buff = (char*) malloc (sizeof (char)*lSize);
            if (buff == NULL) {
                fputs ("Memory error", stderr);
                exit (2);
            }

            /** 将文件拷贝到buffer中 */
            result = fread (buff, 1, lSize, fd);
            if (result != lSize) {
                fputs ("Reading error", stderr);
                exit (3);
            }
            /** 文件读取成功 */
            return buff;
        }

        /**
         * releaseBuf
         */ 
        void releaseBuf () {
            if (buff == NULL) {
                return ;
            }

            if (fd == NULL) {
                return ;
            }
            fclose (fd);
            free (buff);
        }
    };
    
    class StringOpt {
        
        public:
            /**
             * 分割函数
             */
            static list<string> split (string str, string separator) {
                list<string> result;
                int cutAt;

                while ((cutAt = str.find (separator)) != str.npos) {
                    if (cutAt > 0) {
                        result.push_back (str.substr (0, cutAt));
                    } else {
                        // result.push_back ("");
                    }
                    str = str.substr (cutAt + separator.length());
                }
                if (str.length() > 0) {
                    result.push_back (str);
                } else {
                    // result.push_back ("");
                } 
                return result;
            } 
    };
}

#endif /** __FILEOPT_H__ */

