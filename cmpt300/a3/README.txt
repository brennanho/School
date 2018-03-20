A3 Implementation

In my OS design, I would like to describe a few OS implementations I chose.

1. Priority Queues - I am using a 3 level priority queue reflecting high, med, and low priorities. With this choice, I am aware starvation can occur if there are always available higher
					 priority processes over lower ones.

2. Quantum - 	  I have made the decision to randomly assign burst times to newly created processes ranging from 1-10 units. A time quantum in my simulation lasts for 5
				  units time. This is to make it easier for the tester (and you the marker) to see the Quantum function is implemented. When the user enters Q, this subtracts 5 from the 
				  current process' burst time. If the new burst time is <= 0, then the process will be terminated. If the new burst time is > 0, it will be added back to the appropriate
				  ready queue (pre-emption). Note that I could have had the user manually enter in burst times for the newly created jobs, but this soon becomes quite tedious for testing 
				  the simulation. Also, there are no parameters required in the Quantum function in the assignment description, hence why I chose this implementation for the question.

3. Messages - 	  I have implemented a message inbox that can store multiple messages sent to various users. 

4. Init Process -	'init' process is not stored in any queue, and is a shared global variable that will be pointed to by the runningProc if and only if there are no other available processes
			   	 	 to run or the simulation first starts.
					 