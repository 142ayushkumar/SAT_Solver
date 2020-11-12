for i in $(find ../testcases/unsat -type f -name '*')
do
    ../src/a.out < $i > tmp
    if [ $(head -n1 tmp) == "SAT" ] ;then
        echo "SAT"
    else
        echo "UNSAT"
    fi
done
rm tmp
