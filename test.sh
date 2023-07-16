test_file() {
    input=$1
    expected_output=$2

    actual_output=$(./unique_words -i ../../res/$input)

    if [ "$actual_output" = "$expected_output" ]; then
        echo "PASS: Success with '$input'"
    else
        echo "FAIL: Input '$input' produces output '$actual_output', expected '$expected_output'"
    fi
}

test_raw_file() {
    input=$1
    expected_output=$2

    actual_output=$(./unique_words -i ../../res/$input -p)

    if [ "$actual_output" = "$expected_output" ]; then
        echo "PASS: Success with '$input'"
    else
        echo "FAIL: Input '$input' produces output '$actual_output', expected '$expected_output'"
    fi
}

cd out/build/

test_file horsedog.txt 4
test_file million.txt 186

test_raw_file housejack_raw.txt 13
test_raw_file million_raw.txt 186