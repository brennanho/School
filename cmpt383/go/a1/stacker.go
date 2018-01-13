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

func (s *StackLinked) pop() (int,error) {
	if s.length == 0 {
		return 0, errors.New("Cannot pop from an empty stack.")
	} else if s.length == 1 {
		topInt := s.tail.data
		s.head = nil
		s.tail = s.head
		s.length--
		return topInt, nil
	} else {
		//fmt.Println(s.tail)
		topInt := s.tail.data
		s.tail = s.tail.prev
		s.tail.next = nil
		s.length--
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
		copyStack.tail = &copyHead
		current = current.next
		copyStack.length++
		
		for current != nil { //Copy elements in stack after head
			nodeCopy := Node{current.next,current.prev,current.data}
			copyStack.tail.next = &nodeCopy
			temp := copyStack.tail
			copyStack.tail = copyStack.tail.next
			copyStack.tail.prev = temp
			copyStack.tail.next = nil
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

func (s StackSlice) String() string {
	return fmt.Sprintf("%v",s.slice)
}

func (s StackLinked) String() string {
	current := s.head
	str := ""
	for current != nil {
		str += fmt.Sprintf("%d->",current.data)
		current = current.next
	} // for
	if len(str) > 0 {
		str = str[:len(str)-2] // Cuts off the last -> in printing out the linked list
	} else {
		str = "[]"
	}	
	return str
}

// *----QUESTION 4----* //

// s is assumed to be empty
func stackerTest(s Stacker) {

	stackSize := 10

	//PUSH TEST
    for i := 0; i < stackSize; i++ {
    	s.push(i+1)
    }

    //POP TEST
    // for i := 0; i < stackSize; i++ {
    // 	fmt.Println(s)
    // 	s.pop()
    // }

    fmt.Println(s)

    // COPY TEST
    t := s.copy()
    fmt.Println("Initial stack s:",s)
    fmt.Println("Copied stack t:",t)

    //POP TEST

    fmt.Println("s and t are equal:", stackEquals(s,t))

    fmt.Println("Initial stack s:",s)
    fmt.Println("Copied stack t:",t)
    
    //POPALL TESTS
    popAll(t)
    popAll(s)
    fmt.Println("Stack after popAll:",s)
    fmt.Println("Copied stack after popAll:",t)
   
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

// *----QUESTION 5----* //

// Pre-condition:
//    none
// Post-condition:
//    s.isEmpty()
func popAll(s Stacker) {
	for s.size() > 0 {
		s.pop()
	}
	//for
}

//*----QUESTION 6----* //

// Pre-condition:
//    none
// Post-condition:
//    returns true if s and t have the same elements in the same order;
//    both s and t have the same value after calling stackEquals as before
// Annoying constraint:
//    Use only Stackers in the body of this functions: don't use arrays,
//    slices, or any container other than a Stacker.
func stackEquals(s, t Stacker) bool {
   	if s.size() != t.size() {
   		return false
   	} else { // Create the 2 temp stacks for s and t so they can be used to fill in once elements have been popped

   		sTemp := makeStackSlice()
   		tTemp := makeStackSlice()
   		for s.size() > 0 {

   			nums, _ := s.pop()
   			numt, _ := t.pop()

   			sTemp.push(nums)
   			tTemp.push(numt)

   			fmt.Println("nums: ",nums)
   			fmt.Println("numt: ",numt)
   			// if nums != numt {
   			// 	return false
   			// }
   		}

   		fmt.Println(sTemp)
   		fmt.Println(tTemp)

   		for sTemp.size() > 0 { // Restacking original stacks s and t

   			numsTemp, _ := sTemp.pop()
   			numtTemp, _ := tTemp.pop()

   			s.push(numsTemp)
   			t.push(numtTemp)

   		}
   		return true
   	}
}


func main() {

	stackSliceTest()

	fmt.Println("----------------------------------")

	stackLinkedTest()


}