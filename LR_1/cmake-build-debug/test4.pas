program test(input, output);
    const pai = 3.14; {常量声明}
    type list = array [0 .. 10] of integer; {类型声明}
    var a,b,c: integer; {变量声明}
        e,f: real;
        rec: record
                i: integer;
                j,k: char
             end; {end of record}
        arr: array [0 .. 7, 0 .. 3] of char;
        brr: array [0 .. 7]of array [0 .. 3, 6 .. 10] of integer;
    begin
        a := 3;
        b := 4;
        c := 5;
        a := 4 * 7; {正确}
        a := 0.2 * 3; {错误，REAL赋值给了INTEGER}
        rec := 4 * 7; {错误，INTEGER赋值给了RECORD}
        a := 'c' * 7; {错误，运算对象类型错误，这时表达式的类型为VOID}
    end. {end of test}