PROGRAM yuan (i);
    var R,S,C:real; W:array[0..40]of char;
    function func1 ( var id2 : real ;var id4 : real) : integer ;
        procedure proc1 (var id3 : integer;var id4 : real) ;
            function func2 ( var id2 : real) : integer ;
                begin C:=pi*r*2; end ; {end of func2}
            begin C:=pi*r*2; end ; {end of proc1}
        begin C:=pi*r*2; end ; {end of func1}
    begin
    write('请输入圆的半径');
    readln(R);
    if R<5
    then write('半径小于5')
    else write('半径大于或等于5');
    S:=pi*r*r;
    C:=pi*r*2;
    writeln('圆的面积为',s);
    writeln('圆的周长为',c);
    W:='fwefawefsdfawefawefawef';
    writeln(w);
    readln;
    END. {end of yuan}
