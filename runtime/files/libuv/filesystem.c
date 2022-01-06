#include "../globalInclude.h"


yy_ptr yyReadFileSync(yy_ptr filenamearg) {
    uv_fs_t open_req;
    uv_fs_t read_req;
    // open_req = GC_MALLOC(sizeof(uv_fs_t))

    int openResult = uv_fs_open(uv_default_loop(), &open_req, 
        addr_to_string(filenamearg), O_RDONLY, 
        0, NULL);

    if (openResult < 0 ){
        // error happpened
    }
    char* result = GC_MALLOC(1);
    int resultLength = 0;
    result[0] = '\0';
    char* tempBuffer = GC_MALLOC(65535);
    uv_buf_t uvBuf= uv_buf_init(tempBuffer, 65534);
    
    int nread = uv_fs_read(uv_default_loop(), &read_req, open_req.result, &uvBuf, 1, -1, NULL);
    while (read_req.result > 0) {
        resultLength += nread;
        result = GC_REALLOC(result, resultLength +1);
        tempBuffer[nread] = '\0';
        strlcat(result, tempBuffer, resultLength+1);
        nread = uv_fs_read(uv_default_loop(), &read_req, open_req.result, &uvBuf, 1, -1, NULL);
    }
    if (read_req.result == UV_EOF | read_req.result == 0){
        // done
    } else {
        fprintf(stderr, "(error reading file ), %s", uv_strerror(read_req.result));
        fflush(stderr);
    }
    return string_to_addr(result);

}


yy_ptr yyListDirectorySync(yy_ptr dirname) {
    uv_fs_t open_req;
    uv_fs_t read_req;
    // open_req = GC_MALLOC(sizeof(uv_fs_t))

    int openResult = uv_fs_opendir(uv_default_loop(), &open_req, 
        addr_to_string(dirname), NULL);

    if (openResult < 0 ){
        // error happpened
        fprintf(stderr, "(error reading dir ), %s", uv_strerror(open_req.result));
        fflush(stderr);
    }
    uv_dir_t *dirs = open_req.ptr;
    int nread = uv_fs_readdir(uv_default_loop(), &read_req, dirs, NULL);
    if (read_req.result < 0){
        fprintf(stderr, "(error reading filedir ), %s", uv_strerror(read_req.result));
        fflush(stderr);
    }
    if (nread < 0){
        fprintf(stderr, "(error reading filedir ), %s", uv_strerror(nread));
        fflush(stderr);
    }

    yy_ptr entries[nread];

    for(int i = 0; i < nread; i++){
        char * name = dirs->dirents[i].name;
        entries[i] = string_to_addr(name);
    }


    return array_to_iso_addr(nread, entries);

}