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
│   │   └── libhello.a
│   └── static
│       └── libhello.a
├── main.c
├── Makefile
├── obj
│   ├── hello_A.o
│   ├── hello_B.o
│   └── main.o
├── README.md
└── src
    ├── hello_A.c
    └── hello_B.c