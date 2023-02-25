clang++ -fsycl -fsycl-targets=nvptx64-nvidia-cuda $1 -L/usr/lib/gcc/x86_64-linux-gnu/11 -o app -I /usr/include/c++/11 -I /usr/include/x86_64-linux-gnu/c++/11
