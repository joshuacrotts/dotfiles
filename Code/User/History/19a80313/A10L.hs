module Tutorial0 where

import Data.List (elemIndex, sort, sortBy, group, lookup)

-- To get the type of an expression, use :t

-- Basic lists and functions on lists
fact :: Int -> Int
fact 0 = 1
fact n = n * fact (n - 1)

fact' :: Int -> Int
fact' n
    | n == 0 = 1
    | n > 0 = n * fact' (n - 1)
    | otherwise = error "fact' is undefined for negative numbers"

fact'' :: Int -> Int
fact'' n = case n of
    0 -> 1
    n -> n * fact'' (n - 1)

quickSort :: Ord a => [a] -> [a]
quickSort [] = []
quickSort (x : xs) = left ++ [x] ++ right
    where
        left = quickSort (filter (<= x) xs)
        right = quickSort (filter (> x) xs)

-- + is the list concatenation operator
removeDupsSorted :: Ord a => [a] -> [a]
removeDupsSorted ls = removeDupsSortedHelper (sort ls)
    where
        removeDupsSortedHelper :: Ord a => [a] -> [a]
        removeDupsSortedHelper [] = []
        removeDupsSortedHelper [a] = [a]
        removeDupsSortedHelper (a1 : a2 : as) | a1 == a2 = removeDupsSortedHelper (a2 : as)
                                        | otherwise = a1 : removeDupsSortedHelper (a2 : as)

-- . is the function composition operator
removeDups :: Ord a => [a] -> [a]
removeDups = removeDupsSorted . sort

merge :: Ord a => [a] -> [a] -> [a]
merge [] ys = ys
merge xs [] = xs
merge (x : xs) (y : ys) | x <= y = x : merge xs (y : ys)
                        | otherwise = y : merge (x : xs) ys

-- Data types (enumerations/sum types)
data UserType = Basic | Premium

-- canViewMedia :: MediaId -> UserType -> Bool

-- a is a type variable.
-- Algebraic data types
-- data Maybe a = Nothing | Just a

-- Below is a GADT (generalized algebraic data type)
-- data Maybe a where
--    Nothing :: Maybe a
--    Just :: a -> Maybe a
-- Maybe Int, Maybe Bool, Maybe String, Maybe (Maybe Int), Maybe (Maybe (Maybe Int)), etc.


-- Why Maybe? Why not return -1?
-- -1 may be a valid value, so we can't use it to indicate an error
elemIndex' :: Eq a => a -> [a] -> Maybe Int
elemIndex' e [] = Nothing
elemIndex' e as = elemIndexHelper 0 as
    where
        elemIndexHelper i [] = Nothing
        elemIndexHelper i (a : as) 
            | e == a = Just i
            | otherwise = elemIndexHelper (i + 1) as

-- A binary tree is either empty or a branch with a value and two subtrees
data Tree a
    = Empty
    | Branch a (Tree a) (Tree a)
    deriving (Show, Eq)

tree0 = Branch 2 Empty Empty
tree1 = Branch 1 (Branch 2 Empty Empty) (Branch 3 Empty Empty)
tree2 = Branch 1 (Branch 2 (Branch 3 Empty Empty) Empty) (Branch 4 Empty Empty)
tree3 = Branch 1 (Branch 2 (Branch 3 Empty Empty) (Branch 4 Empty Empty)) (Branch 5 Empty Empty)

height :: Eq a => Tree a -> Int
height Empty = 0
height (Branch x t1 t2) = 1 + max (height t1) (height t2)


-- HOFs, compositions, compositions with context
wc :: String -> [(Int, String)]
-- get the "words", sort them, group them, and then get the length of each group
-- group creates sublists of the same elements. 
-- Example group [1, 1, 2, 3, 3, 3] = [[1, 1], [2], [3, 3, 3]]
wc =  reverse . sort . map (\g -> (length g, head g)) . group . sort . words

-- Types (typedefs)
type Address = String
type Name = String
type Generation = String