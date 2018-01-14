package a1

import "fmt"
import "testing"

// *----QUESTION 4----* //

// s is assumed to be empty
func stackerTest(s Stacker) {

	stackSize := 10

	//PUSH TEST
    for i := 0; i < stackSize; i++ {
    	s.push(i+1)
    	fmt.Println("Pushing numbers... ", s, "     Size =",s.size()) // SIZE TEST
    }

    //IS EMPTY TEST 1
    isEmpty := s.isEmpty()
    fmt.Println("\nStack is empty:",isEmpty, "\n")

    // COPY TEST
    t := s.copy()
    fmt.Println("s (orig):",s)
    fmt.Println("t (copy):",t)

    // TEST IS EQUAL (should evaluate to true)
    fmt.Println("s and t are equal:", stackEquals(s,t))

    // TEST IS EQUAL (should evaluate to false)
    t.pop()
    fmt.Println("\ns :",s)
    fmt.Println("t :",t)
    fmt.Println("s and t are equal:", stackEquals(s,t))

    //PEEK TEST
    num,err := s.peek()
    numt,errt := t.peek()
    fmt.Println("\nPeek top of s:",num, ", error:",err)
    fmt.Println("Peek top of t:",numt, ", error:",errt,"\n")

    //POP STACK
    for i := 0; i < stackSize; i++ {
    	s.pop()
    	fmt.Println("Popping numbers... ", s, "     Size =",s.size()) // SIZE TEST 
    }

    //PUSH TEST 2
    for i := 0; i < stackSize; i++ {
        s.push(i+1)
        fmt.Println("Pushing numbers... ", s, "     Size =",s.size()) // SIZE TEST
    }

    //POPALL TESTS
    popAll(s)
    t = s.copy()

    fmt.Println("\nStack s after popAll:",s)
    fmt.Println("Copied stack t after popAll:",t)

    //IS EMPTY TEST 2
    isEmpty = s.isEmpty()
    fmt.Println("Stack is empty:",isEmpty,"\n")

    //POP EMPTY STACK TEST
    num,err = s.pop()
    fmt.Println("Pop top:",num, ", error:",err)

    //PEEK EMPTY STACK TEST
    num,err = s.peek()
    fmt.Println("Peek top:",num, ", error:",err,"\n")
   
}

func stackSliceTest() {
    s := makeStackSlice()
    stackerTest(s)
    fmt.Println("all StackSlice tests passed")
}

func stackLinkedTest() {
    s := makeStackLinked()
    stackerTest(s)
    fmt.Println("all StackLinked tests passed")
}

//*----QUESTION 7----* //
func TestAssignment1(t *testing.T){
    fmt.Println("Begin slice implementation of Stacker interface test.\n")
    stackSliceTest()

    fmt.Println("\n-------------------------------------------------------------\n")

    fmt.Println("Begin linked list implementation of Stacker interface test.\n")
    stackLinkedTest()
}