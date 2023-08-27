import Data.List

module Main where

removeDups :: Ord a => [a] -> [a]
removeDups as = removeDupsHelper as
    where
        removeDupsHelper :: Ord a => [a] -> [a]
        removeDupsHelper [] = []
        removeDupsHelper [a] = [a]
        removeDupsHelper (a1 : a2 : as) | a1 == a2 = removeDupsHelper (a2 : as)
                                        | otherwise = a1 : removeDupsHelper (a2 : as)

