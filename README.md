# Memory Bugs


## Bug 1
```c++

#include <iostream>
#include <string>
#include <fstream>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stoi;
using std::ifstream;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    int n;
    ifstream input(argv[1]);
    if (!input.is_open()) {
        cerr << "Unable to open file " << argv[1] << endl;
        return 1;
    }
    input >> n; // read size of input

    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
        input >> arr[i];
    }

    return 0;

}
```
```shell

g++ -std=c++20 -Wall -g bug1.cpp
valgrind --leak-check=full ./a.out numbers.txt
```

### Output
```
==194== Memcheck, a memory error detector
==194== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==194== Using Valgrind-3.23.0 and LibVEX; rerun with -h for copyright info
==194== Command: ./a.out numbers.txt
==194==
==194==
==194== HEAP SUMMARY:
==194==     in use at exit: 40,000 bytes in 1 blocks
==194==   total heap usage: 4 allocs, 3 frees, 122,392 bytes allocated
==194==
==194== 40,000 bytes in 1 blocks are definitely lost in loss record 1 of 1
==194==    at 0x488724C: operator new[](unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-arm64-linux.so)
==194==    by 0x108F57: main (bug1.cpp:26)
==194==
==194== LEAK SUMMARY:
==194==    definitely lost: 40,000 bytes in 1 blocks
==194==    indirectly lost: 0 bytes in 0 blocks
==194==      possibly lost: 0 bytes in 0 blocks
==194==    still reachable: 0 bytes in 0 blocks
==194==         suppressed: 0 bytes in 0 blocks
==194==
==194== For lists of detected and suppressed errors, rerun with: -s
==194== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

## Bug 2

```c++

#include <iostream>
#include <string>
#include <fstream>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stoi;
using std::ifstream;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    int n;
    ifstream input(argv[1]);
    if (!input.is_open()) {
        cerr << "Unable to open file " << argv[1] << endl;
        return 1;
    }
    input >> n; // read size of input

    int* arr = new int[n];
    for (int i = 0; i <= n; i++) {
        input >> arr[i];
    }

    delete[] arr;
    return 0;

}
```

```shell

g++ -std=c++20 -Wall -g bug2.cpp
valgrind --leak-check=full ./a.out numbers.txt
```

### Output

```
==259== Memcheck, a memory error detector
==259== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==259== Using Valgrind-3.23.0 and LibVEX; rerun with -h for copyright info
==259== Command: ./a.out numbers.txt
==259==
==259== Invalid read of size 4
==259==    at 0x109124: main (bug2.cpp:34)
==259==  Address 0x4e06280 is 0 bytes after a block of size 4,000 alloc'd
==259==    at 0x488724C: operator new[](unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-arm64-linux.so)
==259==    by 0x109097: main (bug2.cpp:26)
==259==
==259== Invalid read of size 4
==259==    at 0x109144: main (bug2.cpp:35)
==259==  Address 0x4e06280 is 0 bytes after a block of size 4,000 alloc'd
==259==    at 0x488724C: operator new[](unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-arm64-linux.so)
==259==    by 0x109097: main (bug2.cpp:26)
==259==
==259== Invalid write of size 4
==259==    at 0x109160: main (bug2.cpp:35)
==259==  Address 0x4e06280 is 0 bytes after a block of size 4,000 alloc'd
==259==    at 0x488724C: operator new[](unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-arm64-linux.so)
==259==    by 0x109097: main (bug2.cpp:26)
==259==
42007598
==259==
==259== HEAP SUMMARY:
==259==     in use at exit: 0 bytes in 0 blocks
==259==   total heap usage: 5 allocs, 5 frees, 87,416 bytes allocated
==259==
==259== All heap blocks were freed -- no leaks are possible
==259==
==259== For lists of detected and suppressed errors, rerun with: -s
==259== ERROR SUMMARY: 3 errors from 3 contexts (suppressed: 0 from 0)
```

## Bug 3

```c++

#include <iostream>
#include <string>
#include <fstream>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stoi;
using std::ifstream;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    int n;
    ifstream input(argv[1]);
    if (!input.is_open()) {
        cerr << "Unable to open file " << argv[1] << endl;
        return 1;
    }
    input >> n; // read size of input

    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
        input >> arr[i];
    }

    input.close();
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
        arr[i] = sum / arr[i];
    }
    cout << sum << endl;

    delete arr;
    return 0;

}
```

```shell

