1. A user generates a virtual address 11123456 (in hexadecimal). We will first convert its corresponding binary form to distinguish the upper bits (location in page table) and lower bits (location within page). We have the converted virtual address:

0001 0001 0001 0010 0011 0100 0101 0110 

We have the page size is 4096 bytes which is equal to 2^12 bytes. Then the remainder of bytes used for the page table (X) is then 2^12(X) = 2^32
so X = 2^20. Therefore, we have the 12 lower order bits (0100 0101 0110) is used as a displacement within the page and the upper order 20 bits (0001 0001 0001 0010 0011) is used as displacement within the page table.

Software operations could include determining which pages are selected to be swapped in and out of memory e.g. page replacement algorithm. A hardware operation would be the translation of the virtual address into its corresponding physical address.

2. Let p be the page fault rate i.e. page fault has occured. Then (1 - p) will correspond to a page hit, which will take 100ns provided in the question. Then 
we will get the equation as followed. Solving for p will give us the maximum acceptable page fault rate. 

200ns = (100ns)(1 - p) + (0.7)(20ms)(p) + (0.3)(8ms)(p)
200ns = 100ns - 100ns(p) + 14,000,000ns(p) + 2,400,000ns(p) (all units are in nanoseconds)
100ns = 16,400,100ns(p)
p = 100ns/16,400,100ns
p = 6.1 x 10^-6
p = 0.00061%

Therefore, the maximum acceptable page fault rate would be at 0.00061% for 200ns.