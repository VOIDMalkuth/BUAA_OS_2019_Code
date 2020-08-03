echo 'echo Shell Start' >>test
echo 'echo set a = 1' >>test
echo 'a=1' >>test
echo 'echo set b = 2' >>test
echo 'b=2' >>test
echo 'echo set c = a + b' >>test
echo 'c=$[$a + $b]' >>test
echo 'echo c = $c' >>test
echo 'echo save c to ./file1' >>test
echo 'echo $c >file1' >>test
echo 'echo save b to ./file2' >>test
echo 'echo $b>file2' >>test
echo 'echo save a to ./file3' >>test
echo 'echo $a>file3' >>test
echo 'echo save file1 file2 file3 to file4' >>test
echo 'cat file1>file4' >>test
echo 'cat file2>>file4' >>test
echo 'cat file3>>file4' >>test
echo 'echo save file4 to ./result' >>test
echo 'cat file4>>result' >>test
