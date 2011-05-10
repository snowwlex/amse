
module Lection3 where

--sumList :: [Integer] -> Integer
--sumList [] = 0
--sumList (x:s) = x + sumList s

last_ :: [a] -> a
last_ [] = error "empty list"
last_ [x] = x
last_ (x:ls) = last_ ls

init_ :: [a] -> a
init_ [] = error "empty list"
--init_ [x] = x
init_ (x:ls) = x

some :: [a] -> Int -> a
some [] _ = error "empty list"
some (x:ls) 0 = x
some (x:ls) n = some ls (n-1)

plus :: [a] -> [a] -> [a]
plus [] list = list
plus (x:ls) list = x: (plus ls list)

add_end :: [a] -> a -> [a]
add_end [] el = [el]
add_end (x:ls) el = x : (add_end ls el) 

but_end :: [a] -> [a]
but_end [] = error "empty list"
but_end [a] = []
but_end (x:ls) = x : but_end ls

reverse_ :: [a] -> [a]
reverse_ (x:ls) = reverse_' ls [x]
reverse_' :: [a] -> [a] -> [a]
reverse_' [] list = list
reverse_' (x:ls) list = reverse_' ls (x:list)

















--type String = [Char]
--type Coord = (Double, Double)
type Pair a = (a,a)
type Complex = Pair Double

--find :: String -> Char -> Int
--find [] _ = -1
--find string char = find' string char 0
--find' :: String -> Char -> Int -> Int
--find' [] _ _ = -1
--find' (x:s) char pos | x == char = pos
--                     | otherwise = find' s char (pos+1)
                     
find :: String -> Char -> Int
find string char = sum (
                        map (
                             \(ch,num) -> if char == ch then num+1 
                                                        else 0
                            ) 
                            (zip string [0..(length string)])
                       ) -1 

data Month = January | February | March


winter :: Month -> Bool
winter January = True
winter February = True
winter _ = False

data Coord = Point Double Double


distance :: Coord -> Coord -> Double
distance (Point x1 y1) (Point x2 y2) = sqrt ( (x2-x1)^2 + (y2-y1)^2 )

user :: Coord -> Coord -> Double
user (Point r1 i1) (Point r2 i2) = distance (Point r1 i1) (Point r2 i2)

data IntList = Nil | Integer :++: IntList
sumList :: IntList -> Integer
sumList Nil  = 0
sumList (e :++: ls) = e + sumList ls

insert :: (Ord a)=>a -> [a] -> [a]
insert a [] = [a]
insert a list@(el:ls) | a < el = a:list
                      | otherwise = el:(insert a ls)


search :: (Eq a) => a -> [a] -> Bool
search el list = foldr (||) False (map (\eq -> eq == el) list)

join1 :: [a] -> [a] -> [a]
join1 l1 l2 = foldr (:) l2 l1
--join1 [] l2 = l2
--join1 l1@(el:s) l2 = el : (join1 s l2)

flippy :: [a] -> [a] -> [a]
flippy [] list = list 
flippy (el:s) list = flippy s (el:list)

foldr_ :: (a->b->b) -> b -> [a] -> b
foldr_ func seed []     = seed
foldr_ func seed (el:s) = func el (foldr_ func seed s)
--foldr_ func (func seed el) s

foldl_ :: (b->a->b) -> b -> [a] -> b
foldl_ func seed []     = seed
foldl_ func seed (el:s) = foldl_ func (func seed el) s

reverse2 :: [a] -> [a]
reverse2 list = foldl_ apply [] list
                        where apply l x = x:l
                        
flip_ :: (a->b->c) -> (b->a->c)
flip_ f = f' where f' x y = f y x

reverse3 :: [a] -> [a]
reverse3 list = foldl_ (flip_ (:)) [] list

sqr :: (Num a) => a->a
sqr a = a*a
power4 :: (Num a) => a -> a
power4 = sqr . sqr

