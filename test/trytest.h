
#define EX_NONE       0
#define EX_OUTOFMEM 100
#define EX_NOFILE   110
#define EX_NODB     314

#define tstout(...) (fprintf(stderr,__VA_ARGS__),fprintf(stderr,"  %s:%d\n",__FILE__,__LINE__),fflush(stderr))
