package main

import "fmt"

func passArray(s []int) {
	fmt.Printf("In pass array: %d\n", s[0])
}

func vc_counts(s string) (int,int) {
	cons,vow := 0, 0
	for i, _ := range s {
		if s[i] == 'a' || s[i] == 'e' || s[i] == 'i' || s[i] == 'o' || s[i] == 'u' {
			vow += 1
		} else if s[i] == 'y' {
			vow += 1 
			cons += 1
		} else {
			cons += 1
		}
	}
	return vow, cons
}

func add(x int, y int) int {
	return x + y
}

func c_add(x int) func(y int) int {
	return func(y int) int {
		return x + y
	}
}

func curry(f func(int,int) int) func(int) func(int) int {
	return func(x int) func(int) int {
		return func(y int) int {
			return f(x,y)
		}
	}
}



func main() {
	array := [4]int{0}
	slice := []int{0}
    fmt.Printf("hello, world\n")

    passArray(slice)

    fmt.Printf("%d\n",len(array))
    fmt.Printf("%d", len(slice))

    v,c := vc_counts("hmmi")

    fmt.Printf("\nvowels: %d cons: %d", v, c)
    fmt.Println(add(3, 5))

    add3 := c_add(3)
    fmt.Println(add3(5))
    fmt.Println(c_add(2)(4))

}