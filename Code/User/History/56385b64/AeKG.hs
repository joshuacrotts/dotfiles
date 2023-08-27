module Lib
    ( fact, fib, removeDups, removeDups2
    ) where

fact :: Int -> Int
fact 0 = 1
fact n = n * fact (n - 1)

fib :: Int -> Int
fib 0 = 0
fib 1 = 1
fib n = fib (n - 1) + fib (n - 2)

-- => is a type constraint (?)
-- [] is a list, Eq a says that a is some type that is comparable
removeDups :: Eq a => [a] -> [a]
removeDups [] = []
-- x : xs = (cons x xs)
-- : x is cons. /= is not equal, anything not equal to x, remove it.
removeDups (x : xs) = x : removeDups (filter (=/ x) xs)

-- Another example of removeDups
removeDups2 :: Eq a => [a] -> [a]
removeDups2 [] = []
removeDups2 (x : xs) | x `elem` xs = removeDups2 xs
                     | otherwise x : removeDups2 xs