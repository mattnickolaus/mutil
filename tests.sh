
if objdump -tC mutil | grep "mutil_align_forward"; then
    echo "Compiler did not remove mutil function check it MUTILDEF is used ahead of function definition and that #define MUTILDEF static inline is included in source file (mutil.c)"
else 
    echo "Passed - compiler removed unused mutil functions"
fi
