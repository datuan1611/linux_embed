##static lib && shared lib

#tree
.
├── bin
│   ├── shared_exec
│   └── static_exec
├── inc
│   ├── hello_A.h
│   └── hello_B.h
├── lib
│   ├── shared
│   │   └── libhello.so
│   └── static
│       └── libhello.a
├── main.c
├── Makefile
├── obj
│   ├── main.o
│   ├── shared
│   │   ├── hello_A.o
│   │   └── hello_B.o
│   └── static
│       ├── hello_A.o
│       └── hello_B.o
└── src
    ├── hello_A.c
    └── hello_B.c
