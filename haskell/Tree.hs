module Tree where

data Tree a = Empty | Node (Tree a) a (Tree a)

--sort :: (Ord a)=> [a] -> [a]
build :: (Ord a)=> [a] -> Tree a
insert :: (Ord a)=> a -> Tree a -> Tree a
--flatten :: Tree a -> [a]

--sort ls = flatten (build ls)

build [] = Empty
build (x:s) = insert x (build s)

insert e Empty = Node Empty e Empty
insert e (Node t1 root t2) | e < t1 = Node (insert e t1) root t2
                           | otherwise = Node t1 root (insert e t2) 

             
--flatten Empty = []
--flatten (Node t1 root t2) = (flatten t1) ++ (root : (flatten t2))