/master-class-alpha/memory-bugs# g++ -std=c++20 -Wall -g bug3.cpp
bug3.cpp: In function ‘int main(int, char**)’:
bug3.cpp:39:12: warning: ‘void operator delete(void*, std::size_t)’ called on pointer returned from a mismatched allocation function [-Wmismatched-new-delete]
   39 |     delete arr;
      |            ^~~
bug3.cpp:26:25: note: returned from ‘void* operator new [](std::size_t)’
   26 |     int* arr = new int[n];
      |                         ^
/master-class-alpha/memory-bugs# valgrind --leak-check=full ./a.out numbers.txt
```

### Output

```
==277== Memcheck, a memory error detector
==277== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==277== Using Valgrind-3.23.0 and LibVEX; rerun with -h for copyright info
==277== Command: ./a.out numbers.txt
==277==
42007598
==277== Mismatched new/delete size value: 4
==277==    at 0x4889440: operator delete(void*, unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-arm64-linux.so)
==277==    by 0x1091AF: main (bug3.cpp:39)
==277==  Address 0x4e052e0 is 0 bytes inside a block of size 4,000 alloc'd
==277==    at 0x488724C: operator new[](unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-arm64-linux.so)
==277==    by 0x109097: main (bug3.cpp:26)
==277==
==277== Mismatched free() / delete / delete []
==277==    at 0x48894A4: operator delete(void*, unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-arm64-linux.so)
==277==    by 0x1091AF: main (bug3.cpp:39)
==277==  Address 0x4e052e0 is 0 bytes inside a block of size 4,000 alloc'd
==277==    at 0x488724C: operator new[](unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-arm64-linux.so)
==277==    by 0x109097: main (bug3.cpp:26)
==277==
==277==
==277== HEAP SUMMARY:
==277==     in use at exit: 0 bytes in 0 blocks
==277==   total heap usage: 5 allocs, 5 frees, 87,416 bytes allocated
==277==
==277== All heap blocks were freed -- no leaks are possible
==277==
==277== For lists of detected and suppressed errors, rerun with: -s
```

## Bug 4

```c++

```

```shell

/master-class-alpha/memory-bugs# g++ -std=c++20 -Wall -g bug4.cpp
/master-class-alpha/memory-bugs# valgrind --leak-check=full ./a.out numbers.txt


```
### Output
```
==284== Memcheck, a memory error detector
==284== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==284== Using Valgrind-3.23.0 and LibVEX; rerun with -h for copyright info
==284== Command: ./a.out numbers.txt
==284==
42007598
Done!
==284== Invalid free() / delete / delete[] / realloc()
==284==    at 0x488AD60: operator delete[](void*) (in /usr/libexec/valgrind/vgpreload_memcheck-arm64-linux.so)
==284==    by 0x1091E3: main (bug4.cpp:41)
==284==  Address 0x4e052e0 is 0 bytes inside a block of size 4,000 free'd
==284==    at 0x488AD60: operator delete[](void*) (in /usr/libexec/valgrind/vgpreload_memcheck-arm64-linux.so)
==284==    by 0x1091AF: main (bug4.cpp:39)
==284==  Block was alloc'd at
==284==    at 0x488724C: operator new[](unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-arm64-linux.so)
==284==    by 0x109097: main (bug4.cpp:26)
==284==
==284==
==284== HEAP SUMMARY:
==284==     in use at exit: 0 bytes in 0 blocks
==284==   total heap usage: 5 allocs, 6 frees, 87,416 bytes allocated
==284==
==284== All heap blocks were freed -- no leaks are possible
==284==
==284== For lists of detected and suppressed errors, rerun with: -s
==284== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```
## Bug 5
> Let's test Bug3 or Bug 2 with first line (numbers.txt) set to 10,000.
```c++

```

```shell


```
### Output
```

```

## Summary

| Program | Problem         | Description                               |
|---------|-----------------|-------------------------------------------|
| bug1    | Memory Leak     | The program finishes without deallocating |
| bug2    | Heap Corruption | Invalid heap access (stack possible too!) |
| bug3    | Missing []      | Allocated with [] deleted without them    |
| bug4    | Double delete   | Deallocating an allocated memory twice    |

