
#include "globalInclude.h"

#include "gc.h"

extern int entryMain(); // the entry to yy llvm ir


int global_argc = 0;
char** global_argv = NULL;

uv_loop_t *uv_global_loop;
int main(int argc, char* argv[]) {

    // save global paramters
    global_argc = argc;
    global_argv = argv;

    // initialize garbage collection
    GC_INIT();

    // uv_replace_allocator(&GC_MALLOC, GC_REALLOC, GC_MALLOC, GC_FREE);

    // initialize uv
    uv_global_loop = uv_default_loop();
    


    return entryMain();
}



int informResultRec (uint64_t result[], int prevPred) {
    // assume it is a tuple
    uint64_t header = result[0];
    int type = (header >> (62 - 6)) &  0b11111;
    int length = (header >> (62 - 22)) &  0b1111111111;

    int preds[] = {0, 0, 660, 680, 670, 720};
    int shouldPrintQuote = 2 <= type && type <= 5 && preds[type] < prevPred;

    if(shouldPrintQuote){
        fprintf(stderr,"「");
    }


    switch (type)
    {
    case 1:
        fprintf(stderr,"Received a function closure (length is %d). Did you define a function?\n", length);
        // for (int i = 0; i < length ; i ++){
        //     fprintf(stderr,"%d : ", i);
        //     informResult((yy_ptr)result[i+1]);
        // }
        break;
    case 2:
        fprintf(stderr,"卷");
        informResultRec((yy_ptr) result[1], preds[type]);
        break;
   
    case 3:
        informResultRec((yy_ptr) result[1], preds[type]);
        for (int i = 1; i < length ; i++) {
            fprintf(stderr,"与");
            informResultRec((yy_ptr) result[1+i], preds[type]);
        }
        break;
     case 4:
        fprintf(stderr,"%s临", (char*)result[2]);
        informResultRec((yy_ptr) result[3], preds[type]);
        break;
    case 5:
        fprintf(stderr,"元");
        break;
    case 6:
        fprintf(stderr,"『");
        fprintf(stderr,"%s", (char *)result[1]);
        fprintf(stderr,"』");
        break;
    case 7:
        fprintf(stderr,"%d", (int64_t)result[1]);
        break;
    case 8:
        fprintf(stderr,"%f", *((double*)&result[1]));
        break;

    
    
    default:
        fprintf(stderr, "ERROR: result is not interpretable. because the type %d is not defined for the runtime.\n", type);
        break;
    }

    if(shouldPrintQuote){
        fprintf(stderr,"」");
    }


    return 0;
}
int informResult (uint64_t result[]) {
    return informResultRec(result, 0);
}

