package main
import "fmt"
import "errors"

type Stacker interface 
{
    // Pre-condition:
    //    none
    // Post-condition:
    //    returns true if this stack has no elements, and false otherwise
    // Performance:
    //    runs in O(1) time
    isEmpty() bool

    // Pre-condition:
    //    none
    // Post-condition:
    //    returns the number of elements in this stack
    // Performance:
    //    runs in O(1) time
    size() int

    // puts x on the top of the stack (increasing size by 1)
    // Pre-condition:
    //    none
    // Post-condition:
    //    puts a copy of x on top of the stack (and the size of the stack
    //    increases by 1)
    // Performance:
    //    runs in O(1) time most of the time; can occasionally run in O(n)
    //    time if memory re-organization is needed
    push(x int)

    // Pre-condition:
    //    none
    // Post-condition:
    //    If the stack is empty, returns 0 and a non-nil error value (with a
    //    helpful error message).
    //    If the stack is not empty, returns a copy of the top element of the
    //    stack, and a nil value for the error.
    //    In both cases, the stack has the same value after value after calling
    //    peek() as before.
    // Performance:
    //    runs in O(1) time
    peek() (int, error)

    // Pre-condition:
    //    none
    // Post-condition:
    //    If the stack is empty, returns 0 and a non-nil error value (with a
    //    helpful error message). The stack remains empty.
    //    If the stack is not empty, returns, and removes, the top element of
    //    the stack, and a nil value for the error. In this case, the size of
    //    the stack decreases by 1.

    // Performance:
    //    runs in O(1) time
    pop() (int, error)

    // Pre-condition:
    //    none
    // Post-condition:
    //    Returns a copy of this stack.
    // Performance:
    //    runs in O(n) time, where n is the size of the stack
    copy() Stacker
} // Stacker

// *----QUESTION 1----*

//struct used to implement the Stacker interface
type stackSlice struct {
    slice []int
    Stacker
}

func (s stackSlice) isEmpty() bool {
    if len(s.slice) == 0 {
        return true
    }
    return false
}

func (s stackSlice) size() int {
    return len(s.slice)
}

func (s *stackSlice) push(x int) {
	s.slice = append(s.slice,x)
}

func (s stackSlice) peek() (int,error) {
    if len(s.slice) == 0 {
        return 0, errors.New("0: The stack is empty and there are no elements to peek.")
    } else {
        topInt := s.slice[len(s.slice)-1]
        return topInt, errors.New("1: Successfully peeked at the top integer in the stack.")
    }
}

func (s *stackSlice) pop() (int,error) {
    if len(s.slice) == 0 {
        return 0, errors.New("0: Cannot remove integer element from an empty stack.")
    } else {
    	var num int
        num, s.slice = s.slice[len(s.slice)-1], s.slice[:len(s.slice)-1]
        return num, errors.New("1: Successfully returned an integer at the top of the stack.")
    }
}

func (s stackSlice) copy() Stacker {
    copyStack := stackSlice{}
    copy(copyStack.slice, s.slice)
    copyStack.slice = s.slice
    return &copyStack
}

//Returns a new empty stack using an int slice representation
func makeStackSlice() Stacker {
    stackSlice := stackSlice{}
    return &stackSlice
}

// *----END OF QUESTION 1----*

func main() {

    s := makeStackSlice()
    s.push(4)


    t := s.copy()

    nums,errs := s.pop()
    numt,errt := t.pop()

    
    fmt.Printf("%d\n", nums)
    fmt.Printf("%s\n", errs)
    fmt.Printf("%d\n", numt)
    fmt.Printf("%s\n", errt)

    fmt.Printf("%v", s.slice)

}