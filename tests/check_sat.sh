for i in $(find ../testcases/sat -type f -name '*')
do
    ../src/a.out < $i > tmp
    if [ $(head -n1 tmp) == "SAT" ] ;then
        echo "SAT"
    else
        echo $i
        echo "UNSAT"
    fi
done
rm tmp
