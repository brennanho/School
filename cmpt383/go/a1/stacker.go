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

// *----QUESTION 1----* //

//struct used to implement the Stacker interface
type StackSlice struct {
    slice []int
    Stacker
}

func (s StackSlice) isEmpty() bool {
    if len(s.slice) == 0 {
        return true
    }
    return false
}

func (s StackSlice) size() int {
    return len(s.slice)
}

func (s *StackSlice) push(x int) {
	s.slice = append(s.slice,x)
}

func (s StackSlice) peek() (int,error) {
    if len(s.slice) == 0 {
        return 0, errors.New("The stack is empty and there are no elements to peek.")
    } else {
        topInt := s.slice[len(s.slice)-1]
        return topInt, nil
    }
}

func (s *StackSlice) pop() (int,error) {
    if len(s.slice) == 0 {
        return 0, errors.New("Cannot pop from an empty stack.")
    } else {
    	var num int
        num, s.slice = s.slice[len(s.slice)-1], s.slice[:len(s.slice)-1]
        return num, nil
    }
}

func (s StackSlice) copy() Stacker {
    copyStack := StackSlice{}
    copy(copyStack.slice, s.slice)
    copyStack.slice = s.slice
    return &copyStack
}

//Returns a new empty stack using an int slice representation
func makeStackSlice() Stacker {
    StackSlice := StackSlice{}
    return &StackSlice
}

// *----QUESTION 2----* //

//Node struct required for link list implementation
type Node struct {
	prev *Node
	next *Node
	data int
}

//Used for linked list implementation of Stacker interface
type StackLinked struct {
	head *Node
	tail *Node
	length int
	Stacker
}

func (s StackLinked) isEmpty() bool {
	if s.length == 0 {
		return true
	}
	return false
}

func (s StackLinked) size() int {
	return s.length
}

func (s *StackLinked) push(x int) {
	newNode := Node{s.tail,nil,x}
	if s.length == 0 {
		s.head = &newNode
		s.tail = &newNode
	} else {
		s.tail.next = &newNode
		s.tail = s.tail.next
		s.tail.next = nil
	}
	s.length++
}

func (s StackLinked) peek() (int,error) {
	if s.length == 0 {
        return 0, errors.New("The stack is empty and there are no elements to peek.")
    } else {
        topInt := s.tail.data
        return topInt, nil
    }
}

func (s StackLinked) pop() (int,error) {
	if s.length == 0 {
		return 0, errors.New("Cannot pop from an empty stack.")
	} else {
		topInt := s.tail.data
		s.tail = s.tail.prev
		s.tail.next = nil
		return topInt, nil
	}
}

func (s StackLinked) copy () Stacker {
	if s.head == nil {
		return nil
	} else {
		current := s.head
		copyStack := StackLinked{}
		copyHead := Node{nil,nil,current.data}
		copyStack.head = &copyHead
		current = current.next
		
		for current != nil {
			nodeCopy := Node{current.next,current.prev,current.data}
			copyStack.tail = &nodeCopy
			copyStack.tail = copyStack.tail.next
			copyStack.length++

			current = current.next
		}
		return &copyStack
	}
}

// returns a new empty stack using a linked list representation
func makeStackLinked() Stacker {
   linkedStack := StackLinked{}
   return &linkedStack
}

// *----QUESTION 3----* //


func main() {

	// Slice tests
    // s := makeStackSlice()
    // s.push(4)


    // t := s.copy()

    // nums,errs := s.pop()
    // numt,errt := t.pop()

    
    // fmt.Printf("%d\n", nums)
    // fmt.Printf("%s\n", errs)
    // fmt.Printf("%d\n", numt)
    // fmt.Printf("%s\n", errt)

    //Linked list tests

    a := makeStackLinked()
    a.push(5)
    a.push(2)
    a.push(3)

    numa,erra := a.peek()
    fmt.Printf("%d\n", numa)
    fmt.Printf("%v\n", erra)


}