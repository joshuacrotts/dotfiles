module Tutorial0 where

import Data.List (elemIndex, sort, sortBy, group, lookup)
import Control.Arrow (ArrowChoice(right))

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

-- data Maybe a = Nothing | Just a 
-- Maybe Int, Maybe Bool, Maybe String, Maybe (Maybe Int), Maybe (Maybe (Maybe Int)), etc.


-- Why Maybe? Why not return -1?
elemIndex' :: Eq a => a -> [a] -> Maybe Int
elemIndex' e [] = Nothing
elemIndex' e (x : xs) = elemIndexHelper 0 (x : xs)
    where
        elemIndexHelper :: Eq a => Int -> a -> [a] -> Maybe Int
        elemIndexHelper _ _ [] = Nothing
        elemIndexHelper i e (x : xs) | e == x = Just i
                                     | otherwise = elemIndexHelper (i + 1) e xs