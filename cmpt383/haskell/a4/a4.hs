--Basic Functions

--Q1
snoc :: a -> [a] -> [a]
snoc x [] = [x]
snoc x (y:ys) = y:(snoc x ys)

--Q2
myappend :: [a] -> [a] -> [a]
myappend [] [] = []
myappend (x:xs) [] = (x:xs)
myappend [] (y:ys) = (y:ys)
myappend (x:xs) (y:ys) = x:(myappend xs (y:ys))

--Q3
myreverse :: [a] -> [a]
myreverse [] = []
myreverse (x:xs) = (myappend (reverse xs) [x])

--Q4 HELPERS FROM NOTES
smallest_divisor :: Int -> Int
smallest_divisor n
     | n < 0     = error "n must be >= 0"
     | n == 0    = 0
     | n == 1    = 1
     | otherwise = head (dropWhile (\x -> n `mod` x /= 0) [2..n])

is_prime :: Int -> Bool
is_prime n | n < 2     = False
           | otherwise = (smallest_divisor n) == n

reverseInt :: Int -> Int
reverseInt = read . myreverse .show

--Q4
count_emirps :: Int -> Int
count_emirps 0 = 0
count_emirps n | (is_prime n) && (is_prime (reverseInt n)) && (n /= (reverseInt n)) = 1 + count_emirps(n - 1)
               |                                                         otherwise  = 0 + count_emirps(n - 1)

--Q5
biggest_sum :: [[Int]] -> [Int]
biggest_sum [(x:xs)] = (x:xs)
biggest_sum (x:xs) | (sum x) > (sum (head xs)) = biggest_sum (x:(tail xs))
                   |                 otherwise = biggest_sum xs

--Q6 
greatest :: (a -> Int) -> [a] -> a
greatest f [x] = x
greatest f (x:xs) | (f x) > (f (head xs)) = greatest f (x:(tail xs))
                  |             otherwise = greatest f xs

--Basic Bits

--Q7
is_bit :: Int -> Bool
is_bit x | (x == 0) || (x == 1) = True
         |            otherwise = False

--Q8
flip_bit :: Int -> Int
flip_bit x | x == 0 = 1
           | x == 1 = 0
           | otherwise = error "Error: flip_bit takes in a bit type i.e. 0 or 1"

--Q9 

--a)
is_bit_seq1 :: [Int] -> Bool
is_bit_seq1 [] = True
is_bit_seq1 (x:xs) | (is_bit x) == True = is_bit_seq1 xs
                   | otherwise = False

--b)
is_bit_seq2 :: [Int] -> Bool
is_bit_seq2 [] = True
is_bit_seq2 (x:xs) =
    if (is_bit x)
        then (is_bit_seq2 xs)
    else
        False

--c)
is_bit_seq3 :: [Int] -> Bool
is_bit_seq3 [] = True
is_bit_seq3 (x:xs) =  ((filter (/= True) (map is_bit (x:xs))) == [])

--Q10

--a)
invert_bits1 :: [Int] -> [Int]
invert_bits1 [] = []
invert_bits1 (x:xs) = (flip_bit x):(invert_bits1 xs)

--b)
invert_bits2 :: [Int] -> [Int]
invert_bits2 [] = []
invert_bits2 (x:xs) = map flip_bit (x:xs)

--c) 
invert_bits3 :: [Int] -> [Int]
invert_bits3 [] = []
invert_bits3 (x:xs) = [ flip_bit x | x <- (x:xs), x == 0 || x == 1]

--Q11
bit_count :: [Int] -> (Int,Int)
bit_count [] = (0,0)
bit_count (x:xs) = ((length (filter (== 0) (x:xs))), (length (filter (== 1) (x:xs))))

--Q12
all_basic_bit_seqs :: Int -> [[Int]]
all_basic_bit_seqs n | n < 1 = [[]]
                     | n == 1 = [[0],[1]]
                     | otherwise = myappend (map (myappend [0]) (all_basic_bit_seqs (n-1))) (map (myappend [1]) (all_basic_bit_seqs (n-1)))

--A Custom List Data Type
data List a = Empty | Cons a (List a)
    deriving Show

--Q13
toList :: [a] -> List a
toList [] = Empty
toList (x:xs) = Cons x (toList xs)

--Q14
toHaskellList :: List a -> [a]
toHaskellList Empty = []
toHaskellList (Cons first Empty) = [first]
toHaskellList (Cons first rest) = first:(toHaskellList rest)

--Q15
append :: List a -> List a -> List a
append a b = toList (myappend (toHaskellList a) (toHaskellList b))

--Q16 HELPER FUNCTION - Inverted filter 
filterNot :: (a -> Bool) -> [a] -> [a]
filterNot pred a = filter (not . pred) a

--Q16
removeAll :: (a -> Bool) -> List a -> List a
removeAll f a = toList (filterNot f (toHaskellList a))

--Q17 HELPER FUNCTION - Quicksort for normal Haskell lists
sortHaskell :: Ord a => [a] -> [a]
sortHaskell [] = []
sortHaskell [a] = [a]
sortHaskell (x:xs) = myappend (sortHaskell [i | i <- xs, i < x]) (myappend [x] (sortHaskell [i | i <- xs, i >= x]))

--Q17 - Quicksort
sort :: Ord a => List a -> List a
sort a = toList (sortHaskell (toHaskellList a))

fib :: Int -> Int
fib n |  n == 1 = 1
      |  n == 2 = 1
      |  otherwise = fib (n - 1) + fib (n - 2)

